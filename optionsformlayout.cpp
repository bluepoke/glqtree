#include "optionsformlayout.h"
#include "plant.h"
#include "time.h"
#include "scene.h"
#include <QColorDialog>
#include <QDebug>

OptionsFormLayout::OptionsFormLayout(QWidget *parent) :
    QFormLayout(parent)
{
    Plant *p = Plant::activePlant;
    txtName= new QLineEdit(p->name);
    this->addRow("Name:",txtName);
    spinAge = new QSpinBox();
    spinAge->setMinimum(0);
    spinAge->setMaximum(INT_MAX);
    spinAge->setValue(p->maxAge);
    this->addRow("Maximum age:",spinAge);
    spinSeed = new QSpinBox();
    spinSeed->setMaximum(INT_MAX);
    spinSeed->setValue(p->seed);
    btnRandomSeed = new QPushButton("Random");
    QHBoxLayout *lyoSeed = new QHBoxLayout();
    lyoSeed->addWidget(spinSeed,1);
    lyoSeed->addWidget(btnRandomSeed);
    this->addRow("Seed:",lyoSeed);

    clrLeafColor = new ColorLabel("  ",0);
    clrLeafColor->setAutoFillBackground(true);
    QPalette palette = clrLeafColor->palette();
    // FIXME: plant's leaf color
    palette.setColor(palette.Background,QColor(Qt::green));
    clrLeafColor->setPalette(palette);
    this->addRow("Leaf color",clrLeafColor);

    clrTreeColor = new ColorLabel("  ",0);
    clrTreeColor->setAutoFillBackground(true);
    palette = clrTreeColor->palette();
    // FIXME: plant's branch color
    palette.setColor(palette.Background,QColor(Qt::blue));
    clrTreeColor->setPalette(palette);
    this->addRow("Branch color",clrTreeColor);

    cbxLeaves = new QCheckBox("Draw leaves");
    // FIXME: plant's setting
    cbxLeaves->setChecked(true);
    this->addRow("",cbxLeaves);

    cbxBranchCaps = new QCheckBox("Draw branch caps");
    // FIXME: plant's setting
    cbxBranchCaps->setChecked(true);
    this->addRow("",cbxBranchCaps);

    cbxConnectors = new QCheckBox("Draw connectors");
    // FIXME: plant's setting
    cbxConnectors->setChecked(true);
    this->addRow("",cbxConnectors);

    spinSlices = new QSpinBox();
    // FIXME: plant's setting
    spinSlices->setValue(5);
    this->addRow("Slices",spinSlices);

    spinSegments = new QSpinBox();
    // FIXME: plant's setting
    spinSegments->setValue(5);
    this->addRow("Segments",spinSegments);

    spinXMove = new QSpinBox();
    // FIXME: plant's setting
    spinXMove->setValue(0);
    spinXMove->setMaximum(INT_MAX);
    spinXMove->setMinimum(INT_MIN);
    // not editable, reacts to mouse movement
    spinXMove->setEnabled(false);
    this->addRow("Movement X",spinXMove);

    spinYMove = new QSpinBox();
    // FIXME: plant's setting
    spinYMove->setValue(0);
    spinYMove->setMaximum(INT_MAX);
    spinYMove->setMinimum(INT_MIN);
    spinYMove->setEnabled(false);
    this->addRow("Movement Y",spinYMove);

    spinZoom = new QSpinBox();
    // FIXME: plant's setting
    spinZoom->setValue(0);
    spinZoom->setMaximum(INT_MAX);
    spinZoom->setMinimum(INT_MIN);
    spinZoom->setEnabled(false);
    this->addRow("Zoom",spinZoom);

    connect(txtName,SIGNAL(textChanged(QString)),this,SLOT(changeName(QString)));
    connect(spinSeed,SIGNAL(valueChanged(int)),this,SLOT(changeSeed(int)));
    connect(btnRandomSeed,SIGNAL(clicked()),this,SLOT(randomSeed()));
    connect(spinAge,SIGNAL(valueChanged(int)),this,SLOT(changeMaxAge(int)));
    connect(clrLeafColor,SIGNAL(clicked()),this,SLOT(changeLeafColor()));
    connect(clrTreeColor,SIGNAL(clicked()),this,SLOT(changeBranchColor()));
    connect(cbxLeaves,SIGNAL(toggled(bool)),this,SLOT(switchLeaves(bool)));
    connect(cbxBranchCaps,SIGNAL(toggled(bool)),this,SLOT(switchBranchCaps(bool)));
    connect(cbxConnectors,SIGNAL(toggled(bool)),this,SLOT(switchConnectors(bool)));
    connect(spinSlices,SIGNAL(valueChanged(int)),this,SLOT(changeSlices(int)));
    connect(spinSegments,SIGNAL(valueChanged(int)),this,SLOT(changeSegments(int)));


}

void OptionsFormLayout::randomSeed()
{
    spinSeed->setValue(qrand());
    // no need to call redraw because spinner emits change signal
}

void OptionsFormLayout::changeMaxAge(int maxAge)
{
    // TODO: rescale ages
//    Plant::activePlant->maxAge = spinAge->value();
    //    Scene::activeScene->initScene(Plant::activePlant);
}

void OptionsFormLayout::changeLeafColor()
{
    QPalette p = clrLeafColor->palette();
    QColor color = QColorDialog::getColor(p.color(clrLeafColor->backgroundRole()));
    p.setColor(p.Background,color);
    clrLeafColor->setPalette(p);
    // TODO: update color in plant
}

void OptionsFormLayout::changeBranchColor()
{
    QPalette p = clrTreeColor->palette();
    QColor color = QColorDialog::getColor(p.color(clrTreeColor->backgroundRole()));
    p.setColor(p.Background,color);
    clrTreeColor->setPalette(p);
    // TODO: update color in plant
}

void OptionsFormLayout::changeSlices(int slices)
{
}

void OptionsFormLayout::changeSegments(int segments)
{
}

void OptionsFormLayout::switchLeaves(bool toggle)
{
}

void OptionsFormLayout::switchBranchCaps(bool toggle)
{
}

void OptionsFormLayout::switchConnectors(bool toggle)
{
}

void OptionsFormLayout::changeCamera(int x, int y, int zoom)
{
    spinXMove->setValue(x);
    spinYMove->setValue(y);
    spinZoom->setValue(zoom);
}

void OptionsFormLayout::changeName(QString name)
{
    Plant::activePlant->name = name;
}

void OptionsFormLayout::changeSeed(int seed)
{
    Plant::activePlant->seed = seed;
    Scene::activeScene->initScene(Plant::activePlant);
}


// derived a QLabel to make it clickable

ColorLabel::ColorLabel( const QString & text, QWidget * parent )
:QLabel(text,parent)
{
}

void ColorLabel::mousePressEvent ( QMouseEvent * event )
{
    emit clicked();
}
