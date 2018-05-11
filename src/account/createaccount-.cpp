#include "createaccount.h"
#include "utility/httpclient.h"
#include "mainwindow.h"
#include "chain/eosnewaccount.h"
#include "chain/eosbytewriter.h"
#include "chain/action.h"
#include "chain/chainmanager.h"
#include "ec/typechainid.h"
#include "ec/eos_key_encode.h"
#include "chain/packedtransaction.h"
#include "accountmanager.h"
#include "setting/config.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>
#include <QDebug>
#include <QEventLoop>
#include <QMetaObject>

#include <memory>

extern QString privateKey;

CreateAccount::CreateAccount(QObject *parent) :
    httpc(new HttpClient)
{
}

CreateAccount::~CreateAccount()
{
    delete httpc;
}

bool CreateAccount::create(const QString &firstName, int num)
{
    if (num <= 0) {
        return false;
    }

   for (int i = 0; i < 1; ++i) {
       eos_key ownerKey, activeKey;

       QString newName = firstName + QString::number(i);
       QEventLoop loop;
       if (httpc) {
           httpc->get_info();
           auto infoconn = std::make_shared<QMetaObject::Connection>();
           *infoconn = connect(httpc, &HttpClient::responseData, [=](const QByteArray& info){
               disconnect(*infoconn);

                SignedTransaction signedTxn;
                QByteArray getRequiredKeysparam = packGetRequiredKeysParam(newName, ownerKey, activeKey, info, signedTxn);

                httpc->get_required_keys(std::move(QString::fromStdString(getRequiredKeysparam.toStdString())));
                auto rkeyconn = std::make_shared<QMetaObject::Connection>();
                *rkeyconn = connect(httpc, &HttpClient::responseData, [&](const QByteArray& keysInfo){
                    disconnect(*rkeyconn);

                    QByteArray pushTxnParam = packPushTransactionParam(keysInfo, privateKey.toStdString(), signedTxn);
                    if (pushTxnParam.isEmpty()) {
                        QMessageBox::warning(nullptr, "Error", "Error ocurs while creating account.");
                        return false;
                    }

                    httpc->push_transaction(std::move(QString::fromStdString(pushTxnParam.toStdString())));
                    auto txnconn = std::make_shared<QMetaObject::Connection>();
                    *txnconn = connect(httpc, &HttpClient::responseData, [=](const QByteArray& pushInfo){
                        disconnect(*txnconn);

                        QJsonObject obj = QJsonDocument::fromJson(pushInfo).object();
                        if (obj.isEmpty()) {
                            return false;
                        }

                        if (obj.contains("code") || obj.contains("error")) {
                            return false;
                        }

                        AccountManager::instance().addAccounts(newName, QPair<eos_key, eos_key>(ownerKey, activeKey));
                    });
                });
           });
       }

       loop.exec();
   }

   return true;
}

QByteArray CreateAccount::packGetRequiredKeysParam(const QString& newName, const eos_key& ownerKey, const eos_key& activeKey, const QByteArray& getInfo, SignedTransaction& signedTxn)
{
    EOSNewAccount newAccount(EOS_SYSTEM_ACCOUNT, newName.toStdString(),
                            ownerKey.get_eos_public_key(), activeKey.get_eos_public_key(),
                             EOS_SYSTEM_ACCOUNT);

    std::vector<unsigned char> hexData = newAccount.dataAsHex();

    signedTxn = ChainManager::createTransaction(EOS_SYSTEM_ACCOUNT, newAccount.getActionName(), std::string(hexData.begin(), hexData.end()),
                                                ChainManager::getActivePermission(EOS_SYSTEM_ACCOUNT), getInfo);
    QJsonObject txnObj = signedTxn.toJson().toObject();

    QJsonArray avaibleKeys;

    std::string pubKey = eos_key::get_eos_public_key_by_wif(privateKey.toStdString());
    if (pubKey.empty()) {
        return QByteArray();
    }

    avaibleKeys.append(QJsonValue(QString::fromStdString(pubKey)));

    QJsonObject obj;
    obj.insert("available_keys", avaibleKeys);
    obj.insert("transaction", txnObj);
    return QJsonDocument(obj).toJson();
}

QByteArray CreateAccount::packPushTransactionParam(const QByteArray& getRequiredKeysData, const std::string &wif, SignedTransaction& signedTxn)
{
    QJsonArray array = QJsonDocument::fromJson(getRequiredKeysData).object().value("required_keys").toArray();
    if (!array.size()) {
        return QByteArray();
    }

    bool match = false;
    std::string pubKey = eos_key::get_eos_public_key_by_wif(wif);
    for (int i = 0; i < array.size(); ++i) {
        std::string rkey = array.at(i).toString().toStdString();
        if (rkey.compare(pubKey) == 0) {
            match = true;
            break;
        }
    }

    if (!match) {
        return QByteArray();
    }

    std::vector<unsigned char> private_key = eos_key::get_private_key_by_wif(wif);

    signedTxn.sign(private_key, TypeChainId());
    PackedTransaction packedTxn(signedTxn, "none");

    QJsonObject obj = packedTxn.toJson().toObject();

    return QJsonDocument(obj).toJson();
}
