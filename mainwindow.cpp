#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->optionsButton, SIGNAL(clicked()), this, SLOT(toggleOptionsWindow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleOptionsWindow() {
    ow.isVisible() ? ow.hide() : ow.show();
}

