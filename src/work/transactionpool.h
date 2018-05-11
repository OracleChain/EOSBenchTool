#ifndef TRANSACTIONPOOL_H
#define TRANSACTIONPOOL_H

#include <QThread>
#include <QVector>
#include <QMutex>

class TransactionPool : public QThread
{
    Q_OBJECT

public:
    TransactionPool();
    void setTargetSize(int size);

    virtual void run();
    void stop();

public slots:
    void incomingPackedTrxs(const QByteArray& data);

signals:
    void packedTrxPoolFulfilled();

private:
    int get_block_info();

private:
    QVector<QByteArray> packedTransactions;
    int targetSize;

    bool needStop;

    QMutex mutex;
};

#endif // TRANSACTIONPOOL_H
