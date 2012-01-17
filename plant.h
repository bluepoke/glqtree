#ifndef PLANT_H
#define PLANT_H

#include <QObject>

// classes for tupels

class Tupel2
{

public:
    explicit Tupel2(int age=0, int value=0);
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

class Plant
{
public:
    explicit Plant(int maxAge = 0, QString name = "0", int seed = 0);
    static Plant* activePlant;
private:

public:
    QString name;
    int seed;
    int maxAge;

    QList<Tupel3> branchThickness;
    QList<Tupel3> branchLength;
    QList<Tupel3> branching;
    QList<Tupel3> branchingAngle;
    QList<Tupel3> branchingRotation;
    QList<Tupel3> mainBranch;
    QList<Tupel2> gravitationalInfluence;
    QList<Tupel3> growthInterruption;
    QList<Tupel3> branchWobbliness;

    QList<Tupel2> leafLevels;
    QList<Tupel2> leafCountPerLevel;
    QList<Tupel2> leafAngle;
    QList<Tupel2> leafLength;
    QList<Tupel2> leafWidth;

    void addBranchThickness(int age, int thickness, double rel_deviation);
    void addBranchLength(int age, int length, double rel_deviation);
    void addBranching(int age, int count, double probability);
    void addBranchingAngle(int age, int angle, double rel_deviation);
    void addBranchingRotation(int age, int angle, double rel_deviation);
    void addMainBranch(int age, double probability);
    void addGravitationalInfluence(int age, int influence);
    void addGrowthInterruption(int age, int duration, double probability);
    void addBranchWobbliness(int age, int wobble, double rel_deviation);

    void addLeafLevels(int age, int count);
    void addLeafCountPerLevel(int age, int count);
    void addLeafAngle(int age, int angle);
    void addLeafLength(int age, int length);
    void addLeafWidth(int age, int width);

    int getBranchThicknessAt(int *age);
    int getBranchLengthAt(int *age);
    int getBranchingRotationAt(int *age);
    int getBranchingAngle(int *age);
    int getBranchingAt(int *age);
    int getGravitationalInfluenceAt(int *age);
    int getGrowthInterruptionAt(int *age);
    int getBranchWobblinessAt(int *age);
    int getLeafLevelsAt(int *age);
    int getLeafCountPerLevelAt(int *age);
    int getLeafAngleAt(int *age);
    int getLeafLengthAt(int *age);
    int getLeafWidthAt(int *age);
    void randomize();
    double getBranchingProbabilityAt(int *age);
    double getMainBranchProbabilityAt(int *age);
    double getGrowthInterruptionProbabilityAt(int *age);
    bool isBranchingAt(int *age);
    bool continueMainBranchAt(int *age);
    bool isGrowthInterruptingAt(int *age);
signals:
    
public slots:
    
};
#endif // PLANT_H
