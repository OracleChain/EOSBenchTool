#include "pushmanager.h"
#include "account/accountmanager.h"

#include "codebase/chain/packedtransaction.h"
#include "codebase/chain/chainmanager.h"
#include "codebase/utility/httpclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QByteArray>
#include <QEventLoop>

extern QString contract_account;

PushManager::PushManager(bool flag) : httpc(new HttpClient), result(false)
{
    sendTrx = flag;
}

PushManager::~PushManager()
{
    delete httpc;
}

bool PushManager::transferToken(const QString &fromAccount, const QString &toAccount, const QString &quantity)
{
    transactionId.clear();

    QJsonObject obj;
    obj.insert("from", QJsonValue(fromAccount));
    obj.insert("to", QJsonValue(toAccount));
    obj.insert("quantity", QJsonValue(quantity));
    obj.insert("memo", QJsonValue(QString::number(rand() % 10000)));

    actor = fromAccount;
    action = "transfer";

    return make_push(contract_account, action, QJsonDocument(obj).toJson());
}

QString PushManager::getLastTransactionId()
{
    return transactionId;
}

bool PushManager::make_push(const QString &code, const QString &action, const QByteArray &input)
{
    if (code.isEmpty() || action.isEmpty() || input.isEmpty()) {
        return false;
    }

    QJsonObject obj;
    obj.insert("action", QJsonValue(action));
    obj.insert("code", QJsonValue(code));
    QJsonObject argObj = QJsonDocument::fromJson(input).object();
    obj.insert("args", QJsonValue(argObj));

    result = false;

    QEventLoop loop;
    connect(this, &PushManager::oneRoundFinished, &loop, &QEventLoop::quit);

    if (httpc) {
        httpc->abi_json_to_bin(QJsonDocument(obj).toJson());
        connect(httpc, &HttpClient::responseData, this, &PushManager::abi_json_to_bin_returned);
    }

    loop.exec();

    return result;
}

QByteArray PushManager::packGetRequiredKeysParam()
{
    QJsonObject abiBinObj = QJsonDocument::fromJson(abiJsonToBinData).object();
    QString binargs = abiBinObj.value("binargs").toString();

    signedTrx = ChainManager::createTransaction(contract_account.toStdString(), action.toStdString(), binargs.toStdString(), ChainManager::getActivePermission(actor.toStdString()), getInfoData);

    QJsonObject TrxObj = signedTrx.toJson().toObject();

    QJsonArray avaibleKeys;
    QPair<std::string, std::string> keys = AccountManager::instance().listKeys(actor);
    avaibleKeys.append(QJsonValue(QString::fromStdString(eos_key::get_eos_public_key_by_wif(keys.second))));

    QJsonObject obj;
    obj.insert("available_keys", avaibleKeys);
    obj.insert("transaction", TrxObj);
    return QJsonDocument(obj).toJson();
}

QByteArray PushManager::packPushTransactionParam()
{
    QJsonArray array = QJsonDocument::fromJson(getRequiredKeysData).object().value("required_keys").toArray();
    if (!array.size()) {
        return QByteArray();
    }

    QPair<std::string, std::string> keys = AccountManager::instance().listKeys(actor);
    std::vector<unsigned char> pri;
    bool match = false;
    for (int i = 0; i < array.size(); ++i) {
        std::string key = array.at(i).toString().toStdString();
        if (key.compare(eos_key::get_eos_public_key_by_wif(keys.first)) == 0) {
            pri = eos_key::get_private_key_by_wif(keys.first);
            match = true;
            break;
        }

        if (key.compare(eos_key::get_eos_public_key_by_wif(keys.second)) == 0) {
            pri = eos_key::get_private_key_by_wif(keys.second);
            match = true;
            break;
        }
    }

    if (!match) {
        return QByteArray();
    }

    QJsonObject info = QJsonDocument::fromJson(getInfoData).object();
    if (info.isEmpty()) {
        return QByteArray();
    }

    signedTrx.sign(pri, TypeChainId::fromHex(info.value("chain_id").toString().toStdString()));
    PackedTransaction packedTrx(signedTrx, "none");

    QJsonObject obj = packedTrx.toJson().toObject();

    return QJsonDocument(obj).toJson();
}

void PushManager::abi_json_to_bin_returned(const QByteArray &data)
{
    disconnect(httpc, &HttpClient::responseData, this, &PushManager::abi_json_to_bin_returned);

    if (data.isEmpty()) {
        emit oneRoundFinished();
        return;
    }

    abiJsonToBinData.clear();
    abiJsonToBinData = data;

    if (httpc) {
        httpc->get_info();
        connect(httpc, &HttpClient::responseData, this, &PushManager::get_info_returned);
    }
}

void PushManager::get_info_returned(const QByteArray &data)
{
    disconnect(httpc, &HttpClient::responseData, this, &PushManager::get_info_returned);

    if (data.isEmpty()) {
        emit oneRoundFinished();
        return;
    }

    getInfoData.clear();
    getInfoData = data;

    QByteArray param = packGetRequiredKeysParam();
    if (param.isEmpty()) {
        emit oneRoundFinished();
        return;
    }

    if (httpc) {
        httpc->get_required_keys(param);
        connect(httpc, &HttpClient::responseData, this, &PushManager::get_required_keys_returned);
    }
}

void PushManager::get_required_keys_returned(const QByteArray &data)
{
    disconnect(httpc, &HttpClient::responseData, this, &PushManager::get_required_keys_returned);

    if (data.isEmpty()) {
        emit oneRoundFinished();
        return;
    }

    getRequiredKeysData.clear();
    getRequiredKeysData = data;

    QByteArray param = packPushTransactionParam();
    emit trxPacked(param);

    if (param.isEmpty() || !sendTrx) {
        emit oneRoundFinished();
        return;
    }

    if (httpc) {
        httpc->push_transaction(param);
        connect(httpc, &HttpClient::responseData, this, &PushManager::push_transaction_returned);
    }
}

void PushManager::push_transaction_returned(const QByteArray &data)
{
    disconnect(httpc, &HttpClient::responseData, this, &PushManager::push_transaction_returned);

    QJsonObject obj = QJsonDocument::fromJson(data).object();
    if (!obj.isEmpty()) {
        if (!obj.contains("code") && !obj.contains("error")) {
            result = true;

            transactionId = obj.value("transaction_id").toString();
        }
    }

    emit oneRoundFinished();
}
