#include "persistencemanager.h"
#include <QtXml/QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include "plant.h"

// XML tag names
static const QString P_TAG = "PLANT";
static const QString THICK_TAG = "THICKNESS";
static const QString LEN_TAG = "LENGTH";
static const QString B_TAG = "BRANCHING";
static const QString B_ANGLE_TAG = "BRANCHING_ANGLE";
static const QString B_ROT_TAG = "BRANCHING_ROTATION";
static const QString MAIN_B_TAG = "MAIN_BRANCH";
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
static const QString TUPEL_TAG = "TUPEL";
static const QString V_TAG = "VALUE";
static const QString AGE_TAG = "AGE";
static const QString PROB_TAG = "PROBABILITY";
static const QString REL_DEV_TAG = "RELATIVE_DEVIATION";

// XML <PLANT> attribute names
static const QString NAME_ATTRIB = "NAME";
static const QString MAX_AGE_ATTRIB = "MAX_AGE";
static const QString SEED_ATTRIB = "RANDOM_SEED";

// some fields to make the source code more readable
static const Qt::CaseSensitivity CI = Qt::CaseInsensitive;
static const QXmlStreamReader::TokenType E_START = QXmlStreamReader::StartElement;
static const QXmlStreamReader::TokenType E_END = QXmlStreamReader::EndElement;

// reads the data from the xml file fileName and returns a pointer
// to a new Plant containing the data
Plant* PersistenceManager::readPlant(QString fileName) {
    QFile file (fileName);
    Plant *p = new Plant(0,0,0);
    int pMaxAge = 0;
    if (file.open(QFile::ReadOnly)) {
        QXmlStreamReader reader(&file);
        while (!reader.atEnd() && !reader.hasError()) {
            // start reading
            QXmlStreamReader::TokenType token = reader.readNext();
            // skip document start
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }            
            if(token == E_START) {
                // <PLANT>
                if (reader.name().toString().compare(P_TAG,CI)==0) {
                    QXmlStreamAttributes attributes = reader.attributes();
                    QXmlStreamAttributes::iterator i;
                    // iterate over plant's attributes
                    for (i = attributes.begin(); i!=attributes.end(); ++i) {
                        QXmlStreamAttribute attrib = *i;
                        // set plant's name
                        if (attrib.name().toString().compare(NAME_ATTRIB,CI)==0) {
                            p->name=attrib.value().toString();
                        }
                        // set plant's maximum age
                        else if (attrib.name().toString().compare(MAX_AGE_ATTRIB,CI)==0) {
                            p->maxAge=attrib.value().toString().toInt();
                            pMaxAge = attrib.value().toString().toInt();
                        }
                        // set plant's random seed
                        else if (attrib.name().toString().compare(SEED_ATTRIB,CI)==0) {
                            p->seed = attrib.value().toString().toInt();
                        }
                    }
                    // gather all the rest of the data
                    token = reader.readNext();
                    while(!(token==E_END && reader.name().compare(P_TAG,CI)==0)) {
                        // <SOMETHING>
                        QString tagName = reader.name().toString();
                        int minValue = 0;
                        double minProb = 0;
                        int maxValue = 0;
                        double maxProb = 0;
                        QList<int> tupelAges;
                        QList<int> tupelValues;
                        QList<double> tupelProbs;
                        tupelAges.clear();
                        tupelValues.clear();
                        tupelProbs.clear();
                        if (token == E_START) {
                            // look for data until we find closing tag for this
                            token = reader.readNext(); // next element inside <PLANT>
                            while(!(token==E_END && reader.name()==tagName)) {
                                // <SOMETHING><MIN>
                                if (token == E_START && reader.name().compare(MIN_TAG,CI)==0) {
                                    token = reader.readNext();
                                    while(!(token==E_END && reader.name().compare(MIN_TAG,CI)==0)) {
                                        // <SOMETHING><MIN><VALUE>
                                        if (token == E_START && reader.name().compare(V_TAG,CI)==0) {
                                            minValue = reader.readElementText().toInt();
                                        }
                                        // <SOMETHING><MIN><PROBABILITY> or <SOMETHING><MIN><RELATIVE_DEVIATION>
                                        else if (token == E_START && (reader.name().compare(PROB_TAG,CI)==0 || reader.name().compare(REL_DEV_TAG,CI)==0)){
                                            minProb = reader.readElementText().toDouble();
                                        }
                                        token = reader.readNext();
                                    }
                                }
                                // <SOMETHING><MAX>
                                else if (token == E_START && reader.name().compare(MAX_TAG,CI)==0) {
                                    token = reader.readNext();
                                    while(!(token==E_END && reader.name().compare(MAX_TAG,CI)==0)) {
                                        // <SOMETHING><MAX><VALUE>
                                        if (token == E_START && reader.name().compare(V_TAG,CI)==0) {
                                            maxValue = reader.readElementText().toInt();
                                        }
                                        // <SOMETHING><MAX><PROBABILITY> or <SOMETHING><MAX><RELATIVE_DEVIATION>
                                        else if (token == E_START && (reader.name().compare(PROB_TAG,CI)==0 || reader.name().compare(REL_DEV_TAG,CI)==0)){
                                            maxProb = reader.readElementText().toDouble();
                                        }
                                        token = reader.readNext();
                                    }
                                }
                                // <SOMETHING><TUPEL>
                                else if (token == E_START && reader.name().compare(TUPEL_TAG,CI)==0) {
                                    token = reader.readNext();
                                    while(!(token==E_END && reader.name().compare(TUPEL_TAG,CI)==0)) {
                                        // <SOMETHING><TUPEL><VALUE>
                                        if (token == E_START && reader.name().compare(V_TAG,CI)==0) {
                                            tupelValues.append(reader.readElementText().toInt());
                                        }
                                        // <SOMETHING><TUPEL><PROBABILITY> or <SOMETHING><TUPEL><RELATIVE_DEVIATION>
                                        else if (token == E_START && (reader.name().compare(PROB_TAG,CI)==0 || reader.name().compare(REL_DEV_TAG,CI)==0)){
                                            tupelProbs.append(reader.readElementText().toDouble());
                                        }
                                        // <SOMETHING><TUPEL><AGE>
                                        else if (token == E_START && reader.name().compare(AGE_TAG,CI)==0){
                                            tupelAges.append(reader.readElementText().toInt());
                                        }
                                        token = reader.readNext(); // Next element inside <SOMETHING><TUPEL>
                                    }
                                    // in case no value or no probability is defined in tupel
                                    // just add 0 to the list
                                    if (tupelAges.size()>tupelProbs.size()) {
                                        tupelProbs.append(0);
                                    }
                                    if (tupelAges.size()>tupelValues.size()) {
                                        tupelValues.append(0);
                                    }
                                }
                                token = reader.readNext(); // Next element inside <SOMETHING>
                            }
                            // add collected values to the list
                            // <THICKNESS>
                            if (tagName.compare(THICK_TAG,CI)==0) {
                                p->addBranchThickness(0,minValue,minProb);
                                p->addBranchThickness(p->maxAge,maxValue,maxProb);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addBranchThickness(tupelAges.at(i),tupelValues.at(i),tupelProbs.at(i));
                                }
                            }
                            // <LENGTH>
                            else if (tagName.compare(LEN_TAG,CI)==0) {
                                p->addBranchLength(0,minValue,minProb);
                                p->addBranchLength(p->maxAge,maxValue,maxProb);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addBranchLength(tupelAges.at(i),tupelValues.at(i),tupelProbs.at(i));
                                }
                            }
                            // <BRANCHING>
                            else if (tagName.compare(B_TAG,CI)==0) {
                                p->addBranching(0,minValue,minProb);
                                p->addBranching(p->maxAge,maxValue,maxProb);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addBranching(tupelAges.at(i),tupelValues.at(i),tupelProbs.at(i));
                                }
                            }
                            // <BRANCHING_ANGLE>
                            else if (tagName.compare(B_ANGLE_TAG,CI)==0) {
                                p->addBranchingAngle(0,minValue,minProb);
                                p->addBranchingAngle(p->maxAge,maxValue,maxProb);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addBranchingAngle(tupelAges.at(i),tupelValues.at(i),tupelProbs.at(i));
                                }
                            }
                            // <BRANCHING_ROTATION>
                            else if (tagName.compare(B_ROT_TAG,CI)==0) {
                                p->addBranchingRotation(0,minValue,minProb);
                                p->addBranchingRotation(p->maxAge,maxValue,maxProb);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addBranchingRotation(tupelAges.at(i),tupelValues.at(i),tupelProbs.at(i));
                                }
                            }

                            // <MAIN_BRANCH>
                            else if (tagName.compare(MAIN_B_TAG,CI)==0) {
                                p->addMainBranch(0,minProb);
                                p->addMainBranch(p->maxAge,maxProb);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addMainBranch(tupelAges.at(i),tupelProbs.at(i));
                                }
                            }

                            // <GRAVITATIONAL_INFLUENCE>
                            else if (tagName.compare(GRAV_TAG,CI)==0) {
                                p->addGravitationalInfluence(0,minValue);
                                p->addGravitationalInfluence(p->maxAge,maxValue);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addGravitationalInfluence(tupelAges.at(i),tupelValues.at(i));
                                }
                            }
                            // <GROWTH_INTERRUPTION>
                            else if (tagName.compare(GROWTH_INTERRUT_TAG,CI)==0) {
                                p->addGrowthInterruption(0,minValue,minProb);
                                p->addGrowthInterruption(p->maxAge,maxValue,maxProb);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addGrowthInterruption(tupelAges.at(i),tupelValues.at(i),tupelProbs.at(i));
                                }
                            }
                            // <WOBBLINESS>
                            else if (tagName.compare(WOBBLE_TAG,CI)==0) {
                                p->addBranchWobbliness(0,minValue,minProb);
                                p->addBranchWobbliness(p->maxAge,maxValue,maxProb);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addBranchWobbliness(tupelAges.at(i),tupelValues.at(i),tupelProbs.at(i));
                                }
                            }
                            // <LEAF_LEVELS>
                            else if (tagName.compare(L_LEVELS_TAG,CI)==0) {
                                p->addLeafLevels(0,minValue);
                                p->addLeafLevels(p->maxAge,maxValue);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addLeafLevels(tupelAges.at(i),tupelValues.at(i));
                                }
                            }
                            // <LEAF_COUNT_PER_LEVEL>
                            else if (tagName.compare(L_COUNT_P_LEVEL_TAG,CI)==0) {
                                p->addLeafCountPerLevel(0,minValue);
                                p->addLeafCountPerLevel(p->maxAge,maxValue);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addLeafCountPerLevel(tupelAges.at(i),tupelValues.at(i));
                                }
                            }
                            // <LEAF_ANGLE>
                            else if (tagName.compare(L_ANGLE_TAG,CI)==0) {
                                p->addLeafAngle(0,minValue);
                                p->addLeafAngle(p->maxAge,maxValue);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addLeafAngle(tupelAges.at(i),tupelValues.at(i));
                                }
                            }
                            // <LEAF_LENGTH>
                            else if (tagName.compare(L_LEN_TAG,CI)==0) {
                                p->addLeafLength(0,minValue);
                                p->addLeafLength(p->maxAge,maxValue);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addLeafLength(tupelAges.at(i),tupelValues.at(i));
                                }
                            }
                            // <LEAF_WIDTH>
                            else if (tagName.compare(L_WIDTH_TAG,CI)==0) {
                                p->addLeafWidth(0,minValue);
                                p->addLeafWidth(p->maxAge,maxValue);
                                for (int i=0; i<tupelAges.size(); i++) {
                                    p->addLeafWidth(tupelAges.at(i),tupelValues.at(i));
                                }
                            }
                        }
                        token = reader.readNext(); // next element inside <PLANT>
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


// write the data of Plant p to the file located at fileName
bool PersistenceManager::writePlant(QString fileName, const Plant *p){
    bool successful = true;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        successful = false;
    } else {
        QXmlStreamWriter *writer = new QXmlStreamWriter(&file);
        // beautifully indented XML
        writer->setAutoFormatting(true);
        // indentation. positive = spaces, negative = tabs
        writer->setAutoFormattingIndent(-1); // 1 tab
        // start document
        writer->writeStartDocument();
        // <PLANT>
        writer->writeStartElement(P_TAG);
        // attributes
        writer->writeAttribute(NAME_ATTRIB,p->name);
        writer->writeAttribute(MAX_AGE_ATTRIB,QString::number(p->maxAge));
        writer->writeAttribute(SEED_ATTRIB,QString::number(p->seed));

        bool writeAge = true;

        // <THICKNESS>
        writer->writeStartElement(THICK_TAG);
        for (int i=0; i<p->branchThickness.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->branchThickness.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->branchThickness.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->branchThickness.at(i).value));
            writer->writeTextElement(REL_DEV_TAG,QString::number(p->branchThickness.at(i).probability));
            writer->writeEndElement();
        }
        // </THICKNESS>
        writer->writeEndElement();

        // <LENGTH>
        writer->writeStartElement(LEN_TAG);
        for (int i=0; i<p->branchLength.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->branchLength.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->branchLength.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->branchLength.at(i).value));
            writer->writeTextElement(REL_DEV_TAG,QString::number(p->branchLength.at(i).probability));
            writer->writeEndElement();
        }
        // </LENGTH>
        writer->writeEndElement();

        // <BRANCHING>
        writer->writeStartElement(B_TAG);
        for (int i=0; i<p->branching.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->branching.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->branching.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->branching.at(i).value));
            writer->writeTextElement(PROB_TAG,QString::number(p->branching.at(i).probability));
            writer->writeEndElement();
        }
        // </BRANCHING>
        writer->writeEndElement();

        // <BRANCHING_ANGLE>
        writer->writeStartElement(B_ANGLE_TAG);
        for (int i=0; i<p->branchingAngle.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->branchingAngle.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->branchingAngle.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->branchingAngle.at(i).value));
            writer->writeTextElement(REL_DEV_TAG,QString::number(p->branchingAngle.at(i).probability));
            writer->writeEndElement();
        }
        // </BRANCHING_ANGLE>
        writer->writeEndElement();

        // <BRANCHING_ROTATION>
        writer->writeStartElement(B_ROT_TAG);
        for (int i=0; i<p->branchingRotation.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->branchingRotation.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->branchingRotation.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->branchingRotation.at(i).value));
            writer->writeTextElement(REL_DEV_TAG,QString::number(p->branchingRotation.at(i).probability));
            writer->writeEndElement();
        }
        // </BRANCHING_ROTATION>
        writer->writeEndElement();

        // <MAIN_BRANCH>
        writer->writeStartElement(MAIN_B_TAG);
        for (int i=0; i<p->mainBranch.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->mainBranch.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->mainBranch.at(i).age));
            }
            writer->writeTextElement(PROB_TAG,QString::number(p->mainBranch.at(i).probability));
            writer->writeEndElement();
        }
        // </MAIN_BRANCH>
        writer->writeEndElement();

        // <GROWTH_INTERRUPTION>
        writer->writeStartElement(GROWTH_INTERRUT_TAG);
        for (int i=0; i<p->growthInterruption.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->growthInterruption.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->growthInterruption.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->growthInterruption.at(i).value));
            writer->writeTextElement(PROB_TAG,QString::number(p->growthInterruption.at(i).probability));
            writer->writeEndElement();
        }
        // </GROWTH_INTERRUPTION>
        writer->writeEndElement();

        // <WOBBLINESS>
        writer->writeStartElement(WOBBLE_TAG);
        for (int i=0; i<p->branchWobbliness.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->branchWobbliness.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->branchWobbliness.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->branchWobbliness.at(i).value));
            writer->writeTextElement(REL_DEV_TAG,QString::number(p->branchWobbliness.at(i).probability));
            writer->writeEndElement();
        }
        // </WOBBLINESS>
        writer->writeEndElement();

        // <GRAVITATIONAL_INFLUENCE>
        writer->writeStartElement(GRAV_TAG);
        for (int i=0; i<p->gravitationalInfluence.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->gravitationalInfluence.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->gravitationalInfluence.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->gravitationalInfluence.at(i).value));
            writer->writeEndElement();
        }
        // </GRAVITATIONAL_INFLUENCE>
        writer->writeEndElement();

        // <LEAF_LEVELS>
        writer->writeStartElement(L_LEVELS_TAG);
        for (int i=0; i<p->leafLevels.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->leafLevels.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->leafLevels.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->leafLevels.at(i).value));
            writer->writeEndElement();
        }
        // </LEAF_LEVELS>
        writer->writeEndElement();

        // <LEAF_COUNT_PER_LEVEL>
        writer->writeStartElement(L_COUNT_P_LEVEL_TAG);
        for (int i=0; i<p->leafCountPerLevel.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->leafCountPerLevel.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->leafCountPerLevel.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->leafCountPerLevel.at(i).value));
            writer->writeEndElement();
        }
        // </LEAF_COUNT_PER_LEVEL>
        writer->writeEndElement();

        // <LEAF_ANGLE>
        writer->writeStartElement(L_ANGLE_TAG);
        for (int i=0; i<p->leafAngle.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->leafAngle.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->leafAngle.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->leafAngle.at(i).value));
            writer->writeEndElement();
        }
        // </LEAF_ANGLE>
        writer->writeEndElement();

        // <LEAF_LENGTH>
        writer->writeStartElement(L_LEN_TAG);
        for (int i=0; i<p->leafLength.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->leafLength.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->leafLength.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->leafLength.at(i).value));
            writer->writeEndElement();
        }
        // </LEAF_LENGTH>
        writer->writeEndElement();

        // <LEAF_WIDTH>
        writer->writeStartElement(L_WIDTH_TAG);
        for (int i=0; i<p->leafWidth.size(); i++) {
            if (i==0) {
                // <MIN>
                writer->writeStartElement(MIN_TAG);
                writeAge=false;
            } else if (i==p->leafWidth.size()-1) {
                // <MAX>
                writer->writeStartElement(MAX_TAG);
                writeAge=false;
            } else {
                // <TUPEL>
                writer->writeStartElement(TUPEL_TAG);
                writeAge=true;
            }
            if (writeAge) {
                writer->writeTextElement(AGE_TAG,QString::number(p->leafWidth.at(i).age));
            }
            writer->writeTextElement(V_TAG,QString::number(p->leafWidth.at(i).value));
            writer->writeEndElement();
        }
        // </LEAF_WIDTH>
        writer->writeEndElement();

        // </PLANT>
        writer->writeEndElement();
        // finish Doc
        writer->writeEndDocument();
    }
    file.close();
    return successful;
}
