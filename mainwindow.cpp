#include "mainwindow.h"

static const int MIN_WIDTH = 550;
static const int MIN_HEIGHT = 600;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // set minimum window size and load default plant
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    Plant::activePlant = PersistenceManager::readPlant("default.plant");

    // define a QGLFormat with multisampling and alpha blending
    QGLFormat fmt;
    fmt.setAlpha(true);
    fmt.setSampleBuffers(true);
    fmt.setSamples(8);

    // apply default format before creation of the QGLWidget (done in the ui setup)
    QGLFormat::setDefaultFormat(fmt);
    ui->setupUi(this);
    // set window title and statusbar message
    ui->statusBar->showMessage("Left-click to rotate model - right-click to move model - move mousewheel to zoom model.");
    setWindowTitle("OpenGL-Plants");

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
    // connect glpanel and optionsformlayout, and the other way around
    connect(ui->panelGL, SIGNAL(cameraChanged(int,int,int)), tabs->optionsFormLayout, SLOT(changeCamera(int,int,int)));
    connect(tabs, SIGNAL(cameraChanged(int,int,int)), ui->panelGL, SLOT(changeCamera(int,int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showOptionsDialog() {
    tabs->isVisible() ? tabs->hide() : tabs->show();
}

