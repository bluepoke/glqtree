#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // the options button needs to do something
    connect(ui->optionsButton, SIGNAL(clicked()), this, SLOT(toggleOptionsWindow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleOptionsWindow() {
    // show or hide? that is the question!
    ow.isVisible() ? ow.hide() : ow.show();
}

