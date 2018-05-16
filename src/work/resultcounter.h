#ifndef RESULTCOUNTER_H
#define RESULTCOUNTER_H

#include <QThread>
#include <QSet>

class ResultCounter : public QThread
{
    Q_OBJECT

public:
    ResultCounter();
    ~ResultCounter();

    virtual void run();
    void stop();

signals:
    void oneRoundFinished();
    void handledTrxsArrived(qint64, qint64);
    void duration(qint64 );
    void maxTPS(double );

private slots:
    void get_block_returned(const QByteArray& data);

private:
    void get_block(int block_num);

private:
    bool needStop;

    qint64 curTrxs;
    qint64 curTimestamp;
};

#endif // RESULTCOUNTER_H
