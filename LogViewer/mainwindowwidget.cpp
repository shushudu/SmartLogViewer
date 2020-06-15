#include "mainwindowwidget.h"

#include <QDir>

#include "logviewwidget.h"

MainWindowWidget::MainWindowWidget(QWidget* parent): QTabWidget(parent)
{

}

void MainWindowWidget::openFile(const QString & path, const QString & mask)
{
    LogViewWidget * lv = new LogViewWidget(findLastModifiedFile(path, mask));
    addTab(lv, getTabCaptionFromFilename(findLastModifiedFile(path, mask)));
}

QString MainWindowWidget::findLastModifiedFile(const QString & path, const QString & mask)
{
    QDir mDir(path);

    QFileInfoList fiList = mDir.entryInfoList(QStringList(mask), QDir::Files, QDir::Time);

    if (fiList.count() == 0)
        return "";

    QString ret = fiList[0].absoluteFilePath();

    return ret;
}

QString MainWindowWidget::getTabCaptionFromFilename(const QString & fn)
{
    QString ret = QFileInfo(fn).baseName();

    ret.remove(QRegExp("[^A-Za-z]"));

    return ret;
}
