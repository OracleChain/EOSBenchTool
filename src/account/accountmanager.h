#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QMap>
#include <QVector>
#include <QString>

#include "codebase/ec/eos_key_encode.h"

class AccountManager
{
public:
    AccountManager();
    static AccountManager& instance();

    void addAccounts(const QString& name, const QPair<std::string, std::string>& keypairs);
    void removeAll();
    QPair<std::string, std::string> listKeys(const QString& account);
    QVector<std::string> listAccounts();
    int count() const;

private:
    QMap<QString, QPair<std::string, std::string>> accounts;
};

#endif // ACCOUNTMANAGER_H
