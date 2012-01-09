#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // the options button needs to do something
    connect(ui->optionsButton, SIGNAL(clicked()), this, SLOT(showOptionsDialog()));
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

