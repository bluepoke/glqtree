#include "newdialog.h"

NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::WindowCloseButtonHint);

    // buttons for the button box
    btnCancel = new QPushButton("Cancel");
    btnNew = new QPushButton("New");

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(btnNew, QDialogButtonBox::ActionRole);
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
    delete this;
}

void NewDialog::randomSeed()
{
    spinSeed->setValue(qrand());
}

void NewDialog::newPlant()
{
    // TODO create a new, empty plant
    // pick color from the currently displayed plant

}
