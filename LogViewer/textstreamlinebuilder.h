#ifndef TEXTSTREAMLINEBUILDER_H
#define TEXTSTREAMLINEBUILDER_H

#include <QObject>

class TextStreamLineBuilder: public QObject
{
    Q_OBJECT

    QString current_line = "";

signals:
    void newLine(const QString & str);

public slots:
    void addSymbol(const QChar & c);
};

#endif // TEXTSTREAMLINEBUILDER_H
