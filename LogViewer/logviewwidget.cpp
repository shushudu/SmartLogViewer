#include "logviewwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QTimer>

#include "textstreamlinebuilder.h"
#include "utf8streamconverter.h"
#include "fileappendeddatareader.h"

LogViewWidget::LogViewWidget(const QString & filename, QWidget* parent) : QWidget(parent)
{
    fadr = new FileAppendedDataReader(filename);
    utf8 = new UTF8StreamConverter();
    tlb = new TextStreamLineBuilder();


    mainLayout = new QVBoxLayout(this);
    teLog = new QTextEdit();

    teLog->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    teLog->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    teLog->setReadOnly(true);
    teLog->setWordWrapMode(QTextOption::NoWrap);

    teError = new QPlainTextEdit();
    teWarn = new QPlainTextEdit();
    teHighlight = new QPlainTextEdit();
    teHide = new QPlainTextEdit();

    QHBoxLayout *hb = new QHBoxLayout();

    hb->addWidget(teError);
    hb->addWidget(teWarn);
    hb->addWidget(teHighlight);
    hb->addWidget(teHide);

    mainLayout->addWidget(teLog, 5);
    mainLayout->addLayout(hb, 1);

    fadr->toEnd();

    QObject::connect(fadr, &FileAppendedDataReader::newData,
                         utf8, &UTF8StreamConverter::addNewByte);

    QObject::connect(utf8, &UTF8StreamConverter::newChar,
                         tlb, &TextStreamLineBuilder::addSymbol);

    QObject::connect(tlb, &TextStreamLineBuilder::newLine,
                         this, &LogViewWidget::addNewLine);

    timer = new QTimer(this);

    QObject::connect(timer, &QTimer::timeout, fadr, &FileAppendedDataReader::check);
    timer->start(200);
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

void LogViewWidget::setColorForLine(const QString & line)
{
    if (ColorFromTextEditRegexp(teError, line))
    {
        teLog->setTextColor(QColor::fromRgb(255, 255, 255));
        teLog->setTextBackgroundColor(QColor::fromRgb(255, 0, 0));
        return;
    }

    if (ColorFromTextEditRegexp(teWarn, line))
    {
        teLog->setTextColor(QColor::fromRgb(0, 0, 0));
        teLog->setTextBackgroundColor(QColor::fromRgb(255, 255, 0));
        return;
    }

    if (ColorFromTextEditRegexp(teHighlight, line))
    {
        teLog->setTextColor(QColor::fromRgb(0, 0, 0));
        teLog->setTextBackgroundColor(QColor::fromRgb(0, 255, 0));
        return;
    }

    if (ColorFromTextEditRegexp(teHide, line))
    {
        teLog->setTextColor(QColor::fromRgb(192, 192, 192));
        teLog->setTextBackgroundColor(QColor::fromRgb(255, 255, 255));
        return;
    }

    teLog->setTextColor(QColor::fromRgb(0, 0, 0));
    teLog->setTextBackgroundColor(QColor::fromRgb(255, 255, 255));
}

void LogViewWidget::addNewLine(const QString & line)
{
    setColorForLine(line);

    teLog->append(line);
}
