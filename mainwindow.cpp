#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // define a QGLFormat with multisampling and alpha blending
    QGLFormat fmt;
    fmt.setAlpha(true);
    fmt.setSampleBuffers(true);
    fmt.setSamples(8);

    // apply default format before creation of the QGLWidget (done in the ui setup)
    QGLFormat::setDefaultFormat(fmt);
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

