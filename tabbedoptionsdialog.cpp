#include "tabbedoptionsdialog.h"
#include "optionsdialogtablayout.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>

TabbedOptionsDialog::TabbedOptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    // this bool decides whether closing this dialog is possible
    updated = false;

    tabWidget = new QTabWidget;

    OptionsDialogTabLayout *tab1Layout = new OptionsDialogTabLayout(this);
    QVBoxLayout *tab2LayoutStub = new QVBoxLayout;

    // organize tab1Layout contents here like this:
    tab1Layout->initValues();

    tabWidget->addTab(new Tab1(this, tab1Layout), "Options 1");
    tabWidget->addTab(new Tab2(this, tab2LayoutStub), "Options 2");

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
    setWindowTitle("Tab Dialog");
    setModal(true);
}

// two generic tabs
Tab1::Tab1(QWidget *parent, QLayout *mainLayout)
    : QWidget(parent)
{
    //OptionsDialogTabLayout *mainLayout = new OptionsDialogTabLayout;
    setLayout(mainLayout);
}

Tab2::Tab2(QWidget *parent, QLayout *mainLayout)
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
        case QMessageBox::Save: this->saveToXML(); break;
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
    QMessageBox(QMessageBox::Information, "Open info", "Open was invoked", QMessageBox::Ok).exec();
    updated = false;
    // TODO: load new values here
}

void TabbedOptionsDialog::saveToXML() {
    QMessageBox(QMessageBox::Information, "Save info", "Save was invoked", QMessageBox::Ok).exec();
    updated = false;
    // TODO: save to XML here
    closeDialog();
}

void TabbedOptionsDialog::valuesChanged() {
    updated = true;
}