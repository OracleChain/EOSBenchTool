#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "work/packer.h"
#include "work/resultcounter.h"
#include "work/transactionpool.h"

namespace Ui {
class MainWindow;
}

class AboutFrame;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    void updateAbout();

private:
    void initButtonStyle();
    void loadSettings();
    void loadSuperKey();
    void commonOutput(const QString& msg);

    void enablePacker(bool enable);
    void enableResultCounter(bool enable);
    void enableTrxpool(bool enable);
    void enableTransferCount(bool enable);

private slots:
    void on_pushButtonInitialize_clicked();
    void on_pushButtonRun_clicked();
    void on_pushButtonOK_clicked();

    void updateHandledTransationUI(qint64 trxs, qint64 duration);
    void updateMaxTPS(double tps);

    void packedTrxReady();
    void packedTrxTransferFinished();
    void resultCountFinished();

private:
    Ui::MainWindow *ui;
    AboutFrame *about;

    QVector<Packer*> packers;
    ResultCounter *resultCounter;
    TransactionPool *trxpool;

    qint64 total_trxs;

    const static int MAX_ROW = 10000;
};

#endif // MAINWINDOW_H
