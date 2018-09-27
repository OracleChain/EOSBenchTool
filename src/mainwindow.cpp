#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "account/createaccount.h"
#include "account/accountmanager.h"
#include "push/pushmanager.h"
#include "setting/config.h"
#include "misc/customtabstyle.h"
#include "misc/aboutframe.h"

#include <QTabBar>

QString base_url;
QString url_port;
QString super_account = "eosio";
QString super_private_key = "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3";
QString contract_account = "eosio.token";
QString token_name = "EOS";
int trx_size = 5000;
int total_tokens = 100000;
int thread_num = 8;
int batch_size = 300;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    about(nullptr),
    resultCounter(nullptr),
    trxpool(nullptr),
    total_trxs(0)
{
    ui->setupUi(this);
    ui->tabWidget->setTabPosition(QTabWidget::West);
    ui->tabWidget->tabBar()->setStyle(std::move(new CustomTabStyle));
    initButtonStyle();
    updateAbout();

    loadSettings();
    loadSuperKey();

    ui->pushButtonRun->setEnabled(false);   // before init, run is disabled.
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateAbout();
}

void MainWindow::updateAbout()
{
#ifndef Q_OS_MAC
    if (!about) {
        about = new AboutFrame(this);
    }

    QPoint p(this->rect().bottomLeft().x(), this->rect().bottomLeft().y());
    p.setY(p.y() - about->geometry().height());
    about->move(p);
#endif
}

void MainWindow::initButtonStyle()
{
    QString testingBtnQss = "QPushButton:hover:!pressed {"
                        "background-color: rgb(75,74,74);"
                        "color: rgb(255, 255, 255);"
                        "border-radius: 4px;}"
                  "QPushButton:hover:pressed {"
                        "background-color: rgb(70,69,69);"
                         "color: rgb(255, 255, 255);"
                         "border-radius: 4px;}"
                  "QPushButton {"
                        "background-color: rgb(95, 94, 94);"
                        "color: rgb(255, 255, 255);"
                        "border-radius: 4px;}"
                  "QPushButton:!enabled{"
                        "background-color: rgb(125, 124, 124);"
                  "}";

    QString settingQss = "QPushButton:hover:!pressed {"
                   "background-color: rgb(50,120,240);"
                   "color: rgb(255, 255, 255);"
                   "border-radius: 4px;}"
             "QPushButton:hover:pressed {"
                   "background-color: rgb(45,106,230);"
                    "color: rgb(255, 255, 255);"
                    "border-radius: 4px;}"
             "QPushButton {"
                   "background-color: rgb(57, 136, 255);"
                   "color: rgb(255, 255, 255);"
                   "border-radius: 4px;}";

    ui->pushButtonInitialize->setStyleSheet(testingBtnQss);
    ui->pushButtonRun->setStyleSheet(testingBtnQss);
    ui->pushButtonOK->setStyleSheet(settingQss);
}

void MainWindow::loadSuperKey()
{
    QPair<std::string, std::string> keypair;
    keypair.first = super_private_key.toStdString();
    keypair.second = super_private_key.toStdString();
    AccountManager::instance().addAccounts(super_account, keypair);
}

void MainWindow::commonOutput(const QString &msg)
{
    ui->textEditCommonOut->append(msg);
}

void MainWindow::updateHandledTransationUI(qint64 trxs, qint64 duration)
{
    total_trxs += trxs;

    ui->textEditHandledTrxs->append(QString::number(trxs));
    ui->lineEditHandleTrxsCount->setText(QString::number(total_trxs));
    ui->lineEditTotalTime->setText(QString::number(duration));

    double tps = 0.0;
    if (duration) {
        tps = (total_trxs * 1.0) / duration * 1000;
    }
    ui->lineEditAverageTPS->setText(QString::number(tps, 'f', 1));
}

void MainWindow::updateMaxTPS(double tps)
{
    ui->lineEditMaxTPS->setText(QString::number(tps, 'f', 1));
}

void MainWindow::enablePacker(bool enable)
{
    if (enable) {
        auto accounts = AccountManager::instance().listAccounts();
        for (int i = 0; i < accounts.size(); ++i) {
            auto p = new Packer;
            connect(p, &Packer::finished, p, &QObject::deleteLater);    // auto delete
            connect(p, &Packer::newPackedTrx, trxpool, &TransactionPool::incomingPackedTrxs);

            p->setAccountName(QString::fromStdString(accounts.at(i)));
            p->setCallback([=] (const QString& msg) {
                commonOutput(msg);
            });
            p->start();

            packers.push_back(p);
        }
    } else {
        if (!packers.isEmpty()) {
            for (auto& p : packers) {
                p->stop();
            }

            packers.clear();
        }
    }
}

void MainWindow::enableResultCounter(bool enable)
{
    if (enable) {
        resultCounter = new ResultCounter;
        connect(resultCounter, &ResultCounter::finished, this, &MainWindow::resultCountFinished);
        connect(resultCounter, &ResultCounter::handledTrxsArrived, this, &MainWindow::updateHandledTransationUI, Qt::QueuedConnection);
        connect(resultCounter, &ResultCounter::maxTPS, this, &MainWindow::updateMaxTPS, Qt::QueuedConnection);
        resultCounter->start();
    } else {
        if (resultCounter) {
            resultCounter->stop();
        }
    }
}

void MainWindow::enableTrxpool(bool enable)
{
    if (enable) {
       if (!trxpool) {
           return;
       }

       trxpool->start();
    } else {
        if (trxpool) {
            trxpool->stop();
        }
    }
}

void MainWindow::on_pushButtonInitialize_clicked()
{
    ui->pushButtonInitialize->setEnabled(false);
    ui->pushButtonInitialize->setText("Preparing");
    commonOutput(QString("Start creating %1 accounts ...").arg(thread_num));

    CreateAccount createAccount;
    int count = createAccount.create(thread_num, [=](const QString& name, bool res) {
        commonOutput(QString("Create %1 %2.").arg(name).arg(res ? "succeed" : "failed"));
    });
    if (!count) {
        commonOutput("Create account failed, maybe accounts already exists, try again!");
        return;
    } else {
        if (count != thread_num) {
            QString msg = QString("Try to create ") + QString::number(thread_num) + QString(" accounts, But created ")
                    + QString::number(count) + QString(" accounts.");
            commonOutput(msg);
        }
    }

    commonOutput(QString("Create account done.\nNow start to transfer %1 %2 from %3 to accounts just created.").arg(total_tokens).arg(token_name).arg(super_account));

    auto accounts = AccountManager::instance().listAccounts();
    int accountSize = accounts.size();
    int balance = total_tokens / accountSize;
    for (int i = 0; i < accountSize; ++i) {
        PushManager push;
        auto quantity = QString("%1.0000 %2").arg(balance).arg(token_name);
        auto to       = QString::fromStdString(accounts.at(i));
        commonOutput(QString("Transfering %1 to %2 ...").arg(quantity).arg(to));
        bool ret = push.transferToken(super_account, to, quantity);
        commonOutput(ret ? "Succeed." : "Failed.");
    }

    trxpool = new TransactionPool;
    trxpool->setTargetSize(trx_size);
    connect(trxpool, &TransactionPool::finished, this, &MainWindow::packedTrxTransferFinished);
    connect(trxpool, &TransactionPool::packedTrxPoolFulfilled, this, &MainWindow::packedTrxReady);

    enablePacker(true);
}

void MainWindow::on_pushButtonRun_clicked()
{
    enableTrxpool(true);

    commonOutput("------------------------------------------");
    commonOutput("Start transfering packed trxs...");
    ui->pushButtonRun->setText("Transfering");
    ui->pushButtonRun->setEnabled(false);
}

void MainWindow::loadSettings()
{
    Config cfg;
    cfg.loadSetting();

    ui->lineEditHost->setText(base_url);
    ui->lineEditPort->setText(url_port);

    ui->lineEditSuperAccount->setText(super_account);
    ui->textEditKey->setText(super_private_key);

    ui->lineEditContractAccount->setText(contract_account);
    ui->lineEditTokenName->setText(token_name);
    ui->spinBoxTokenNum->setValue(total_tokens);

    ui->spinBoxThreadNum->setValue(thread_num);
    ui->spinBoxTransactionPoolSize->setValue(trx_size);
    ui->spinBoxBatchSize->setValue(batch_size);
}

void MainWindow::on_pushButtonOK_clicked()
{
    base_url = ui->lineEditHost->text();
    url_port = ui->lineEditPort->text();

    super_account = ui->lineEditSuperAccount->text();
    super_private_key = ui->textEditKey->toPlainText();

    contract_account = ui->lineEditContractAccount->text();
    token_name = ui->lineEditTokenName->text();
    total_tokens = ui->spinBoxTokenNum->value();

    thread_num = ui->spinBoxThreadNum->value();
    trx_size = ui->spinBoxTransactionPoolSize->value();
    batch_size = ui->spinBoxBatchSize->value();

    Config cfg;
    cfg.saveSettings();

    loadSuperKey();
}

void MainWindow::packedTrxReady()
{
    enablePacker(false);

    ui->pushButtonRun->setEnabled(true);
    ui->pushButtonInitialize->setText("Prepared");
    commonOutput("Prepare done, now you can start test!");
}

void MainWindow::packedTrxTransferFinished()
{
    if (trxpool) {
        trxpool->deleteLater();
    }

    enableResultCounter(true);

    ui->pushButtonRun->setEnabled(false);
    ui->pushButtonRun->setText("Counting Result");
    commonOutput("Transfer done, wait for counting result.");
    commonOutput("------------------------------------------");
}

void MainWindow::resultCountFinished()
{
   if (resultCounter) {
       resultCounter->deleteLater();
   }

   ui->pushButtonRun->setText("Finished");
   commonOutput("*****Test done!*****");
}
