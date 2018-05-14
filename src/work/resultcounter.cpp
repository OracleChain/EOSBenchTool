#include "resultcounter.h"
#include "datamanager.h"

#include "codebase/utility/httpclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QEventLoop>
#include <QDateTime>

ResultCounter::ResultCounter()
{
    needStop = false;
}

ResultCounter::~ResultCounter()
{
    handledTransactions.clear();
}

void ResultCounter::run()
{
    int beginBlock = DataManager::instance().beginBlockNum();
    int endBlock = DataManager::instance().endBlockNum();
    qint64 beginTime = 0;
    qint64 lastTime = 0;
    double maxtps = 0.0;

    QSet<QString> totalTrxs;

    for (int block_num = beginBlock; block_num <= endBlock && !needStop; ++block_num) {
        get_block(block_num);

        if (block_num == beginBlock) {
            beginTime = curTimestamp;
        }

        totalTrxs = totalTrxs.unite(handledTransactions);
        qint64 delta = curTimestamp - beginTime;
        emit handledTrxsArrived(totalTrxs, delta);

        int interval = curTimestamp - lastTime;
        if (interval) {
            maxtps = std::max(handledTransactions.size() * 1.0 / interval * 1000, maxtps);
            emit maxTPS(maxtps);
        }

        lastTime = curTimestamp;
    }
}

void ResultCounter::stop()
{
    needStop = true;
}

void ResultCounter::get_block_returned(const QByteArray &data)
{
    QJsonObject obj = QJsonDocument::fromJson(data).object();
    do
    {
        if (obj.isEmpty()) {
            break;
        }

        QString ts = obj.value("timestamp").toString();
        if (ts.isEmpty()) {
            break;
        }

        curTimestamp = QDateTime::fromString(ts, Qt::ISODateWithMs).toMSecsSinceEpoch();

        QJsonArray regions = obj.value("regions").toArray();
        if (regions.isEmpty()) {
            break;
        }

        for (int i = 0; i < regions.size(); ++i) {
            QJsonObject regObj = regions.at(i).toObject();
            if (regObj.isEmpty()) {
                continue;
            }

            QJsonArray cyclesArray = regObj.value("cycles_summary").toArray();
            if (cyclesArray.isEmpty()) {
                continue;
            }

            for (int j = 0; j < cyclesArray.size(); ++j) {
                QJsonArray cArray = cyclesArray.at(j).toArray();
                if (cArray.isEmpty()) {
                    continue;
                }

                for (int k = 0; k < cArray.size(); ++k) {
                    QJsonObject cObj = cArray.at(k).toObject();
                    if (cObj.isEmpty()) {
                        continue;
                    }

                    QJsonArray txnArray = cObj.value("transactions").toArray();
                    if (txnArray.isEmpty()) {
                        continue;
                    }

                    if (txnArray.isEmpty()) {
                        continue;
                    }

                    // so many loops... -_-!
                    handledTransactions.clear();
                    for (int m = 0; m < txnArray.size(); ++m) {
                        QString id = txnArray.at(m).toObject().value("id").toString();
                        handledTransactions.insert(id);
                    }
                }
            }
        }
    } while(false);

    emit oneRoundFinished();
}

void ResultCounter::get_block(int block_num)
{
    QJsonObject obj;
    obj.insert("block_num_or_id", QJsonValue(block_num));

    QEventLoop loop;
    connect(this, &ResultCounter::oneRoundFinished, &loop, &QEventLoop::quit);

    HttpClient httpc;
    httpc.get_block(QString::fromStdString(QJsonDocument(obj).toJson().toStdString()));
    connect(&httpc, &HttpClient::responseData, this, &ResultCounter::get_block_returned);

    loop.exec();
}
