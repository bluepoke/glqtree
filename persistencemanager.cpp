#include "persistencemanager.h"
#include <QtXml/QXmlStreamReader>
#include <QFile>
#include <QDebug>


void PersistenceManager::readPlant(QString fileName) {
    QFile file (fileName);
    qDebug()<<fileName;
    if (file.open(QFile::ReadOnly)) {
        QXmlStreamReader reader(&file);
        while (!reader.atEnd() && !reader.hasError()) {
            // read
            QXmlStreamReader::TokenType token = reader.readNext();
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }
            if(token == QXmlStreamReader::StartElement) {
                qDebug()<<reader.name().toString();
                if (reader.name().toString()=="plant") {
                    qDebug() << reader.attributes().size();
                }
            }
        }
        if (reader.hasError()) {
            qDebug()<<reader.errorString();
        }
    } else {
        qDebug() << "failed to open file " << fileName;
    }

    file.close();
}


// test main for xml reading
int main(int argc, char *argv[])
{
    PersistenceManager::readPlant(QString ("D:\\default.xml"));
}
