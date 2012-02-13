#ifndef OPTIONSFORMLAYOUT_H
#define OPTIONSFORMLAYOUT_H

#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include "scene.h"
#include <QColorDialog>

class ColorLabel : public QLabel
{
    Q_OBJECT

public:
    ColorLabel( const QString & text, QWidget * parent);
    void mousePressEvent(QMouseEvent *event);
signals:
    void clicked();

};

class OptionsFormLayout : public QFormLayout
{
    Q_OBJECT
public:
    explicit OptionsFormLayout(QWidget *parent = 0);
    QSpinBox *spinXMove;
    QSpinBox *spinYMove;
    QSpinBox *spinZoom;
    QLabel *lblBranches;
    QLabel *lblSpheres;
    QLabel *lblLeaves;
    QLabel *lblPolygons;
    QLabel *lblTriangles;

private:
    QLineEdit *txtName;
    QSpinBox *spinAge;
    QSpinBox *spinSeed;
    QPushButton *btnRandomSeed;
    QSpinBox *spinSlices;
    QSpinBox *spinSegments;
    QCheckBox *cbxLeaves;
    QCheckBox *cbxBranchCaps;
    QCheckBox *cbxConnectors;
    ColorLabel *clrTreeColor;
    ColorLabel *clrPrimLeafColor;
    ColorLabel *clrSecLeafColor;
    ColorLabel *clrBackgroundColor;
    QSpinBox *spinGrowth;

signals:
    void treeRescaled();
    
public slots:
    void randomSeed();
    void changeSeed(int seed);
    void changeName(QString name);
    void changeMaxAge();

    void changePrimLeafColor();
    void changeSecLeafColor();
    void changeBranchColor();
    void changeBackgroundColor();

    void changeSlices(int slices);
    void changeSegments(int segments);

    void switchLeaves(bool toggle);
    void switchBranchCaps(bool toggle);
    void switchConnectors(bool toggle);

    void changeGrowthAge(int age);
    void updateStats();
};

#endif // OPTIONSFORMLAYOUT_H
