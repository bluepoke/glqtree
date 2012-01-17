#include "plant.h"
#include <QList>
#include <iterator>
#include <QDebug>
#include "persistencemanager.h"

Plant* Plant::activePlant = 0; //PersistenceManager::readPlant("default.xml");

// add Tupel3 into list at correct position (regarding age)
void addTupel3(QList<Tupel3> *list, Tupel3 *tupel) {
    if (list->size()==0) {
        // no tupels in list, simply append
        list->append(*tupel);
    } else {
        QList<Tupel3>::iterator i;
        bool added = false;
        for (i = list->begin(); i != list->end(); ++i) {
            Tupel3 t = *i;
            if (t.age<tupel->age) {
                continue;
            } else {
                list->insert(i,*tupel);
                added = true;
                break;
            }
        }
        if (!added) {
            // all tupels for smaller age, append
            list->append(*tupel);
        }
    }
}

// returns interpolated value of Tupel3 at given age
int interpolateValue3(QList<Tupel3> *list, int *age) {
    Tupel3 lastTupel;
    Tupel3 interpolatedTupel;
    QList<Tupel3>::iterator i;
    for (i = list->begin(); i!= list->end(); ++i) {
        Tupel3 t = *i;
        // found exact value
        if (t.age==*age) {
            interpolatedTupel = t;
        }
         else if (t.age<*age) {
            lastTupel = t;
        }
        // interpolate
         else if (t.age>*age) {
            Tupel3 currentTupel = t;
            // elevation of value
            double m_v = (((double)currentTupel.value-lastTupel.value))/(((double)currentTupel.age-lastTupel.age));
            // value
            double v_v = ((*age-lastTupel.age)*m_v)+lastTupel.value;
            int x = v_v;
            // elevation of deviation
            double m_d = (currentTupel.probability-lastTupel.probability)/(currentTupel.age-lastTupel.age);
            // deviation
            double d = ((*age-lastTupel.age)*m_d)+lastTupel.probability;
            interpolatedTupel.age=*age;
            interpolatedTupel.value=x;
            interpolatedTupel.probability=d;
        }
    }
    return interpolatedTupel.value;
}

// returns interpolated value added with some deviation of Tupel3 at given age
int interpolateDeviatedValue3(QList<Tupel3> *list, int *age) {
    Tupel3 lastTupel;
    Tupel3 interpolatedTupel;
    QList<Tupel3>::iterator i;
    for (i = list->begin(); i!= list->end(); ++i) {
        Tupel3 t = *i;
        // found exact value
        if (t.age==*age) {
            interpolatedTupel = t;
        }
         else if (t.age<*age) {
            lastTupel = t;
        }
        // interpolate
         else if (t.age>*age) {
            Tupel3 currentTupel = t;
            // elevation of value
            double m_v = (((double)currentTupel.value-lastTupel.value))/(((double)currentTupel.age-lastTupel.age));
            // value
            double v_v = ((*age-lastTupel.age)*m_v)+lastTupel.value;
            int x = v_v;
            // elevation of deviation
            double m_d = (currentTupel.probability-lastTupel.probability)/(currentTupel.age-lastTupel.age);
            // deviation
            double d = ((*age-lastTupel.age)*m_d)+lastTupel.probability;
            interpolatedTupel.age=*age;
            interpolatedTupel.value=x;
            interpolatedTupel.probability=d;
        }
    }
    // range of deviation can be value*deviation in positive and negative direction
    int deviationRange = (double)interpolatedTupel.value*interpolatedTupel.probability;
    // calculate deviation.
    // qrand returns values in range [0,RAND_MAX)
    // therefore we fetch a random number from range [0,2*deviationRange)
    // and shift the result to the left by deviationRange.
    // it is not really exact and symmetric but it is sufficient for this application
    int deviation = qrand()%(2*deviationRange) - deviationRange;
    // add deviation to exact value
    return interpolatedTupel.value + deviation;
}

// returns the interpolated probability at given age
double interpolateProbability3(QList<Tupel3> *list, int *age) {
    Tupel3 lastTupel;
    Tupel3 interpolatedTupel;
    QList<Tupel3>::iterator i;
    for (i = list->begin(); i!= list->end(); ++i) {
        Tupel3 t = *i;
        // found exact value
        if (t.age==*age) {
            interpolatedTupel = t;
        }
         else if (t.age<*age) {
            lastTupel = t;
        }
        // interpolate
         else if (t.age>*age) {
            Tupel3 currentTupel = t;
            // elevation of value
            double m_v = (((double)currentTupel.value-lastTupel.value))/(((double)currentTupel.age-lastTupel.age));
            // value
            double v_v = ((*age-lastTupel.age)*m_v)+lastTupel.value;
            int x = v_v;
            // elevation of deviation
            double m_d = (currentTupel.probability-lastTupel.probability)/((double)currentTupel.age-lastTupel.age);
            // deviation
            double d = (((double)*age-lastTupel.age)*m_d)+lastTupel.probability;
            interpolatedTupel.age=*age;
            interpolatedTupel.value=x;
            interpolatedTupel.probability=d;
        }
    }
    return interpolatedTupel.probability;
}

//use probability value and some random number to determine if something takes place or not
bool isTakingPlace(double *probability) {
    int threshold = *probability*RAND_MAX;
    int rnd = qrand();
    if (rnd <= threshold) {
        return true;
    } else {
        return false;
    }
}

// create new plant
Plant::Plant(int maxAge, QString name, int seed) : name(name), seed(seed), maxAge(maxAge)
{
    // initialize randomizer when creating new plant
    randomize();
}

// initialize random number generator
void Plant::randomize() {
    qsrand(this->seed);
}

void Plant::addBranchThickness(int age, int thickness, double rel_deviation)
{
    Tupel3 t(age,thickness,rel_deviation);
    addTupel3(&branchThickness, &t);
}

int Plant::getBranchThicknessAt(int age) {
    return interpolateDeviatedValue3(&branchThickness, &age);
}

void Plant::addBranchLength(int age, int length, double rel_deviation)
{
    Tupel3 t(age, length, rel_deviation);
    addTupel3(&branchLength, &t);
}

int Plant::getBranchLengthAt(int age) {
    return interpolateDeviatedValue3(&branchLength, &age);
}

void Plant::addBranching(int age, int count, double probability)
{
    Tupel3 t(age,count,probability);
    addTupel3(&branching,&t);
}

int Plant::getBranchingAt(int age) {
    return interpolateValue3(&branching, &age);
}

double Plant::getBranchingProbabilityAt(int age) {
    return interpolateProbability3(&branching, &age);
}

bool Plant::isBranchingAt(int age) {
    double prob = getBranchingProbabilityAt(&age);
    return isTakingPlace(&prob);
}

void Plant::addBranchingAngle(int age, int angle, double rel_deviation)
{
    Tupel3 t(age,angle,rel_deviation);
    addTupel3(&branchingAngle,&t);
}

int Plant::getBranchingAngle(int age) {
    return interpolateDeviatedValue3(&branchingAngle, &age);
}

void Plant::addBranchingRotation(int age, int angle, double rel_deviation)
{
    Tupel3 t(age,angle,rel_deviation);
    addTupel3(&branchingRotation,&t);
}

int Plant::getBranchingRotationAt(int age) {
    return interpolateDeviatedValue3(&branchingRotation, &age);
}

void Plant::addMainBranch(int age, double probability)
{
    Tupel3 t(age,0,probability);
    addTupel3(&mainBranch,&t);
}

double Plant::getMainBranchProbabilityAt(int age)
{
    return interpolateProbability3(&mainBranch, &age);
}

bool Plant::continueMainBranchAt(int age)
{
    double prob = getMainBranchProbabilityAt(&age);
    return isTakingPlace(&prob);
}

void Plant::addGravitationalInfluence(int age, int influence)
{
    Tupel3 t(age,influence,0);
    addTupel3(&gravitationalInfluence,&t);
}

int Plant::getGravitationalInfluenceAt(int age) {
    return interpolateValue3(&gravitationalInfluence, &age);
}

void Plant::addGrowthInterruption(int age, int duration, double probability)
{
    Tupel3 t(age,duration,probability);
    addTupel3(&growthInterruption,&t);
}

int Plant::getGrowthInterruptionAt(int age) {
    return interpolateValue3(&growthInterruption, &age);
}

double Plant::getGrowthInterruptionProbabilityAt(int age) {
    return interpolateProbability3(&growthInterruption, &age);
}

bool Plant::isGrowthInterruptingAt(int age) {
    double prob = getGrowthInterruptionProbabilityAt(&age);
    return isTakingPlace(&prob);
}

void Plant::addBranchWobbliness(int age, int wobble, double rel_deviation)
{
    Tupel3 t(age,wobble,rel_deviation);
    addTupel3(&branchWobbliness,&t);
}

int Plant::getBranchWobblinessAt(int age) {
    return interpolateDeviatedValue3(&branchWobbliness, &age);
}

void Plant::addLeafLevels(int age, int count)
{
    Tupel3 t(age,count,0);
    addTupel3(&leafLevels,&t);
}

int Plant::getLeafLevelsAt(int age) {
    return interpolateValue3(&leafLevels, &age);
}

void Plant::addLeafCountPerLevel(int age, int count)
{
    Tupel3 t(age,count,0);
    addTupel3(&leafCountPerLevel,&t);
}

int Plant::getLeafCountPerLevelAt(int age) {
    return interpolateValue3(&leafCountPerLevel, &age);
}

void Plant::addLeafAngle(int age, int angle)
{
    Tupel3 t(age,angle,0);
    addTupel3(&leafAngle,&t);
}

int Plant::getLeafAngleAt(int age) {
    return interpolateValue3(&leafAngle, &age);
}

void Plant::addLeafLength(int age, int length)
{
    Tupel3 t(age,length,0);
    addTupel3(&leafLength, &t);
}

int Plant::getLeafLengthAt(int age) {
    return interpolateValue3(&leafLength, &age);
}

void Plant::addLeafWidth(int age, int width)
{
    Tupel3 t(age,width,0);
    addTupel3(&leafWidth, &t);
}

int Plant::getLeafWidthAt(int age) {
    return interpolateValue3(&leafWidth, &age);
}

// implementations for tupels

Tupel3::Tupel3(int age, int value, double probability) :
    age(age), value(value), probability(probability)
{
}

QString Tupel3::toString()
{
    return QString::number(age)+"|"+QString::number(value)+"|"+QString::number(probability);
}

