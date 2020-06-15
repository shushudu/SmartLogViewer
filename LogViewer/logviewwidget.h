#ifndef LOGVIEWWIDGET_H
#define LOGVIEWWIDGET_H

#include <QWidget>

class QVBoxLayout;
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
    QPlainTextEdit * teError;
    QPlainTextEdit * teWarn;
    QPlainTextEdit * teHighlight;
    QPlainTextEdit * teHide;
    QTextEdit * teLog;
    QPushButton * btnReRead;
    QPushButton * btnStart;
    QPushButton * btnStop;

public:
    LogViewWidget(const QString & filename, QWidget* parent = 0);
    ~LogViewWidget();

public slots:
    void addNewLine(const QString & line);
    void setColorForLine(const QString & line);
    void reRead();
    void start();
    void stop();
};

#endif // LOGVIEWWIDGET_H
