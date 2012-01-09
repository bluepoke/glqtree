#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // some buttons for the button box
    btnQuit = new QPushButton("Quit");
    btnOptions = new QPushButton("Options");

    ui->buttonBox->addButton(btnOptions, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(btnQuit, QDialogButtonBox::RejectRole);

    // a reject action for the quit button
    connect(btnQuit, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnOptions, SIGNAL(clicked()), this, SLOT(showOptionsDialog()));

    // prepare a tabbed Options Dialog
    tabs = new TabbedOptionsDialog;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showOptionsDialog() {
    tabs->show();
}

