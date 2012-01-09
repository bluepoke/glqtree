#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H
#include <QtXml/QDomDocument>

class PersistenceManager
{
public:
    PersistenceManager();
    static void readPlant(QString fileName);
};

#endif // PERSISTENCEMANAGER_H
