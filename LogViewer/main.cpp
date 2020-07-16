/* TODO:
 * - 20200716 Добавить кнопку "очистить все"
 *
 * Ошибки:
 * - 20200716 не выделяется по фильтру, если запись в лог файле занимает несколько строк
 * - 20200716 считать записью строки, начинающиеся по маске, так как одна запись может занимать несколько строк
 */


#include <QApplication>
#include <QtXml>
#include <QMessageBox>

#include "mainwindowwidget.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("shushudu");
    QCoreApplication::setApplicationName("LogViewer");

    QApplication a(argc, argv);

    MainWindowWidget w;

    if (argc != 2)
    {
        QMessageBox::warning(nullptr, QCoreApplication::applicationName(), "В параметрах запуска необходимо указать файл настроек");
        return -1;
    }

    QString config_filename = argv[1];

    QFile* file = new QFile(config_filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, QCoreApplication::applicationName(), "Ошибка открытия файла настроек " + config_filename);
        return -1;
    }

    QXmlStreamReader xml(file);

    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name() == "files")
                continue;
            if (xml.name() == "file")
            {
                QString path;
                QString mask;

                QXmlStreamAttributes attributes = xml.attributes();
                if (attributes.hasAttribute("path"))
                    path = attributes.value("path").toString();
                if (attributes.hasAttribute("mask"))
                    mask = attributes.value("mask").toString();

                w.openFile(path, mask);
            }
        }
    }

    w.show();

    return a.exec();
}
