#include "createaccount.h"
#include "accountmanager.h"

#include "codebase/chain/eosnewaccount.h"
#include "codebase/chain/eosbytewriter.h"
#include "codebase/chain/action.h"
#include "codebase/chain/chainmanager.h"
#include "codebase/ec/typechainid.h"
#include "codebase/chain/packedtransaction.h"
#include "codebase/utility/httpclient.h"
#include "codebase/utility/utils.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>
#include <QDebug>
#include <QEventLoop>

extern QString super_private_key;

CreateAccount::CreateAccount() :
    httpc(new HttpClient)
{
}

CreateAccount::~CreateAccount()
{
    delete httpc;
}

int CreateAccount::create(int threadNum, const create_account_callback& func)
{
    if (threadNum <= 0) {
        return 0;
    }

    AccountManager::instance().removeAll();

    for (int i = 0; i < threadNum; ++i) {
        eos_key owner, active;
        keys.clear();
        keys.push_back(owner);
        keys.push_back(active);

        newAccountName = createNewName();

        bool res = false;

        QEventLoop loop;
        connect(this, &CreateAccount::oneRoundFinished, &loop, &QEventLoop::quit);

        if (httpc) {
            httpc->get_info();
            connect(httpc, &HttpClient::responseData, this, &CreateAccount::get_info_returned);
        }

        loop.exec();

        res = !(AccountManager::instance().listKeys(newAccountName).first.empty());

        func(newAccountName, res);
    }

    return AccountManager::instance().count() - 1;  // super account.
}

void CreateAccount::get_info_returned(const QByteArray &data)
{
    disconnect(httpc, &HttpClient::responseData, this, &CreateAccount::get_info_returned);

    getInfoData.clear();
    getInfoData = data;

    QByteArray param = packGetRequiredKeysParam();
    if (param.isNull()) {
        emit oneRoundFinished();
        return;
    }

    if (httpc) {
        httpc->get_required_keys(param);
        connect(httpc, &HttpClient::responseData, this, &CreateAccount::get_required_keys_returned);
    }
}

void CreateAccount::get_required_keys_returned(const QByteArray &data)
{
    disconnect(httpc, &HttpClient::responseData, this, &CreateAccount::get_required_keys_returned);

    getRequiredKeysData.clear();
    getRequiredKeysData = data;

    QByteArray param = packPushTransactionParam();
    if (param.isNull()) {
        emit oneRoundFinished();
        return;
    }

    if (httpc) {
        httpc->push_transaction(param);
        connect(httpc, &HttpClient::responseData, this, &CreateAccount::push_transaction_returned);
    }
}

void CreateAccount::push_transaction_returned(const QByteArray &data)
{
    disconnect(httpc, &HttpClient::responseData, this, &CreateAccount::push_transaction_returned);

    do
    {
        QJsonObject obj = QJsonDocument::fromJson(data).object();
        if (obj.isEmpty()) {
            break;
        }

        if (obj.contains("code") || obj.contains("error")) {
            break;
        }

        QPair<std::string, std::string> keypair;
        keypair.first = keys.at(0).get_wif_private_key();
        keypair.second = keys.at(1).get_wif_private_key();
        AccountManager::instance().addAccounts(newAccountName, keypair);
    } while(false);

    emit oneRoundFinished();
}

QByteArray CreateAccount::packGetRequiredKeysParam()
{
    if (getInfoData.isEmpty()) {
        return QByteArray();
    }

    EOSNewAccount newAccount(EOS_SYSTEM_ACCOUNT, newAccountName.toStdString(),
                             keys.at(0).get_eos_public_key(), keys.at(1).get_eos_public_key(),
                             EOS_SYSTEM_ACCOUNT);

    std::vector<unsigned char> hexData = newAccount.dataAsHex();

    signedTxn = ChainManager::createTransaction(EOS_SYSTEM_ACCOUNT, newAccount.getActionName(), std::string(hexData.begin(), hexData.end()),
                                                ChainManager::getActivePermission(EOS_SYSTEM_ACCOUNT), getInfoData);
    QJsonObject txnObj = signedTxn.toJson().toObject();

    QJsonArray avaibleKeys;
    std::string pub = eos_key::get_eos_public_key_by_wif(super_private_key.toStdString());
    avaibleKeys.append(QJsonValue(QString::fromStdString(pub)));

    QJsonObject obj;
    obj.insert("available_keys", avaibleKeys);
    obj.insert("transaction", txnObj);
    return QJsonDocument(obj).toJson();
}

QByteArray CreateAccount::packPushTransactionParam()
{
    if (getRequiredKeysData.isEmpty()) {
        return QByteArray();
    }

    QJsonArray array = QJsonDocument::fromJson(getRequiredKeysData).object().value("required_keys").toArray();
    if (!array.size()) {
        return QByteArray();
    }

    bool match = false;
    std::string pub = eos_key::get_eos_public_key_by_wif(super_private_key.toStdString());
    for (int i = 0; i < array.size(); ++i) {
        std::string key = array.at(i).toString().toStdString();
        if (key.compare(pub) == 0) {
            match = true;
            break;
        }
    }

    if (!match) {
        return QByteArray();
    }

    std::vector<unsigned char> pri = eos_key::get_private_key_by_wif(super_private_key.toStdString());
    if (pri.empty()) {
        return QByteArray();
    }

    QJsonObject info = QJsonDocument::fromJson(getInfoData).object();
    if (info.isEmpty()) {
        return QByteArray();
    }

    signedTxn.sign(pri, TypeChainId::fromHex(info.value("chain_id").toString().toStdString()));
    PackedTransaction packedTxn(signedTxn, "none");

    QJsonObject obj = packedTxn.toJson().toObject();

    return QJsonDocument(obj).toJson();
}

QString CreateAccount::createNewName()
{
    static const char *char_map = "12345abcdefghijklmnopqrstuvwxyz";
    int map_size = strlen(char_map);
    QString newName;

    for (int i = 0; i < 5; ++i) {
        int r = rand() % map_size;
        newName += char_map[r];
    }

    return newName;
}
