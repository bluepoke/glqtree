#include "optionsformlayout.h"
#include "plant.h"
#include "time.h"
#include "scene.h"

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

    connect(txtName,SIGNAL(textChanged(QString)),this,SLOT(changeName()));
    connect(spinSeed,SIGNAL(valueChanged(int)),this,SLOT(changeSeed()));
    connect(btnRandomSeed,SIGNAL(clicked()),this,SLOT(randomSeed()));
    connect(spinAge,SIGNAL(valueChanged(int)),this,SLOT(changeMaxAge()));
}

void OptionsFormLayout::randomSeed()
{
    spinSeed->setValue(qrand());
    // no need to call redraw because spinner emits change signal
}

void OptionsFormLayout::changeMaxAge()
{
    // TODO: rescale ages
//    Plant::activePlant->maxAge = spinAge->value();
//    Scene::activeScene->initScene(Plant::activePlant);
}

void OptionsFormLayout::changeName()
{
    Plant::activePlant->name = txtName->text();
}

void OptionsFormLayout::changeSeed()
{
    Plant::activePlant->seed = spinSeed->value();
    Scene::activeScene->initScene(Plant::activePlant);
}
