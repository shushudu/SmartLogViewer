#include "fileappendeddatareader.h"

#include <QFile>

FileAppendedDataReader::FileAppendedDataReader(const QString & filename):
    fn(filename)
{
}

void FileAppendedDataReader::check()
{
    QFile file(fn);
    if(!file.open(QFile::ReadOnly | QIODevice::ExistingOnly))
    {
        Q_ASSERT(0);
        return;
    }

    file.seek(position);

    while (!file.atEnd())
    {
        char c = 0;
        qint64 readed = file.read(&c, 1);
        Q_ASSERT(readed >= 0);

        position += readed;

        if (readed == 1)
        {
            emit newData(c);
        }
        else
            Q_ASSERT(0);
    }

}

void FileAppendedDataReader::toEnd()
{
    QFile file(fn);

    position = file.size();
}
