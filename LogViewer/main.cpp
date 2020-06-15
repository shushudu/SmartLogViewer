#include <QApplication>

#include "mainwindowwidget.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("shushudu");
    QCoreApplication::setApplicationName("LogViewer");

    QApplication a(argc, argv);

    MainWindowWidget w;

    w.show();

    return a.exec();
}
