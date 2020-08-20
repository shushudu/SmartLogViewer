#ifndef LOGVIEWWIDGET_H
#define LOGVIEWWIDGET_H

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QPlainTextEdit;
class QTextEdit;
class QPushButton;

class FileAppendedDataReader;
class UTF8StreamConverter;
class TextStreamLineBuilder;

class LogViewWidget : public QWidget
{
    Q_OBJECT

    FileAppendedDataReader * fadr;
    UTF8StreamConverter * utf8;
    TextStreamLineBuilder * tlb;

    QTimer * timer;
    QVBoxLayout * mainLayout;
    QHBoxLayout * hbuttons;

    QPlainTextEdit * teError;
    QPlainTextEdit * teWarn;
    QPlainTextEdit * teHighlight;
    QPlainTextEdit * teHide;

    QPlainTextEdit * teErrorExclude;
    QPlainTextEdit * teWarnExclude;
    QPlainTextEdit * teHighlightExclude;
    QPlainTextEdit * teHideExclude;

    QTextEdit * teLog;
    QPushButton * btnClear;
    QPushButton * btnReRead;
    QPushButton * btnStart;
    QPushButton * btnStop;
    QPushButton * btnExpand;
    QWidget * filters;

public:
    LogViewWidget(const QString & filename, QWidget* parent = 0);
    ~LogViewWidget();

public slots:
    void addNewLine(const QString & line);
    void setColorForLine(const QString & line);
    void expand();
    void clear();
    void reRead();
    void start();
    void stop();
};

#endif // LOGVIEWWIDGET_H
