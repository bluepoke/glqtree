#include "persistencemanager.h"
#include <QtXml/QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include "plant.h"

static const QString P_TAG = "PLANT";
static const QString THICK_TAG = "THICKNESS";
static const QString LEN_TAG = "LENGTH";
static const QString B_TAG = "BRANCHING";
static const QString B_ANGLE_TAG = "BRANCHING_ANGLE";
static const QString B_ROT_TAG = "BRANCHING_ROTATION";
static const QString GRAV_TAG = "GRAVITATIONAL_INFLUENCE";
static const QString GROWTH_INTERRUT_TAG = "GROWTH_INTERRUPTION";
static const QString WOBBLE_TAG = "WOBBLINESS";
static const QString L_LEVELS_TAG = "LEAF_LEVELS";
static const QString L_COUNT_P_LEVEL_TAG = "LEAF_COUNT_PER_LEVEL";
static const QString L_ANGLE_TAG = "LEAF_ANGLE";
static const QString L_LEN_TAG = "LEAF_LENGTH";
static const QString L_WIDTH_TAG = "LEAF_WIDTH";
static const QString MIN_TAG = "MIN";
static const QString MAX_TAG = "MAX";
static const QString V_TAG = "VALUE";
static const QString AGE_TAG = "AGE";
static const QString PROB_TAG = "PROBABILITY";

static const QString NAME_ATTRIB = "NAME";
static const QString MAX_AGE_ATTRIB = "MAX_AGE";
static const QString SEED_ATTRIB = "RANDOM_SEED";

Plant PersistenceManager::readPlant(QString fileName) {
    QFile file (fileName);
    Plant p(0,0,0);
    int pMaxAge = 0;
    if (file.open(QFile::ReadOnly)) {
        QXmlStreamReader reader(&file);
        while (!reader.atEnd() && !reader.hasError()) {
            // read
            QXmlStreamReader::TokenType token = reader.readNext();
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }
            if(token == QXmlStreamReader::StartElement) {
                if (reader.name().toString().compare(P_TAG,Qt::CaseInsensitive)==0) {
                    QXmlStreamAttributes attributes = reader.attributes();
                    QXmlStreamAttributes::iterator i;
                    for (i = attributes.begin(); i!=attributes.end(); ++i) {
                        QXmlStreamAttribute attrib = *i;
                        if (attrib.name().toString().compare(NAME_ATTRIB,Qt::CaseInsensitive)==0) {
                            p.name=attrib.value().toString();
                        } else if (attrib.name().toString().compare(MAX_AGE_ATTRIB,Qt::CaseInsensitive)==0) {
                            p.maxAge=attrib.value().toString().toInt();
                            pMaxAge = attrib.value().toString().toInt();
                        } else if (attrib.name().toString().compare(SEED_ATTRIB,Qt::CaseInsensitive)==0) {
                            p.seed = attrib.value().toString().toInt();
                        }
                    }
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
    return p;
}


// test main for xml reading
//int main(int argc, char *argv[])
//{
//    PersistenceManager::readPlant(QString ("D:\\default.xml"));
//}
