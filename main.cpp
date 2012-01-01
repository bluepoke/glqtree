#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // done by the QTCreator
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
