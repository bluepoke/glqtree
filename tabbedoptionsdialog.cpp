#include "tabbedoptionsdialog.h"
#include "optionsdialogtablayout.h"

class MyPanelOpenGL;

static const int MIN_WIDTH = 550;
static const int MIN_HEIGHT = 600;

TabbedOptionsDialog::TabbedOptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::WindowMinMaxButtonsHint);
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

    tabWidget = new QTabWidget;

    initTabs();

    // buttons for the button box
    btnClose = new QPushButton("Close");
    btnOpen = new QPushButton("Open");
    btnSave = new QPushButton("Save");
    btnNew = new QPushButton("New");

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(btnNew, QDialogButtonBox::ActionRole);
    buttonBox->addButton(btnOpen, QDialogButtonBox::ActionRole);
    buttonBox->addButton(btnSave, QDialogButtonBox::ActionRole);
    buttonBox->addButton(btnClose, QDialogButtonBox::RejectRole);

    // connect to the appropriate slots
    connect(btnSave, SIGNAL(clicked()), this, SLOT(saveToXML()));
    connect(btnOpen, SIGNAL(clicked()), this, SLOT(openFromXML()));
    connect(btnClose, SIGNAL(clicked()), this, SLOT(closeDialog()));
    connect(btnNew, SIGNAL(clicked()), this, SLOT(newPlant()));

    // lay it all out
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    // show title and make dialog modal
    setWindowTitle("Options");
    setModal(false);
}

void TabbedOptionsDialog::reloadTabs() {
    tabWidget->clear();
    initTabs();
}

void TabbedOptionsDialog::initTabs() {
    OptionsDialogTabLayout *tab1Layout = new OptionsDialogTabLayout(this);
    OptionsDialogTabLayout *tab2Layout = new OptionsDialogTabLayout(this);
    OptionsDialogTabLayout *tab3Layout = new OptionsDialogTabLayout(this);    
    optionsFormLayout = new OptionsFormLayout;
    // organize tabs' contents here
    Plant *p = Plant::activePlant;
    int row = 0;
    QStringList list;

    list << "Age" << "Probability" << "Branching";
    tab1Layout->initValue(row++, new QStringList(list), p->maxAge, true, true, &(p->branching));
    list .clear();
    list << "Age" << "% Deviation" << "Angle";
    tab1Layout->initValue(row++, new QStringList(list), p->maxAge, true, true, &(p->branchingAngle));
    list .clear();
    list << "Age" << "% Deviation" << "Rotation";
    tab1Layout->initValue(row++, new QStringList(list), p->maxAge, true, true, &(p->branchingRotation));
//    list .clear();
//    list << "Age" << "% Continuity";
//    tab1Layout->initValue(row++, new QStringList(list), p->maxAge, true, false, &(p->mainBranch));
    list .clear();
    list << "Age" << "Probability" << "Interruption";
    tab1Layout->initValue(row++, new QStringList(list), p->maxAge, true, true,&(p->growthInterruption));
    tabWidget->addTab(new Tab(this, tab1Layout), "Branching Behaviour");

    row = 0;
    list .clear();
    list << "Age" << "% Deviation" << "Length";
    tab2Layout->initValue(row++, new QStringList(list), p->maxAge,true,true,&(p->branchLength));
    list .clear();
    list << "Age" << "% Deviation" << "Thickness";
    tab2Layout->initValue(row++, new QStringList(list), p->maxAge,true,true,&(p->branchThickness));
    list .clear();
    list << "Age" << "Probability" << "Wobbliness";
    tab2Layout->initValue(row++, new QStringList(list), p->maxAge,true,true,&(p->branchWobbliness));
    list .clear();
    list << "Age" << "% Continuity";
    tab2Layout->initValue(row++, new QStringList(list), p->maxAge, true, false, &(p->mainBranch));
//    list .clear();
//    list << "Age" << "Gravitation";
//    tab2Layout->initValue(row++, new QStringList(list), p->maxAge,false,true,&(p->gravitationalInfluence));
    tabWidget->addTab(new Tab(this,tab2Layout), "Branch Geometry");

    row = 0;
    list .clear();
    list << "Age" << "Count/level";
    tab3Layout->initValue(row++, new QStringList(list), p->maxAge,false,true,&(p->leafCountPerLevel));
    list .clear();
    list << "Age" << "Levels";
    tab3Layout->initValue(row++, new QStringList(list), p->maxAge,false,true,&(p->leafLevels));
    list .clear();
    list << "Age" << "Angle";
    tab3Layout->initValue(row++, new QStringList(list), p->maxAge,false,true,&(p->leafAngle));
    list .clear();
    list << "Age" << "Length";
    tab3Layout->initValue(row++, new QStringList(list),p->maxAge,false,true,&(p->leafLength));
    list .clear();
    list << "Age" << "Width";
    tab3Layout->initValue(row++, new QStringList(list), p->maxAge,false,true,&(p->leafWidth));
    tabWidget->addTab(new Tab(this, tab3Layout), "Leaf Geometry");

    // other options
    tabWidget->addTab(new Tab(this, optionsFormLayout), "Other");
    connect(optionsFormLayout,SIGNAL(treeRescaled()),this,SLOT(refreshData()));
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
    QString fileName = QFileDialog::getOpenFileName(this,"Open plant","","Plants (*.plant);;XML files (*.xml)");
    if (!(fileName.isEmpty() || fileName.isNull())) {
            Plant::activePlant = PersistenceManager::readPlant(fileName);
            Scene::activeScene->initScene();
            reloadTabs();
            emit cameraChanged(Plant::activePlant->movement.x(), Plant::activePlant->movement.y(),
                               Plant::activePlant->movement.z());
    }
}

void TabbedOptionsDialog::saveToXML() {
    QString fileName = QFileDialog::getSaveFileName(this,"Save plant configuration","","Plant (*.plant);;XML file (*.xml)");
    if (!(fileName.isEmpty() || fileName.isNull())) {
            PersistenceManager::writePlant(fileName,Plant::activePlant);
    }
}

void TabbedOptionsDialog::newPlant()
{
    NewDialog *newDialog = new NewDialog;
    newDialog->setModal(true);
    int result = newDialog->exec();
    if (result == QDialog::Accepted) {
        reloadTabs();
        emit cameraChanged(Plant::activePlant->movement.x(), Plant::activePlant->movement.y(),
                           Plant::activePlant->movement.z());
    }
}

void TabbedOptionsDialog::changeCamera(int x, int y, int zoom)
{
    optionsFormLayout->spinXMove->setValue(x);
    optionsFormLayout->spinYMove->setValue(y);
    optionsFormLayout->spinZoom->setValue(zoom);
    Plant::activePlant->movement = QVector3D(x, y, zoom);
}

void TabbedOptionsDialog::refreshData()
{
    reloadTabs();
}

void TabbedOptionsDialog::updateCounts() {
    optionsFormLayout->updateStats();
}

void TabbedOptionsDialog::valuesChanged() {
    ValuesTable *v = (ValuesTable*)QObject::sender()->parent()->parent();

    // Branching changed
    if (v->pValues == &(Plant::activePlant->branching)) {
        Plant::activePlant->branching.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addBranching(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Branching angle changed
    else if (v->pValues == &(Plant::activePlant->branchingAngle)) {
        Plant::activePlant->branchingAngle.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addBranchingAngle(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Branching rotatiton changed
    else if (v->pValues == &(Plant::activePlant->branchingRotation)) {
        Plant::activePlant->branchingRotation.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addBranchingRotation(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Branch length changed
    else if (v->pValues == &(Plant::activePlant->branchLength)) {
        Plant::activePlant->branchLength.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addBranchLength(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Thickness changed
    else if (v->pValues == &(Plant::activePlant->branchThickness)) {
        Plant::activePlant->branchThickness.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addBranchThickness(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Wobbliness changed
    else if (v->pValues == &(Plant::activePlant->branchWobbliness)) {
        Plant::activePlant->branchWobbliness.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addBranchWobbliness(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
//    // Grav. influence changed
//    else if (v->pValues == &(p->gravitationalInfluence)) {
//        p->gravitationalInfluence.clear();
//        for (int i = v->rowCount() - 2; i >= 0; i--) {
//            p->addGravitationalInfluence(((QLabel*)v->cellWidget(i,0))->text().toInt(),
//                            ((QSpinBox*)v->cellWidget(i,1))->value());
//        }
//    }
    // Growth interruption changed
    else if (v->pValues == &(Plant::activePlant->growthInterruption)) {
        Plant::activePlant->growthInterruption.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addGrowthInterruption(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,2))->value(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Main branch changed
    else if (v->pValues == &(Plant::activePlant->mainBranch)) {
        Plant::activePlant->mainBranch.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addMainBranch(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QDoubleSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Leaf angle changed
    else if (v->pValues == &(Plant::activePlant->leafAngle)) {
        Plant::activePlant->leafAngle.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addLeafAngle(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Leaf count p. lvl changed
    else if (v->pValues == &(Plant::activePlant->leafCountPerLevel)) {
        Plant::activePlant->leafCountPerLevel.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addLeafCountPerLevel(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Leaf length changed
    else if (v->pValues == &(Plant::activePlant->leafLength)) {
        Plant::activePlant->leafLength.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addLeafLength(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Leaf levels changed
    else if (v->pValues == &(Plant::activePlant->leafLevels)) {
        Plant::activePlant->leafLevels.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addLeafLevels(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // Leaf width changed
    else if (v->pValues == &(Plant::activePlant->leafWidth)) {
        Plant::activePlant->leafWidth.clear();
        for (int i = v->rowCount() - 2; i >= 0; i--) {
            Plant::activePlant->addLeafWidth(((QLabel*)v->cellWidget(i,0))->text().toInt(),
                            ((QSpinBox*)v->cellWidget(i,1))->value());
        }
    }
    // rebuild scene
    Scene::activeScene->initScene();
}
