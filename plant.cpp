#include "plant.h"
#include <QList>
#include <iterator>
#include <QDebug>

Plant::Plant(QObject *parent, int seed, QString name, int maxAge) :
    QObject(parent), name(name), seed(seed), maxAge(maxAge)
{
}

void addTupel2(QList<Tupel2> *list, Tupel2 *tupel) {
    if (list->size()==0) {
        // no tupels in list, simply append
        list->append(*tupel);
    } else {
        QList<Tupel2>::iterator i;
        bool added = false;
        for (i = list->begin(); i != list->end(); ++i) {
            Tupel2 t = *i;
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

int interpolateValue3(QList<Tupel3> *list, int *age) {
    Tupel3 lastTupel;
    QList<Tupel3>::iterator i;
    for (i = list->begin(); i!= list->end(); ++i) {
        Tupel3 t = *i;
        if (t.age==*age) {
            return t.value;
        }
        if (t.age<*age) {
            lastTupel = t;
        }
        if (t.age>*age) {
            Tupel3 currentTupel = t;
            double m = (((double)currentTupel.value-lastTupel.value))/(((double)currentTupel.age-lastTupel.age));
            double v = ((*age-lastTupel.age)*m)+lastTupel.value;
            int x = v;
            return x;
        }
    }
    return NULL;
}

int interpolateValue2(QList<Tupel2> *list, int *age) {
    Tupel2 lastTupel;
    QList<Tupel2>::iterator i;
    for (i = list->begin(); i!= list->end(); ++i) {
        Tupel2 t = *i;
        if (t.age==*age) {
            return t.value;
        }
        if (t.age<*age) {
            lastTupel = t;
        }
        if (t.age>*age) {
            Tupel2 currentTupel = t;
            double m = (currentTupel.value-lastTupel.value)/(currentTupel.age-lastTupel.age);
            return ((*age-lastTupel.age)*m)+lastTupel.age;
        }
    }
    return NULL;
}

void Plant::addBranchThickness(int age, int thickness, double rel_deviation)
{
    Tupel3 t(age,thickness,rel_deviation);
    addTupel3(&branchThickness, &t);
}

int Plant::getBranchThicknessAt(int *age) {
    return interpolateValue3(&branchThickness, age);
}

void Plant::addBranchLength(int age, int length, double rel_deviation)
{
    Tupel3 t(age, length, rel_deviation);
    addTupel3(&branchLength, &t);
}

int Plant::getBranchLengthAt(int *age) {
    return interpolateValue3(&branchLength, age);
}

void Plant::addBranching(int age, int count, double probability)
{
    Tupel3 t(age,count,probability);
    addTupel3(&branching,&t);
}

int Plant::getBranchingAt(int *age) {
    return interpolateValue3(&branching, age);
}

void Plant::addBranchingAngle(int age, int angle, double rel_deviation)
{
    Tupel3 t(age,angle,rel_deviation);
    addTupel3(&branchingAngle,&t);
}

int Plant::getBranchingAngle(int *age) {
    return interpolateValue3(&branchingAngle, age);
}

void Plant::addBranchingRotation(int age, int angle, double rel_deviation)
{
    Tupel3 t(age,angle,rel_deviation);
    addTupel3(&branchingRotation,&t);
}

int Plant::getBranchingRotationAt(int *age) {
    return interpolateValue3(&branchingRotation, age);
}

void Plant::addGravitationalInfluence(int age, int influence)
{
    Tupel2 t(age,influence);
    addTupel2(&gravitationalInfluence,&t);
}

int Plant::getGravitationalInfluenceAt(int *age) {
    return interpolateValue2(&gravitationalInfluence, age);
}

void Plant::addGrowthInterruption(int age, int duration, double probability)
{
    Tupel3 t(age,duration,probability);
    addTupel3(&growthInterruption,&t);
}

int Plant::getGrowthInterruptionAt(int *age) {
    return interpolateValue3(&growthInterruption, age);
}

void Plant::addBranchWobbliness(int age, int wobble, double rel_deviation)
{
    Tupel3 t(age,wobble,rel_deviation);
    addTupel3(&branchWobbliness,&t);
}

int Plant::getBranchWobblinessAt(int *age) {
    return interpolateValue3(&branchWobbliness, age);
}

void Plant::addLeafLevels(int age, int count)
{
    Tupel2 t(age,count);
    addTupel2(&leafLevels,&t);
}

int Plant::getLeafLevelsAt(int *age) {
    return interpolateValue2(&leafLevels, age);
}

void Plant::addLeafCountPerLevel(int age, int count)
{
    Tupel2 t(age,count);
    addTupel2(&leafCountPerLevel,&t);
}

int Plant::getLeafCountPerLevelAt(int *age) {
    return interpolateValue2(&leafCountPerLevel, age);
}

void Plant::addLeafAngle(int age, int angle)
{
    Tupel2 t(age,angle);
    addTupel2(&leafAngle,&t);
}

int Plant::getLeafAngleAt(int *age) {
    return interpolateValue2(&leafAngle, age);
}

void Plant::addLeafLength(int age, int length)
{
    Tupel2 t(age,length);
    addTupel2(&leafLength, &t);
}

int Plant::getLeafLengthAt(int *age) {
    return interpolateValue2(&leafLength, age);
}

void Plant::addLeafWidth(int age, int width)
{
    Tupel2 t(age,width);
    addTupel2(&leafWidth, &t);
}

int Plant::getLeafWidthAt(int *age) {
    return interpolateValue2(&leafWidth, age);
}

// implementations for tupels

Tupel2::Tupel2(int age, int value) :
    age(age), value(value)
{
}

QString Tupel2::toString()
{
    return QString::number(age)+"|"+QString::number(value);
}

Tupel3::Tupel3(int age, int value, double probability) :
    age(age), value(value), probability(probability)
{
}

QString Tupel3::toString()
{
    return QString::number(age)+"|"+QString::number(value)+"|"+QString::number(probability);
}

