#ifndef FILEAPPENDEDDATAREADER_H
#define FILEAPPENDEDDATAREADER_H

#include <QObject>

class FileAppendedDataReader: public QObject
{
    Q_OBJECT

    qint64 position = 0;
    const QString fn;

public:
    FileAppendedDataReader(const QString & filename);

signals:
    void newData(char c);

public slots:
    void check();
    void toEnd();
    void toBegin();
};

#endif // FILEAPPENDEDDATAREADER_H
