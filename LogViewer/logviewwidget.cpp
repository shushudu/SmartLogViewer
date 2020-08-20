#include "logviewwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QTimer>
#include <QPushButton>
#include <QSettings>

#include "textstreamlinebuilder.h"
#include "utf8streamconverter.h"
#include "fileappendeddatareader.h"
#include "mainwindowwidget.h"

LogViewWidget::LogViewWidget(const QString & filename, QWidget* parent) : QWidget(parent)
{
    setObjectName(MainWindowWidget::getTabCaptionFromFilename(filename));

    fadr = new FileAppendedDataReader(filename);
    utf8 = new UTF8StreamConverter();
    tlb = new TextStreamLineBuilder();

    mainLayout = new QVBoxLayout(this);
    teLog = new QTextEdit();

    teLog->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    teLog->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    teLog->setReadOnly(true);
    teLog->setWordWrapMode(QTextOption::NoWrap);

    QFont teFont("Courier New", 10, 0, false);
    teLog->setFont(teFont);

    teError = new QPlainTextEdit();
    teWarn = new QPlainTextEdit();
    teHighlight = new QPlainTextEdit();
    teHide = new QPlainTextEdit();

    teErrorExclude = new QPlainTextEdit();
    teWarnExclude = new QPlainTextEdit();
    teHighlightExclude = new QPlainTextEdit();
    teHideExclude = new QPlainTextEdit();

    QVBoxLayout * filterLayout = new QVBoxLayout();

    QHBoxLayout * filterLayoutTop = new QHBoxLayout();
    QHBoxLayout * filterLayoutBottom = new QHBoxLayout();

    filterLayoutTop->addWidget(teError);
    filterLayoutTop->addWidget(teWarn);
    filterLayoutTop->addWidget(teHighlight);
    filterLayoutTop->addWidget(teHide);

    filterLayoutBottom->addWidget(teErrorExclude);
    filterLayoutBottom->addWidget(teWarnExclude);
    filterLayoutBottom->addWidget(teHighlightExclude);
    filterLayoutBottom->addWidget(teHideExclude);

    filterLayout->addLayout(filterLayoutTop);
    filterLayout->addLayout(filterLayoutBottom);

    hbuttons = new QHBoxLayout();

    btnClear = new QPushButton("Clear");
    hbuttons->addWidget(btnClear);
    QObject::connect(btnClear, &QPushButton::clicked,
                         this, &LogViewWidget::clear);

    btnReRead = new QPushButton("ReRead");
    hbuttons->addWidget(btnReRead);
    QObject::connect(btnReRead, &QPushButton::clicked,
                         this, &LogViewWidget::reRead);

    btnStart = new QPushButton("Start");
    hbuttons->addWidget(btnStart);
    QObject::connect(btnStart, &QPushButton::clicked,
                         this, &LogViewWidget::start);

    btnStop = new QPushButton("Stop");
    hbuttons->addWidget(btnStop);
    QObject::connect(btnStop, &QPushButton::clicked,
                         this, &LogViewWidget::stop);

    btnExpand = new QPushButton("Show/Hide filters");
    hbuttons->addWidget(btnExpand);
    QObject::connect(btnExpand, &QPushButton::clicked,
                         this, &LogViewWidget::expand);


    filters = new QWidget();
    filters->setLayout(filterLayout);
    filters->setVisible(false);


    mainLayout->addLayout(hbuttons, 1);
    mainLayout->addWidget(teLog, 3);
    mainLayout->addWidget(filters, 7);

    fadr->toEnd();

    QObject::connect(fadr, &FileAppendedDataReader::newData,
                         utf8, &UTF8StreamConverter::addNewByte);

    QObject::connect(utf8, &UTF8StreamConverter::newChar,
                         tlb, &TextStreamLineBuilder::addSymbol);

    QObject::connect(tlb, &TextStreamLineBuilder::newLine,
                         this, &LogViewWidget::addNewLine);

    timer = new QTimer(this);

    QObject::connect(timer, &QTimer::timeout, fadr, &FileAppendedDataReader::check);
    stop();

    QSettings settings;

    QString str = settings.value(QString("ErrorFilter_%1").arg(objectName())).toString();
    teError->setPlainText(str);

    str = settings.value(QString("WarnFilter_%1").arg(objectName())).toString();
    teWarn->setPlainText(str);

    str = settings.value(QString("HighlightFilter_%1").arg(objectName())).toString();
    teHighlight->setPlainText(str);

    str = settings.value(QString("HideFilter_%1").arg(objectName())).toString();
    teHide->setPlainText(str);


    str = settings.value(QString("ErrorExcludeFilter_%1").arg(objectName())).toString();
    teErrorExclude->setPlainText(str);

    str = settings.value(QString("WarnExcludeFilter_%1").arg(objectName())).toString();
    teWarnExclude->setPlainText(str);

    str = settings.value(QString("HighlightExcludeFilter_%1").arg(objectName())).toString();
    teHighlightExclude->setPlainText(str);

    str = settings.value(QString("HideExcludeFilter_%1").arg(objectName())).toString();
    teHideExclude->setPlainText(str);
}

LogViewWidget::~LogViewWidget()
{
    QSettings settings;
    settings.setValue(QString("ErrorFilter_%1").arg(objectName()), teError->toPlainText());
    settings.setValue(QString("WarnFilter_%1").arg(objectName()), teWarn->toPlainText());
    settings.setValue(QString("HighlightFilter_%1").arg(objectName()), teHighlight->toPlainText());
    settings.setValue(QString("HideFilter_%1").arg(objectName()), teHide->toPlainText());
    settings.setValue(QString("ErrorExcludeFilter_%1").arg(objectName()), teErrorExclude->toPlainText());
    settings.setValue(QString("WarnExcludeFilter_%1").arg(objectName()), teWarnExclude->toPlainText());
    settings.setValue(QString("HighExcludelightFilter_%1").arg(objectName()), teHighlightExclude->toPlainText());
    settings.setValue(QString("HideExcludeFilter_%1").arg(objectName()), teHideExclude->toPlainText());
}

bool ColorFromTextEditRegexp(QPlainTextEdit * te, const QString & line)
{
    QString regexp_txt = te->toPlainText();

    if (regexp_txt.length()>0)
    {
        QStringList regexps = regexp_txt.split('\n');

        for (const auto & str : regexps)
        {
            QRegExp rx(str);

            int ret = rx.indexIn(line);
            if (-1 != ret)
            {
                return true;
            }
        }

    }

    return false;
}

void LogViewWidget::expand()
{
    filters->setVisible(!filters->isVisible());
}

void LogViewWidget::clear()
{
    teLog->setText("");
    fadr->toEnd();
    start();
}

void LogViewWidget::reRead()
{
    teLog->setText("");
    fadr->toBegin();
    start();
}

void LogViewWidget::start()
{
    timer->start(200);
    btnStop->setEnabled(true);
    btnStart->setEnabled(false);
}

void LogViewWidget::stop()
{
    timer->stop();
    btnStop->setEnabled(false);
    btnStart->setEnabled(true);
}

void LogViewWidget::setColorForLine(const QString & line)
{
    if (!ColorFromTextEditRegexp(teErrorExclude, line) && ColorFromTextEditRegexp(teError, line))
    {
        teLog->setTextColor(QColor::fromRgb(255, 255, 255));
        teLog->setTextBackgroundColor(QColor::fromRgb(255, 0, 0));
        teLog->setFontWeight(2);
        return;
    }

    if (!ColorFromTextEditRegexp(teWarnExclude, line) && ColorFromTextEditRegexp(teWarn, line))
    {
        teLog->setTextColor(QColor::fromRgb(0, 0, 0));
        teLog->setTextBackgroundColor(QColor::fromRgb(255, 255, 0));
        teLog->setFontWeight(0);
        return;
    }

    if (!ColorFromTextEditRegexp(teHighlightExclude, line) && ColorFromTextEditRegexp(teHighlight, line))
    {
        teLog->setTextColor(QColor::fromRgb(0, 0, 0));
        teLog->setTextBackgroundColor(QColor::fromRgb(0, 255, 0));
        teLog->setFontWeight(0);
        return;
    }

    if (!ColorFromTextEditRegexp(teHideExclude, line) && ColorFromTextEditRegexp(teHide, line))
    {
        teLog->setTextColor(QColor::fromRgb(192, 192, 192));
        teLog->setTextBackgroundColor(QColor::fromRgb(255, 255, 255));
        teLog->setFontWeight(0);
        return;
    }

    teLog->setTextColor(QColor::fromRgb(0, 0, 0));
    teLog->setTextBackgroundColor(QColor::fromRgb(255, 255, 255));
    teLog->setFontWeight(0);
}

void LogViewWidget::addNewLine(const QString & line)
{
    QTextCursor cursor = teLog->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    teLog->setTextCursor(cursor);

    setColorForLine(line);

    teLog->append(line);

    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    teLog->setTextCursor(cursor);

}
