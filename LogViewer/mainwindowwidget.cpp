#include "mainwindowwidget.h"

#include <QDir>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>

#include "logviewwidget.h"

MainWindowWidget::MainWindowWidget(QWidget* parent): QWidget(parent)
{

    butClearAll = new QPushButton(this);
    butClearAll->setText("Clear all");

    tabs = new QTabWidget(this);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(butClearAll);
    mainLayout->addWidget(tabs);
    this->setLayout(mainLayout);
}

void MainWindowWidget::openFile(const QString & path, const QString & mask)
{
    LogViewWidget * lv = new LogViewWidget(findLastModifiedFile(path, mask));
    tabs->addTab(lv, getTabCaptionFromFilename(findLastModifiedFile(path, mask)));

    QObject::connect(butClearAll, &QPushButton::clicked, lv, &LogViewWidget::clear);
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
