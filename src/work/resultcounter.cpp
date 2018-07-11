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
    curTrxs = 0;
}

ResultCounter::~ResultCounter()
{
}

void ResultCounter::run()
{
    int beginBlock = DataManager::instance().beginBlockNum();
    int endBlock = DataManager::instance().endBlockNum();
    qint64 beginTime = 0;
    qint64 lastTime = 0;
    double maxtps = 0.0;

    for (int block_num = beginBlock; block_num <= endBlock && !needStop; ++block_num) {
        get_block(block_num);

        if (block_num == beginBlock) {
            beginTime = curTimestamp;
        }

        qint64 delta = curTimestamp - beginTime;
        emit handledTrxsArrived(curTrxs, delta);

        int interval = curTimestamp - lastTime;
        if (interval) {
            maxtps = std::max(curTrxs * 1.0 / interval * 1000, maxtps);
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
    curTrxs = 0;

    auto obj = QJsonDocument::fromJson(data).object();
    do
    {
        if (obj.isEmpty()) {
            break;
        }

        auto ts = obj.value("timestamp").toString();
        if (ts.isEmpty()) {
            break;
        }

        curTimestamp = QDateTime::fromString(ts, Qt::ISODateWithMs).toMSecsSinceEpoch();

        auto txnArray = obj.value("transactions").toArray();
        curTrxs = txnArray.size();
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
    connect(&httpc, &HttpClient::responseData, this, &ResultCounter::get_block_returned);
    httpc.request(FunctionID::get_block, QJsonDocument(obj).toJson());

    loop.exec();
}
