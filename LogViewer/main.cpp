#include "logviewwidget.h"

#include <QApplication>
#include <QTabWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTabWidget w;

    LogViewWidget lv1 ("D:\\1.log");
    w.addTab(&lv1, "log");

    w.show();

    return a.exec();
}
