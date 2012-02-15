#include "newdialog.h"

NewDialog::NewDialog()
{
    setWindowFlags(Qt::WindowCloseButtonHint);

    // buttons for the button box
    btnCancel = new QPushButton("Cancel");
    btnNew = new QPushButton("New");

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(btnNew, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(btnCancel, QDialogButtonBox::RejectRole);

    // connect to the appropriate slots
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancelDialog()));
    connect(btnNew, SIGNAL(clicked()), this, SLOT(newPlant()));

    // the main panel
    mainPanelLayout = new QFormLayout;

    // a box for the name
    txtName = new QLineEdit("new_plant");
    mainPanelLayout->addRow("Name:", txtName);

    // a spinner for the age
    spinAge = new QSpinBox;
    spinAge->setMinimum(0);
    spinAge->setMaximum(INT_MAX);
    spinAge->setValue(20);
    mainPanelLayout->addRow("Maximum age:", spinAge);

    // a spinner for the random seed
    spinSeed = new QSpinBox;
    spinSeed->setMaximum(INT_MAX);
    spinSeed->setValue(qrand());
    btnRandomSeed = new QPushButton("Random");
    connect(btnRandomSeed, SIGNAL(clicked()), this, SLOT(randomSeed()));

    // a layout for the seed spinner + button
    QHBoxLayout *lyoSeed = new QHBoxLayout;
    lyoSeed->addWidget(spinSeed);
    lyoSeed->addWidget(btnRandomSeed);
    mainPanelLayout->addRow("Seed:", lyoSeed);

    // attatch layout to a panel
    QWidget *mainPanel = new QWidget;
    mainPanel->setLayout(mainPanelLayout);

    // layout for panel + buttonbox
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );

    // finally, drop both into the dialog
    mainLayout->addWidget(mainPanel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    // show title and make dialog modal
    setWindowTitle("New plant");
    setModal(true);
}

void NewDialog::cancelDialog()
{
    close();
}

void NewDialog::randomSeed()
{
    spinSeed->setValue(qrand());
}

void NewDialog::newPlant()
{
    int max = spinAge->value();

    Plant *p = new Plant;
    p->name = txtName->text();
    p->maxAge = max;
    p->seed = spinSeed->value();
    p->branchColor = QColor(141, 70, 0);
    p->primLeafColor = QColor(255, 255, 0);
    p->secLeafColor = QColor(0, 255, 0);
    p->drawCaps = true;
    p->drawConnectors = true;
    p->drawLeaves = true;
    p->slices = 15;
    p->segments = 5;
    p->movement = QVector3D(0, -200, 750);
    p->growthAge = max;

    p->addBranchThickness(0, 20, 0);
    p->addBranchThickness(max, 1, 0);
    p->addBranchLength(0, 20, 0);
    p->addBranchLength(max, 20, 0);
    p->addBranching(0, 0, 0);
    p->addBranching(max, 0, 0);
    p->addBranchingAngle(0, 0, 0);
    p->addBranchingAngle(max, 0, 0);
    p->addBranchingRotation(0, 0, 0);
    p->addBranchingRotation(max, 0, 0);
    p->addMainBranch(0, 1);
    p->addMainBranch(max, 1);
    p->addGrowthInterruption(0, 0, 0);
    p->addGrowthInterruption(max, 0, 0);
//    p->addGravitationalInfluence(0, 0);
//    p->addGravitationalInfluence(max, 0);
    p->addBranchWobbliness(0, 0, 0);
    p->addBranchWobbliness(max, 0, 0);
    p->addLeafAngle(0, 0);
    p->addLeafAngle(max, 0);
    p->addLeafCountPerLevel(0, 0);
    p->addLeafCountPerLevel(max, 0);
    p->addLeafLength(0, 0);
    p->addLeafLength(max, 0);
    p->addLeafLevels(0, 0);
    p->addLeafLevels(max, 0);
    p->addLeafWidth(0, 0);
    p->addLeafWidth(max, 0);

    Plant::activePlant = p;
    Scene::activeScene->initScene();
    done(QDialog::Accepted);
}
