#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QDir>
#include <QSettings>
#include <QCoreApplication>

// network
extern QString base_url;
extern QString url_port;

// account
extern QString super_account;
extern QString super_private_key;

// token
extern QString contract_account;
extern QString token_name;
extern int total_tokens;

// common
extern int thread_num;
extern int trx_size;
extern int batch_size;

class Config
{
public:
    Config()
    {
        settings = new QSettings(iniFile(), QSettings::IniFormat);
    }

    ~Config()
    {
        if (settings) {
            delete settings;
        }
    }

    void loadSetting()
    {
        if (settings) {
            base_url = settings->value("Network/url", "").toString();
            url_port = settings->value("Network/port", "").toString();

            super_account = settings->value("Account/super").toString();
            super_private_key = settings->value("Account/key").toString();

            contract_account = settings->value("Token/contract").toString();
            token_name = settings->value("Token/name").toString();
            total_tokens = settings->value("Token/total").toInt();

            thread_num = settings->value("Common/thread").toInt();
            trx_size = settings->value("Common/trx_size").toInt();
            batch_size = settings->value("Common/batch_size").toInt();
        }
    }

    void saveSettings()
    {
        if (settings) {
            settings->setValue("Network/url", base_url);
            settings->setValue("Network/port", url_port);

            settings->setValue("Account/super", super_account);
            settings->setValue("Account/key", super_private_key);

            settings->setValue("Token/contract", contract_account);
            settings->setValue("Token/name", token_name);
            settings->setValue("Token/total", total_tokens);

            settings->setValue("Common/thread", thread_num);
            settings->setValue("Common/trx_size", trx_size);
            settings->setValue("Common/batch_size", batch_size);
        }
    }

private:
    QString iniFile()
    {

        QString curPath = QCoreApplication::applicationDirPath();
        QDir cur(curPath);
        QString strDataPath = curPath + "/data";
        QDir data(strDataPath);
        if (!data.exists()) {
            cur.mkdir("data");
        }

        return strDataPath + "/config.ini";
    }

private:
    QSettings *settings;
};


#endif // CONFIG_H
