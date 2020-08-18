#ifndef MAINWINDOWWIDGET_H
#define MAINWINDOWWIDGET_H

#include <QWidget>

class QTabWidget;
class QPushButton;

class MainWindowWidget : public QWidget
{
    Q_OBJECT

public:
    static QString findLastModifiedFile(const QString & path, const QString & mask);
    static QString getTabCaptionFromFilename(const QString & fn);

public:
    MainWindowWidget(QWidget* parent = 0);

    void openFile(const QString & path, const QString & mask);

private:
    QTabWidget *tabs = nullptr;
    QPushButton * butClearAll = nullptr;
};

#endif // MAINWINDOWWIDGET_H
