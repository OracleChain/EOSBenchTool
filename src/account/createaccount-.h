#ifndef CREATEACCOUNT_H
#define CREATEACCOUNT_H

#include <QObject>
#include <QPair>

#include "chain/signedtransaction.h"

class eos_key;
class HttpClient;

class CreateAccount : public QObject
{
    Q_OBJECT

public:
    explicit CreateAccount(QObject *parent = 0);
    ~CreateAccount();

    bool create(const QString& firstName, int num);

private:
    QByteArray packGetRequiredKeysParam(const QString& newName, const eos_key& ownerKey, const eos_key& activeKey, const QByteArray& getInfo, SignedTransaction& signedTxn);
    QByteArray packPushTransactionParam(const QByteArray& getRequiredKeysData, const std::string& wif, SignedTransaction &signedTxn);

private:
    HttpClient *httpc;
};

#endif // CREATEACCOUNT_H
