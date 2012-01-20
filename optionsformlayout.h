#ifndef OPTIONSFORMLAYOUT_H
#define OPTIONSFORMLAYOUT_H

#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>

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
    QSpinBox *spinXMove;
    QSpinBox *spinYMove;
    QSpinBox *spinZoom;
    ColorLabel *clrTreeColor;
    ColorLabel *clrLeafColor;

signals:
    
public slots:
    void randomSeed();
    void changeSeed(int seed);
    void changeName(QString name);
    void changeMaxAge(int maxAge);

    void changeLeafColor();
    void changeBranchColor();

    void changeSlices(int slices);
    void changeSegments(int segments);

    void switchLeaves(bool toggle);
    void switchBranchCaps(bool toggle);
    void switchConnectors(bool toggle);

    void changeCamera(int x, int y, int zoom);
};

#endif // OPTIONSFORMLAYOUT_H
