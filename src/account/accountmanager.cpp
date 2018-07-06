#include "accountmanager.h"
#include "codebase/chain/chainbase.h"
#include "setting/config.h"

extern QString super_account;

AccountManager::AccountManager()
{
}

AccountManager &AccountManager::instance()
{
    static AccountManager manager;
    return manager;
}

void AccountManager::addAccounts(const QString &name, const QPair<std::string, std::string> &keypairs)
{
    if (accounts.find(name) != accounts.end()) {
        accounts[name] = keypairs;
    }

    accounts.insert(name, keypairs);
}

void AccountManager::removeAll()
{
    auto superKey = accounts[super_account];
    accounts.clear();
    accounts.insert(super_account, superKey);
}

QPair<std::string, std::string> AccountManager::listKeys(const QString &account)
{
    QPair<std::string, std::string> result;
    if (accounts.find(account) != accounts.end()) {
        result = accounts.value(account);
    }

    return result;
}

QVector<std::string> AccountManager::listAccounts()
{
    QVector<std::string> result;
    auto keys = accounts.keys();
    for (int i = 0; i < keys.size(); ++i) {
        auto name = keys.at(i);
        if (name.compare(QString::fromStdString(EOS_SYSTEM_ACCOUNT)) == 0
                || name.compare(super_account) == 0) {
            continue;
        }

        result.push_back(name.toStdString());
    }

    return result;
}

int AccountManager::count() const
{
    return accounts.count();
}
