#ifndef PACKER_H
#define PACKER_H

#include <QThread>
#include <functional>

using packedtrx_callback = std::function<void(const QString&)>;

class Packer : public QThread
{
    Q_OBJECT

public:
    Packer();

    void setAccountName(const QString& name);
    void setCallback(const packedtrx_callback &func);

    virtual void run();
    void stop();

signals:
    void newPackedTrx(const QByteArray& data);

private:
    QString accountName;
    bool needStop;
    packedtrx_callback func;
};

#endif // PACKER_H
