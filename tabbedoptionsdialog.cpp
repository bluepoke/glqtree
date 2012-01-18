#include "tabbedoptionsdialog.h"
#include "optionsdialogtablayout.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "scene.h"
#include "persistencemanager.h"

class MyPanelOpenGL;

static const int MIN_WIDTH = 550;
static const int MIN_HEIGHT = 600;

TabbedOptionsDialog::TabbedOptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::WindowContextHelpButtonHint | Qt::WindowMinMaxButtonsHint);
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

    tabWidget = new QTabWidget;

    initTabs();

    // buttons for the button box
    btnClose = new QPushButton("Close");
    btnOpen = new QPushButton("Open");
    btnSave = new QPushButton("Save");

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(btnOpen, QDialogButtonBox::ActionRole);
    buttonBox->addButton(btnSave, QDialogButtonBox::ActionRole);
    buttonBox->addButton(btnClose, QDialogButtonBox::RejectRole);

    // connect to the appropriate slots
    connect(btnSave, SIGNAL(clicked()), this, SLOT(saveToXML()));
    connect(btnOpen, SIGNAL(clicked()), this, SLOT(openFromXML()));
    connect(btnClose, SIGNAL(clicked()), this, SLOT(closeDialog()));

    // lay it all out
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    // show title and make dialog modal
    setWindowTitle("Options");
    setModal(true);
}

void TabbedOptionsDialog::reloadTabs() {
    tabWidget->clear();
    initTabs();
}

void TabbedOptionsDialog::initTabs() {
    OptionsDialogTabLayout *tab1Layout = new OptionsDialogTabLayout(this);
    OptionsDialogTabLayout *tab2Layout = new OptionsDialogTabLayout(this);
    OptionsDialogTabLayout *tab3Layout = new OptionsDialogTabLayout(this);
    QVBoxLayout *tab4LayoutStub = new QVBoxLayout;

    // organize tabs' contents here
    Plant *p = Plant::activePlant;
    int row = 0;
    tab1Layout->initValue(row++, "Branching", p->maxAge, true, true, &(p->branching));
    tab1Layout->initValue(row++, "Angle", p->maxAge, true, true, &(p->branchingAngle));
    tab1Layout->initValue(row++, "Rotation", p->maxAge, true, true, &(p->branchingRotation));
    tab1Layout->initValue(row++, "Main Branch", p->maxAge, true, false, &(p->mainBranch));
    tab1Layout->initValue(row++, "Interruption", p->maxAge, true, true,&(p->growthInterruption));
    tabWidget->addTab(new Tab(this, tab1Layout), "Branching Behaviour");

    row = 0;
    tab2Layout->initValue(row++, "Length", p->maxAge,true,true,&(p->branchLength));
    tab2Layout->initValue(row++, "Thickness", p->maxAge,true,true,&(p->branchThickness));
    tab2Layout->initValue(row++, "Wobbliness", p->maxAge,true,true,&(p->branchWobbliness));
    tab2Layout->initValue(row++, "Gravitation", p->maxAge,false,true,&(p->gravitationalInfluence));
    tabWidget->addTab(new Tab(this,tab2Layout), "Branch Geometry");

    row = 0;
    tab3Layout->initValue(row++, "Count/level", p->maxAge,false,true,&(p->leafCountPerLevel));
    tab3Layout->initValue(row++, "Levels", p->maxAge,false,true,&(p->leafLevels));
    tab3Layout->initValue(row++, "Angle", p->maxAge,false,true,&(p->leafAngle));
    tab3Layout->initValue(row++, "Length",p->maxAge,false,true,&(p->leafLength));
    tab3Layout->initValue(row++, "Width", p->maxAge,false,true,&(p->leafWidth));
    tabWidget->addTab(new Tab(this, tab3Layout), "Leaf Geometry");

    tabWidget->addTab(new Tab(this, tab4LayoutStub), "Other");
}

// a generic tabs
Tab::Tab(QWidget *parent, QLayout *mainLayout)
    : QWidget(parent)
{
    setLayout(mainLayout);
}

void TabbedOptionsDialog::closeDialog() {
    close();
}

void TabbedOptionsDialog::openFromXML() {
    QString fileName = QFileDialog::getOpenFileName(this,"Open plant from XML file","","XML files (*.xml)");
    if (!(fileName.isEmpty() || fileName.isNull())) {
            Plant::activePlant = PersistenceManager::readPlant(fileName);
            Scene::activeScene->initScene(Plant::activePlant);
            reloadTabs();
    }
}

void TabbedOptionsDialog::saveToXML() {
    QString fileName = QFileDialog::getSaveFileName(this,"Save plant configuration to XML file","","XML files (*.xml)");
    if (!(fileName.isEmpty() || fileName.isNull())) {
            PersistenceManager::writePlant(fileName,Plant::activePlant);
    }
}

void TabbedOptionsDialog::valuesChanged() {
    Plant *p = Plant::activePlant;
    ValuesTable *v = (ValuesTable*)QObject::sender()->parent()->parent();

    // Branching changed
    if (v->pValues == &(p->branching)) {
        p->branching.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addBranching(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Branching angle changed
    else if (v->pValues == &(p->branchingAngle)) {
        p->branchingAngle.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addBranchingAngle(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Branching rotatiton changed
    else if (v->pValues == &(p->branchingRotation)) {
        p->branchingRotation.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addBranchingRotation(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Branch length changed
    else if (v->pValues == &(p->branchLength)) {
        p->branchLength.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addBranchLength(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Thickness changed
    else if (v->pValues == &(p->branchThickness)) {
        p->branchThickness.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addBranchThickness(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Wobbliness changed
    else if (v->pValues == &(p->branchWobbliness)) {
        p->branchWobbliness.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addBranchWobbliness(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Grav. influence changed
    else if (v->pValues == &(p->gravitationalInfluence)) {
        p->gravitationalInfluence.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addGravitationalInfluence(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Growth interruption changed
    else if (v->pValues == &(p->growthInterruption)) {
        p->growthInterruption.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addGrowthInterruption(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Main branch changed
    else if (v->pValues == &(p->mainBranch)) {
        p->mainBranch.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addMainBranch(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Leaf angle changed
    else if (v->pValues == &(p->leafAngle)) {
        p->leafAngle.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addLeafAngle(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Leaf count p. lvl changed
    else if (v->pValues == &(p->leafCountPerLevel)) {
        p->leafCountPerLevel.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addLeafCountPerLevel(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Leaf length changed
    else if (v->pValues == &(p->leafLength)) {
        p->leafLength.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addLeafLength(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Leaf levels changed
    else if (v->pValues == &(p->leafLevels)) {
        p->leafLevels.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addLeafLevels(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Leaf width changed
    else if (v->pValues == &(p->leafWidth)) {
        p->leafWidth.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            p->addLeafWidth(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // reset seed
    Plant::activePlant->reseed();
    // rebuild scene
    Scene::activeScene->initScene(Plant::activePlant);
}
