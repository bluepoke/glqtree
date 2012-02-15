#include "optionsformlayout.h"
#include "tabbedoptionsdialog.h"
#include <QDebug>

OptionsFormLayout::OptionsFormLayout(QWidget *parent) :
    QFormLayout(parent)
{
    growthDelay = 5;
    growthDelayCounter = 0;
    growUpwards = true;
    Plant *p = Plant::activePlant;
    txtName= new QLineEdit(Plant::activePlant->name);
    this->addRow("Name:",txtName);
    spinAge = new QSpinBox();
    spinAge->setMinimum(0);
    spinAge->setMaximum(INT_MAX);
    spinAge->setValue(Plant::activePlant->maxAge);

    QPushButton *applyMaxAge = new QPushButton("Apply");
    QHBoxLayout *lyoMaxAge = new QHBoxLayout();
    lyoMaxAge->addWidget(spinAge,1);
    lyoMaxAge->addWidget(applyMaxAge);
    this->addRow("Maximum age:",lyoMaxAge);
    spinSeed = new QSpinBox();
    spinSeed->setMaximum(INT_MAX);
    spinSeed->setValue(Plant::activePlant->seed);
    btnRandomSeed = new QPushButton("Random");
    QHBoxLayout *lyoSeed = new QHBoxLayout();
    lyoSeed->addWidget(spinSeed,1);
    lyoSeed->addWidget(btnRandomSeed);
    this->addRow("Seed:",lyoSeed);

    clrPrimLeafColor = new ColorLabel("  ",0);
    clrPrimLeafColor->setAutoFillBackground(true);
    QPalette palette = clrPrimLeafColor->palette();
    palette.setColor(palette.Background,Plant::activePlant->primLeafColor);
    clrPrimLeafColor->setPalette(palette);
    //this->addRow("Primary leaf color",clrPrimLeafColor);

    clrSecLeafColor = new ColorLabel("  ",0);
    clrSecLeafColor->setAutoFillBackground(true);
    palette = clrSecLeafColor->palette();
    palette.setColor(palette.Background,Plant::activePlant->secLeafColor);
    clrSecLeafColor->setPalette(palette);
    //this->addRow("Secondary leaf color",clrSecLeafColor);

    QHBoxLayout *lyoLeafColor = new QHBoxLayout();
    lyoLeafColor->addWidget(clrPrimLeafColor);
    lyoLeafColor->addWidget(clrSecLeafColor);
    this->addRow("Leaf colors", lyoLeafColor);

    clrTreeColor = new ColorLabel("  ",0);
    clrTreeColor->setAutoFillBackground(true);
    palette = clrTreeColor->palette();
    palette.setColor(palette.Background,Plant::activePlant->branchColor);
    clrTreeColor->setPalette(palette);
    this->addRow("Branch color",clrTreeColor);

    clrBackgroundColor = new ColorLabel("  ",0);
    clrBackgroundColor->setAutoFillBackground(true);
    palette = clrBackgroundColor->palette();
    palette.setColor(palette.Background,Plant::activePlant->backgroundColor);
    clrBackgroundColor->setPalette(palette);
    this->addRow("Background color", clrBackgroundColor);

    cbxLeaves = new QCheckBox("Draw leaves");
    cbxLeaves->setChecked(Plant::activePlant->drawLeaves);
    this->addRow("",cbxLeaves);

    cbxBranchCaps = new QCheckBox("Draw branch caps");
    cbxBranchCaps->setChecked(Plant::activePlant->drawCaps);
    this->addRow("",cbxBranchCaps);

    cbxConnectors = new QCheckBox("Draw connectors");
    cbxConnectors->setChecked(Plant::activePlant->drawConnectors);
    this->addRow("",cbxConnectors);

    spinSlices = new QSpinBox();
    spinSlices->setValue(Plant::activePlant->slices);
    this->addRow("Slices",spinSlices);

    spinSegments = new QSpinBox();
    spinSegments->setValue(Plant::activePlant->segments);
    this->addRow("Segments",spinSegments);

    spinXMove = new QSpinBox();
    spinXMove->setMaximum(INT_MAX);
    spinXMove->setMinimum(INT_MIN);
    spinXMove->setValue(Plant::activePlant->movement.x());
    // not editable, reacts to mouse movement
    spinXMove->setEnabled(false);
    this->addRow("Movement X",spinXMove);

    spinYMove = new QSpinBox();
    spinYMove->setMaximum(INT_MAX);
    spinYMove->setMinimum(INT_MIN);
    spinYMove->setEnabled(false);
    spinYMove->setValue(Plant::activePlant->movement.y());
    this->addRow("Movement Y",spinYMove);

    spinZoom = new QSpinBox();
    spinZoom->setMaximum(INT_MAX);
    spinZoom->setMinimum(INT_MIN);
    spinZoom->setValue(Plant::activePlant->movement.z());
    spinZoom->setEnabled(false);
    this->addRow("Zoom",spinZoom);

    QHBoxLayout *lyoGrowth = new QHBoxLayout();
    spinGrowth = new QSpinBox();
    spinGrowth->setMinimum(1);
    spinGrowth->setMaximum(Plant::activePlant->maxAge);
    spinGrowth->setValue(Plant::activePlant->growthAge);
    cbxGrowth = new QCheckBox("animate");
    lyoGrowth->addWidget(spinGrowth);
    lyoGrowth->addWidget(cbxGrowth);
    this->addRow("Grow until age of", lyoGrowth);

    Scene *s = Scene::activeScene;
    lblBranches = new QLabel(QString::number(s->branches));
    lblSpheres = new QLabel(QString::number(s->spheres));
    lblLeaves =  new QLabel(QString::number(s->leaves));
    lblPolygons = new QLabel(QString::number((s->spheres + s->branches) * Plant::activePlant->segments * Plant::activePlant->slices));
    lblTriangles = new QLabel(QString::number(s->leaves * 5));
    this->addRow(" ", new QWidget);
    this->addRow("Statistics:", new QWidget);
    this->addRow("Number of branches:", lblBranches);
    this->addRow("Number of spheres:", lblSpheres);
    this->addRow("Number of leaves:", lblLeaves);
    this->addRow("Number of polygons:", lblPolygons);
    this->addRow("Number of triangles:", lblTriangles);

    connect(txtName,SIGNAL(textChanged(QString)),this,SLOT(changeName(QString)));
    connect(spinSeed,SIGNAL(valueChanged(int)),this,SLOT(changeSeed(int)));
    connect(btnRandomSeed,SIGNAL(clicked()),this,SLOT(randomSeed()));
    connect(applyMaxAge,SIGNAL(clicked()),this,SLOT(changeMaxAge()));
    connect(clrPrimLeafColor,SIGNAL(clicked()),this,SLOT(changePrimLeafColor()));
    connect(clrSecLeafColor,SIGNAL(clicked()),this,SLOT(changeSecLeafColor()));
    connect(clrBackgroundColor,SIGNAL(clicked()),this,SLOT(changeBackgroundColor()));
    connect(clrTreeColor,SIGNAL(clicked()),this,SLOT(changeBranchColor()));
    connect(cbxLeaves,SIGNAL(toggled(bool)),this,SLOT(switchLeaves(bool)));
    connect(cbxBranchCaps,SIGNAL(toggled(bool)),this,SLOT(switchBranchCaps(bool)));
    connect(cbxConnectors,SIGNAL(toggled(bool)),this,SLOT(switchConnectors(bool)));
    connect(spinSlices,SIGNAL(valueChanged(int)),this,SLOT(changeSlices(int)));
    connect(spinSegments,SIGNAL(valueChanged(int)),this,SLOT(changeSegments(int)));
    connect(spinGrowth,SIGNAL(valueChanged(int)),this,SLOT(changeGrowthAge(int)));
    connect(cbxGrowth,SIGNAL(toggled(bool)),this,SLOT(animateGrowth(bool)));

    connect(btnRandomSeed, SIGNAL(clicked()), this, SLOT(updateStats()));
//    connect(spinAge, SIGNAL(valueChanged(int)), this, SLOT(updateStats()));
    connect(cbxLeaves, SIGNAL(toggled(bool)), this, SLOT(updateStats()));
    connect(cbxBranchCaps, SIGNAL(toggled(bool)), this, SLOT(updateStats()));
    connect(cbxConnectors, SIGNAL(toggled(bool)), this, SLOT(updateStats()));
    connect(spinSlices, SIGNAL(valueChanged(int)), this, SLOT(updateStats()));
    connect(spinSegments, SIGNAL(valueChanged(int)), this, SLOT(updateStats()));
    //connect(spinGrowth, SIGNAL(valueChanged(int)), this, SLOT(updateStats()));
}

void OptionsFormLayout::randomSeed()
{
    spinSeed->setValue(qrand());
    // no need to call redraw because spinner emits change signal
}

void OptionsFormLayout::changeMaxAge()
{
    Plant::activePlant->scaleMaxAgeTo(spinAge->value());
    Scene::activeScene->initScene();
    emit treeRescaled();
}

void OptionsFormLayout::changePrimLeafColor()
{
    QPalette p = clrPrimLeafColor->palette();
    QColor color = QColorDialog::getColor(Plant::activePlant->primLeafColor);
    if (color.isValid()) {
        p.setColor(p.Background,color);
        clrPrimLeafColor->setPalette(p);
        Plant::activePlant->primLeafColor = color;
        Scene::activeScene->initScene();
    }
}

void OptionsFormLayout::changeSecLeafColor()
{
    QPalette p = clrSecLeafColor->palette();
    QColor color = QColorDialog::getColor(Plant::activePlant->secLeafColor);
    if (color.isValid()) {
        p.setColor(p.Background,color);
        clrSecLeafColor->setPalette(p);
        Plant::activePlant->secLeafColor = color;
        Scene::activeScene->initScene();
    }
}

void OptionsFormLayout::changeBranchColor()
{
    QPalette p = clrTreeColor->palette();
    QColor color = QColorDialog::getColor(Plant::activePlant->branchColor);
    if (color.isValid()) {
        p.setColor(p.Background,color);
        clrTreeColor->setPalette(p);
        Plant::activePlant->branchColor = color;
        Scene::activeScene->initScene();
    }
}

void OptionsFormLayout::changeBackgroundColor()
{
    QPalette p = clrBackgroundColor->palette();
    QColor color = QColorDialog::getColor(Plant::activePlant->backgroundColor);
    if (color.isValid()) {
        p.setColor(p.Background,color);
        clrBackgroundColor->setPalette(p);
        Plant::activePlant->backgroundColor = color;
        Scene::activeScene->initScene();
    }
}

void OptionsFormLayout::changeSlices(int slices)
{
    Plant::activePlant->slices = slices;
    Scene::activeScene->initScene();
}

void OptionsFormLayout::changeSegments(int segments)
{
    Plant::activePlant->segments = segments;
    Scene::activeScene->initScene();
}

void OptionsFormLayout::switchLeaves(bool toggle)
{
    Plant::activePlant->drawLeaves=toggle;
    Scene::activeScene->initScene();
}

void OptionsFormLayout::switchBranchCaps(bool toggle)
{
    Plant::activePlant->drawCaps=toggle;
    Scene::activeScene->initScene();
}

void OptionsFormLayout::switchConnectors(bool toggle)
{
    Plant::activePlant->drawConnectors=toggle;
    Scene::activeScene->initScene();
}

void OptionsFormLayout::changeGrowthAge(int age)
{
    Plant::activePlant->growthAge = age;
    Scene::activeScene->update();
    updateStats();
}

void OptionsFormLayout::animateGrowth(bool toggle)
{
    if (toggle) {
        spinGrowth->setEnabled(false);
        timer = new QTimer();
        connect(timer,SIGNAL(timeout()),this,SLOT(doAnimation()));
        timer->start(100);
    } else {
        timer->stop();
        spinGrowth->setEnabled(true);
    }
}

void OptionsFormLayout::updateStats()
{
    lblBranches->setText(QString::number(Scene::activeScene->branches));
    lblSpheres->setText(QString::number(Scene::activeScene->spheres));
    lblLeaves->setText(QString::number(Scene::activeScene->leaves));
    lblPolygons->setText(QString::number((Scene::activeScene->spheres +
                                          Scene::activeScene->branches) *
                                         Plant::activePlant->segments *
                                         Plant::activePlant->slices));
    lblTriangles->setText(QString::number(Scene::activeScene->leaves * 5));
}

void OptionsFormLayout::changeName(QString name)
{
    Plant::activePlant->name = name;
}

void OptionsFormLayout::changeSeed(int seed)
{
    Plant::activePlant->seed = seed;
    Scene::activeScene->initScene();
}


// derived a QLabel to make it clickable

ColorLabel::ColorLabel( const QString & text, QWidget * parent )
:QLabel(text,parent)
{
}

void ColorLabel::mousePressEvent ( QMouseEvent * event )
{
    emit clicked();
}

void OptionsFormLayout::doAnimation()
{
    int age = spinGrowth->value();

    // delay animation at maximum growth
    if (age == spinGrowth->maximum() && growthDelayCounter < growthDelay) {
        growthDelayCounter++;
        return;
    }

    growthDelayCounter = 0;

    if (age == spinGrowth->minimum()) {
        growUpwards = true;
    }

    if (age == spinGrowth->maximum()) {
        growUpwards = false;
    }

    if (growUpwards) {
        age++;
    } else {
        age--;
    }

    spinGrowth->setValue(age);
}
