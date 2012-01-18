#include "tabbedoptionsdialog.h"
#include "optionsdialogtablayout.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

TabbedOptionsDialog::TabbedOptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    // this bool decides whether closing this dialog is possible
    updated = false;

    tabWidget = new QTabWidget;

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
    tab2Layout->initValue(row++, "Length",p->maxAge,true,true,&(p->branchLength));
    tab2Layout->initValue(row++, "Thickness", p->maxAge,true,true,&(p->branchThickness));
    tab2Layout->initValue(row++, "Wobbliness", p->maxAge,true,true,&(p->branchWobbliness));
    tab2Layout->initValue(row++, "Gravitation", p->maxAge,false,true,&(p->gravitationalInfluence));
    tabWidget->addTab(new Tab(this,tab2Layout), "Branch Geometry");

    row = 0;
    tab3Layout->initValue(row++, "Count per level", p->maxAge,false,true,&(p->leafCountPerLevel));
    tab3Layout->initValue(row++,"Levels",p->maxAge,false,true,&(p->leafLevels));
    tab3Layout->initValue(row++, "Angle", p->maxAge,false,true,&(p->leafAngle));
    tab3Layout->initValue(row++,"Length",p->maxAge,false,true,&(p->leafLength));
    tab3Layout->initValue(row++,"Width",p->maxAge,false,true,&(p->leafWidth));
    tabWidget->addTab(new Tab(this, tab3Layout), "Leaf Geometry");

    tabWidget->addTab(new Tab(this, tab4LayoutStub), "Other");

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

// a generic tabs
Tab::Tab(QWidget *parent, QLayout *mainLayout)
    : QWidget(parent)
{
    setLayout(mainLayout);
}

void TabbedOptionsDialog::closeDialog() {
    if (updated) {
        // at least have the courtesy to ask before descarding any changes
        int ret = QMessageBox(QMessageBox::Question, "Save values?",
                    "There are unsaved values.\nDo you want to save them now?",
                              QMessageBox::Cancel | QMessageBox::Save | QMessageBox::Discard).exec();

        switch (ret) {
        case QMessageBox::Discard: this->close(); break;
        case QMessageBox::Save: this->saveToXML(); this->close(); break;
        default: break;
        }
    }
    else {
        updated = false;
        // TODO: reload currently loaded values here
        this->close();
    }
}

void TabbedOptionsDialog::openFromXML() {
    QString fileName = QFileDialog::getOpenFileName(this,"Open plant from XML file","","XML files (*.xml)");
    // QMessageBox(QMessageBox::Information, "Open info", "Open was invoked", QMessageBox::Ok).exec();
    updated = false;
    // TODO: load new values here
}

void TabbedOptionsDialog::saveToXML() {
    QString fileName = QFileDialog::getSaveFileName(this,"Save plant configuration to XML file","","XML files (*.xml)");
    //QMessageBox(QMessageBox::Information, "Save info", "Save was invoked", QMessageBox::Ok).exec();
    updated = false;
    // TODO: save to XML here
}

void TabbedOptionsDialog::valuesChanged() {
    updated = true;
    qDebug() << QObject::sender();
}
