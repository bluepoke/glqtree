#ifndef PLANT_H
#define PLANT_H

#include <QObject>

// classes for tupels

class Tupel2
{

public:
    explicit Tupel2(int age, int value);
    QString toString();
    int age;
    int value;

signals:

public slots:

};

class Tupel3
{

public:
    explicit Tupel3(int age=0, int value=0, double probability=0);
    QString toString();
    int age;
    int value;
    double probability;

signals:

public slots:

};

// class for plant

class Plant : public QObject
{
    Q_OBJECT
public:
    explicit Plant(QObject *parent = 0,int seed = 0, QString name = 0, int maxAge = 0);
    int getSeed() {
        return seed;
    }
    QString getName() {
        return name;
    }

private:
    QString name;
    int seed;
    int maxAge;

    QList<Tupel3> branchThickness;
    QList<Tupel3> branchLength;
    QList<Tupel3> branching;
    QList<Tupel3> branchingAngle;
    QList<Tupel3> branchingRotation;
    QList<Tupel2> gravitationalInfluence;
    QList<Tupel3> growthInterruption;
    QList<Tupel3> branchWobbliness;

    QList<Tupel2> leafLevels;
    QList<Tupel2> leafCountPerLevel;
    QList<Tupel2> leafAngle;
    QList<Tupel2> leafLength;
    QList<Tupel2> leafWidth;

public:
    void addBranchThickness(int age, int thickness, double rel_deviation);
    void addBranchLength(int age, int length, double rel_deviation);
    void addBranching(int age, int count, double probability);
    void addBranchingAngle(int age, int angle, double rel_deviation);
    void addBranchingRotation(int age, int angle, double rel_deviation);
    void addGravitationalInfluence(int age, int influence);
    void addGrowthInterruption(int age, int duration, double probability);
    void addBranchWobbliness(int age, int wobble, double rel_deviation);

    void addLeafLevels(int age, int count);
    void addLeafCountPerLevel(int age, int count);
    void addLeafAngle(int age, int angle);
    void addLeafLength(int age, int length);
    void addLeafWidth(int age, int width);

signals:
    
public slots:
    
};

#endif // PLANT_H
