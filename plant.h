#ifndef PLANT_H
#define PLANT_H

#include <QObject>
#include <QList>
#include <cstdlib>
#include <QColor>
#include <QVector3D>

// classes for tupels

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

    QColor branchColor;
    QColor primLeafColor;
    QColor secLeafColor;
    bool drawLeaves;
    bool drawCaps;
    bool drawConnectors;
    int slices;
    int segments;
    QVector3D movement;
    int growthAge;

    QList<Tupel3> branchThickness;
    QList<Tupel3> branchLength;
    QList<Tupel3> branching;
    QList<Tupel3> branchingAngle;
    QList<Tupel3> branchingRotation;
    QList<Tupel3> mainBranch;
//    QList<Tupel3> gravitationalInfluence;
    QList<Tupel3> growthInterruption;
    QList<Tupel3> branchWobbliness;

    QList<Tupel3> leafLevels;
    QList<Tupel3> leafCountPerLevel;
    QList<Tupel3> leafAngle;
    QList<Tupel3> leafLength;
    QList<Tupel3> leafWidth;

    QList<QList<Tupel3>*> dataList;

    void addBranchThickness(int age, int thickness, double rel_deviation);
    void addBranchLength(int age, int length, double rel_deviation);
    void addBranching(int age, int count, double probability);
    void addBranchingAngle(int age, int angle, double rel_deviation);
    void addBranchingRotation(int age, int angle, double rel_deviation);
    void addMainBranch(int age, double probability);
//    void addGravitationalInfluence(int age, int influence);
    void addGrowthInterruption(int age, int duration, double probability);
    void addBranchWobbliness(int age, int wobble, double probability);

    void addLeafLevels(int age, int count);
    void addLeafCountPerLevel(int age, int count);
    void addLeafAngle(int age, int angle);
    void addLeafLength(int age, int length);
    void addLeafWidth(int age, int width);

    int getBranchThicknessAt(int age);
    int getBranchLengthAt(int age);
    int getBranchingRotationAt(int age);
    int getBranchingAngle(int age);
    int getBranchCountAt(int age);
//    int getGravitationalInfluenceAt(int age);
    int getGrowthInterruptionAt(int age);
    int getBranchWobblinessAt(int age);
    int getLeafLevelsAt(int age);
    int getLeafCountPerLevelAt(int age);
    int getLeafAngleAt(int age);
    int getLeafLengthAt(int age);
    int getLeafWidthAt(int age);
    void reseed();
    double getBranchingProbabilityAt(int age);
    double getMainBranchProbabilityAt(int age);
    double getGrowthInterruptionProbabilityAt(int age);
    bool isBranchingAt(int age);
    bool continueMainBranchAt(int age);
    bool isGrowthInterruptingAt(int age);
    int coinflip();
    double getBranchWobblinessProbabilityAt(int age);
    bool isBranchWobblinessAt(int age);
    int getRandomRotationBetween(int low, int high);
    void scaleMaxAgeTo(int age);
signals:
    
public slots:
    
};
#endif // PLANT_H
