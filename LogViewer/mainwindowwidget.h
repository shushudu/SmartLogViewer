#ifndef MAINWINDOWWIDGET_H
#define MAINWINDOWWIDGET_H

#include <QTabWidget>

class MainWindowWidget : public QTabWidget
{
    Q_OBJECT

public:
    static QString findLastModifiedFile(const QString & path, const QString & mask);
    static QString getTabCaptionFromFilename(const QString & fn);

public:
    MainWindowWidget(QWidget* parent = 0);

    void openFile(const QString & path, const QString & mask);
};

#endif // MAINWINDOWWIDGET_H
