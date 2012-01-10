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

void Plant::addBranchThickness(int age, int thickness, double rel_deviation)
{
    Tupel3 t(age,thickness,rel_deviation);
    addTupel3(&branchThickness, &t);
}

void Plant::addBranchLength(int age, int length, double rel_deviation)
{
    Tupel3 t(age, length, rel_deviation);
    addTupel3(&branchLength, &t);
}

void Plant::addBranching(int age, int count, double probability)
{
    Tupel3 t(age,count,probability);
    addTupel3(&branching,&t);
}

void Plant::addBranchingAngle(int age, int angle, double rel_deviation)
{
    Tupel3 t(age,angle,rel_deviation);
    addTupel3(&branchingAngle,&t);
}

void Plant::addBranchingRotation(int age, int angle, double rel_deviation)
{
    Tupel3 t(age,angle,rel_deviation);
    addTupel3(&branchingRotation,&t);
}

void Plant::addGravitationalInfluence(int age, int influence)
{
    Tupel2 t(age,influence);
    addTupel2(&gravitationalInfluence,&t);
}

void Plant::addGrowthInterruption(int age, int duration, double probability)
{
    Tupel3 t(age,duration,probability);
    addTupel3(&growthInterruption,&t);
}

void Plant::addBranchWobbliness(int age, int wobble, double rel_deviation)
{
    Tupel3 t(age,wobble,rel_deviation);
    addTupel3(&branchWobbliness,&t);
}

void Plant::addLeafLevels(int age, int count)
{
    Tupel2 t(age,count);
    addTupel2(&leafLevels,&t);
}

void Plant::addLeafCountPerLevel(int age, int count)
{
    Tupel2 t(age,count);
    addTupel2(&leafCountPerLevel,&t);
}

void Plant::addLeafAngle(int age, int angle)
{
    Tupel2 t(age,angle);
    addTupel2(&leafAngle,&t);
}

void Plant::addLeafLength(int age, int length)
{
    Tupel2 t(age,length);
    addTupel2(&leafLength, &t);
}

void Plant::addLeafWidth(int age, int width)
{
    Tupel2 t(age,width);
    addTupel2(&leafWidth, &t);
}

QList<Tupel3> Plant::getBranchThickness()
{
    return branchThickness;
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

