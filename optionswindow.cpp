#include "optionswindow.h"
#include "ui_optionswindow.h"

OptionsWindow::OptionsWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::OptionsWindow)
{
    // set up the window
    ui->setupUi(this);
    // add a single layout, handling every single child widget
    // (there are two tabs through the ui creator, adding this can be done to each and all of them)
    OptionsWindowLayout *tab2Layout = new OptionsWindowLayout(ui->tab2);
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}
