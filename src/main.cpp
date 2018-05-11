#include "mainwindow.h"
#include <QApplication>
#include <time.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    srand((unsigned)time(0));

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
