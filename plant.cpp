#include "plant.h"
#include <QList>
#include <iterator>

Plant::Plant(QObject *parent, int seed, QString name, int maxAge) :
    QObject(parent)
{
    this->seed = seed;
    this->name = name;
    this->maxAge = maxAge;
}

void addTupel2(QList<Tupel2> list, Tupel2 tupel) {
    if (list.size()==0) {
        // no tupels in list, simply append
        list.append(tupel);
    } else {
        QList<Tupel2>::iterator i;
        for (i = list.begin(); i != list.end(); ++i) {
            Tupel2 t = *i;
            if (t.age<tupel.age) {
                continue;
            } else {
                list.insert(i,tupel);
                return;
            }
        }
        // all tupels for smaller age, append
        list.append(tupel);
    }
}

void addTupel3(QList<Tupel3> list, Tupel3 tupel) {
    if (list.size()==0) {
        // no tupels in list, simply append
        list.append(tupel);
    } else {
        QList<Tupel3>::iterator i;
        for (i = list.begin(); i != list.end(); ++i) {
            Tupel3 t = *i;
            if (t.age<tupel.age) {
                continue;
            } else {
                list.insert(i,tupel);
                return;
            }
        }
        // all tupels for smaller age, append
        list.append(tupel);
    }
}

void Plant::addBranchThickness(int age, int thickness, double rel_deviation)
{
    addTupel3(branchThickness, Tupel3(age,thickness,rel_deviation));
}

void Plant::addBranchLength(int age, int length, double rel_deviation)
{
    addTupel3(branchLength, Tupel3(age, length, rel_deviation));
}

void Plant::addBranching(int age, int count, double probability)
{
    addTupel3(branching,Tupel3(age,count,probability));
}

void Plant::addBranchingAngle(int age, int angle, double rel_deviation)
{
    addTupel3(branchingAngle,Tupel3(age,angle,rel_deviation));
}

void Plant::addBranchingRotation(int age, int angle, double rel_deviation)
{
    addTupel3(branchingRotation,Tupel3(age,angle,rel_deviation));
}

void Plant::addGravitationalInfluence(int age, int influence)
{
    addTupel2(gravitationalInfluence,Tupel2(age,influence));
}

void Plant::addGrowthInterruption(int age, int duration, double probability)
{
    addTupel3(growthInterruption,Tupel3(age,duration,probability));
}

void Plant::addBranchWobbliness(int age, int wobble, double rel_deviation)
{
    addTupel3(branchWobbliness,Tupel3(age,wobble,rel_deviation));
}

void Plant::addLeafLevels(int age, int count)
{
    addTupel2(leafLevels,Tupel2(age,count));
}

void Plant::addLeafCountPerLevel(int age, int count)
{
    addTupel2(leafCountPerLevel,Tupel2(age,count));
}

void Plant::addLeafAngle(int age, int angle)
{
    addTupel2(leafAngle,Tupel2(age,angle));
}

void Plant::addLeafLength(int age, int length)
{
    addTupel2(leafLength, Tupel2(age,length));
}

void Plant::addLeafWidth(int age, int width)
{
    addTupel2(leafWidth, Tupel2(age,width));
}

// implementations for tupels

Tupel2::Tupel2(int age, int value)
{
    this->age=age;
    this->value=value;
}

Tupel3::Tupel3(int age, int value, double probability)
{
    this->age=age;
    this->value=value;
    this->probability=probability;
}
