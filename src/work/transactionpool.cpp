#include "transactionpool.h"
#include "utility/httpclient.h"
#include "datamanager.h"

#include <QMutexLocker>
#include <QDateTime>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

extern int batch_size;

TransactionPool::TransactionPool()
{
    needStop = false;
}

void TransactionPool::setTargetSize(int size)
{
    targetSize = size;
}

void TransactionPool::run()
{
    DataManager::instance().setBeginBlockNum(get_block_info());

     HttpClient httpc;
     int sz = packedTransactions.size();
    for (int i = 0; i < sz && !needStop; i += batch_size) {
        QEventLoop loop;
        connect(&httpc, &HttpClient::responseData, &loop, &QEventLoop::quit);

        QJsonArray array;
        int range = sz - i > batch_size ? batch_size : sz - i;
        for (int j = 0; j < range; ++j) {
            QJsonObject val = QJsonDocument::fromJson(packedTransactions.at(i+j)).object();
            array.append(val);
        }

        httpc.push_transactions(QString::fromStdString(QJsonDocument(array).toJson().toStdString()));
        loop.exec();
    }

    DataManager::instance().setEndBlockNum(get_block_info());

    packedTransactions.clear();
}

void TransactionPool::stop()
{
    needStop = true;
}

void TransactionPool::incomingPackedTrxs(const QByteArray &data)
{
    QMutexLocker locker(&mutex);
    packedTransactions.push_back(data);

    if (packedTransactions.size() >= targetSize) {
        emit packedTrxPoolFulfilled();
    }
}

int TransactionPool::get_block_info()
{
    HttpClient httpc;
    int num = 0;

    QEventLoop loop;
    connect(&httpc, &HttpClient::responseData, [&](const QByteArray& data){
        QJsonObject obj = QJsonDocument::fromJson(data).object();
        if (!obj.isEmpty()) {
            num = obj.value("head_block_num").toInt();
        }

        loop.quit();
    });
    httpc.get_info();
    loop.exec();

    return num;
}
