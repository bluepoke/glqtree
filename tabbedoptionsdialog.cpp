#include "tabbedoptionsdialog.h"
#include "optionsdialogtablayout.h"
#include <QVBoxLayout>

TabbedOptionsDialog::TabbedOptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    tabWidget = new QTabWidget;
    tabWidget->addTab(new Tab1, "Options 1");
    tabWidget->addTab(new Tab2, "Options 2");

    // default buttons may need changing to disable closing of dialog on save and open
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Open
                                     | QDialogButtonBox::Save | QDialogButtonBox::Close);

    // accept role is defined for open and save default buttons
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    // reject role is defined for close default button
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

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
Tab1::Tab1(QWidget *parent)
    : QWidget(parent)
{
    OptionsDialogTabLayout *mainLayout = new OptionsDialogTabLayout;
    setLayout(mainLayout);
}

Tab2::Tab2(QWidget *parent)
    : QWidget(parent)
{
}
