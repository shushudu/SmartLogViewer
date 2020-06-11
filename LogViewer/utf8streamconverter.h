#ifndef UTF8STREAMCONVERTER_H
#define UTF8STREAMCONVERTER_H

#include <QObject>

class UTF8StreamConverter: public QObject
{
    Q_OBJECT

    char c1 = 0;
    char c2 = 0;
    char c3 = 0;
    char c4 = 0;
    int cur_octet = 0;
    int total_octet_count = 0;

signals:
    void newChar(const QChar & c);

public slots:
    void addNewByte(const char c);
};

#endif // UTF8STREAMCONVERTER_H
