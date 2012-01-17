#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H
#include <QtXml/QDomDocument>
#include "plant.h"

class PersistenceManager
{
public:
    PersistenceManager();
    static Plant *readPlant(QString fileName);
    static bool writePlant(QString fileName, const Plant *p);
};

#endif // PERSISTENCEMANAGER_H
