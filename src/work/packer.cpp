#include "packer.h"
#include "push/pushmanager.h"
#include "datamanager.h"

extern QString super_account;
extern QString token_name;

Packer::Packer()
{
    needStop = false;
}

void Packer::setAccountName(const QString &name)
{
    accountName = name;
}

void Packer::setCallback(const packedtrx_callback& func)
{
    this->func = func;
}

void Packer::run()
{
    while(!needStop) {
        PushManager push(false);
        connect(&push, &PushManager::trxPacked, this, [&](const QByteArray& data){
            emit newPackedTrx(data);
            func(QString("PACKED: %1 to %2.").arg(accountName).arg(super_account));
        });
        push.transferToken(accountName, super_account, QString("0.0001 %1").arg(token_name));
    }
}

void Packer::stop()
{
    needStop = true;
}
