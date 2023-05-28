#include "mainwindow.h"

#include <QApplication>
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(res);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
