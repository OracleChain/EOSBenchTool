#ifndef CreateAccount_H
#define CreateAccount_H

#include <QObject>
#include <QVector>
#include <functional>

#include "codebase/chain/signedtransaction.h"

class HttpClient;
class eos_key;

using create_account_callback = std::function<void(const QString&, bool)>;

class CreateAccount : public QObject
{
    Q_OBJECT

public:
    explicit CreateAccount();
    ~CreateAccount();

    int create(int threadNum, const create_account_callback& func);

signals:
    void oneRoundFinished();

private slots:
    void get_info_returned(const QByteArray& data);
    void get_required_keys_returned(const QByteArray& data);
    void push_transaction_returned(const QByteArray& data);

private:
    QByteArray packGetRequiredKeysParam();
    QByteArray packPushTransactionParam();

    QString createNewName();

private:
    QVector<eos_key> keys;
    HttpClient *httpc;

    QByteArray getInfoData;
    QByteArray getRequiredKeysData;

    SignedTransaction signedTxn;

    QString newAccountName;
};

#endif // CreateAccount_H
