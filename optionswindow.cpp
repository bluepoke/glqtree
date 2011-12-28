#include "optionswindow.h"
#include "ui_optionswindow.h"

OptionsWindow::OptionsWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}
