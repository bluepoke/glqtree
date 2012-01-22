#include "optionsformlayout.h"
#include "tabbedoptionsdialog.h"
#include <QDebug>

OptionsFormLayout::OptionsFormLayout(QWidget *parent) :
    QFormLayout(parent)
{
    Plant *p = Plant::activePlant;
    txtName= new QLineEdit(p->name);
    this->addRow("Name:",txtName);
    spinAge = new QSpinBox();
    spinAge->setMinimum(0);
    spinAge->setMaximum(INT_MAX);
    spinAge->setValue(p->maxAge);

    QPushButton *applyMaxAge = new QPushButton("Apply");
    QHBoxLayout *lyoMaxAge = new QHBoxLayout();
    lyoMaxAge->addWidget(spinAge,1);
    lyoMaxAge->addWidget(applyMaxAge);
    this->addRow("Maximum age:",lyoMaxAge);
    spinSeed = new QSpinBox();
    spinSeed->setMaximum(INT_MAX);
    spinSeed->setValue(p->seed);
    btnRandomSeed = new QPushButton("Random");
    QHBoxLayout *lyoSeed = new QHBoxLayout();
    lyoSeed->addWidget(spinSeed,1);
    lyoSeed->addWidget(btnRandomSeed);
    this->addRow("Seed:",lyoSeed);

    clrPrimLeafColor = new ColorLabel("  ",0);
    clrPrimLeafColor->setAutoFillBackground(true);
    QPalette palette = clrPrimLeafColor->palette();
    palette.setColor(palette.Background,p->primLeafColor);
    clrPrimLeafColor->setPalette(palette);
    //this->addRow("Primary leaf color",clrPrimLeafColor);

    clrSecLeafColor = new ColorLabel("  ",0);
    clrSecLeafColor->setAutoFillBackground(true);
    palette = clrSecLeafColor->palette();
    palette.setColor(palette.Background,p->secLeafColor);
    clrSecLeafColor->setPalette(palette);
    //this->addRow("Secondary leaf color",clrSecLeafColor);

    QHBoxLayout *lyoLeafColor = new QHBoxLayout();
    lyoLeafColor->addWidget(clrPrimLeafColor);
    lyoLeafColor->addWidget(clrSecLeafColor);
    this->addRow("Leaf colors", lyoLeafColor);

    clrTreeColor = new ColorLabel("  ",0);
    clrTreeColor->setAutoFillBackground(true);
    palette = clrTreeColor->palette();
    palette.setColor(palette.Background,p->branchColor);
    clrTreeColor->setPalette(palette);
    this->addRow("Branch color",clrTreeColor);

    cbxLeaves = new QCheckBox("Draw leaves");
    cbxLeaves->setChecked(p->drawLeaves);
    this->addRow("",cbxLeaves);

    cbxBranchCaps = new QCheckBox("Draw branch caps");
    cbxBranchCaps->setChecked(p->drawCaps);
    this->addRow("",cbxBranchCaps);

    cbxConnectors = new QCheckBox("Draw connectors");
    cbxConnectors->setChecked(p->drawConnectors);
    this->addRow("",cbxConnectors);

    spinSlices = new QSpinBox();
    spinSlices->setValue(p->slices);
    this->addRow("Slices",spinSlices);

    spinSegments = new QSpinBox();
    spinSegments->setValue(p->segments);
    this->addRow("Segments",spinSegments);

    spinXMove = new QSpinBox();
    spinXMove->setMaximum(INT_MAX);
    spinXMove->setMinimum(INT_MIN);
    spinXMove->setValue(p->movement.x());
    // not editable, reacts to mouse movement
    spinXMove->setEnabled(false);
    this->addRow("Movement X",spinXMove);

    spinYMove = new QSpinBox();
    spinYMove->setMaximum(INT_MAX);
    spinYMove->setMinimum(INT_MIN);
    spinYMove->setEnabled(false);
    spinYMove->setValue(p->movement.y());
    this->addRow("Movement Y",spinYMove);

    spinZoom = new QSpinBox();
    spinZoom->setMaximum(INT_MAX);
    spinZoom->setMinimum(INT_MIN);
    spinZoom->setValue(p->movement.z());
    spinZoom->setEnabled(false);
    this->addRow("Zoom",spinZoom);

    spinGrowth = new QSpinBox();
    spinGrowth->setMinimum(1);
    spinGrowth->setMaximum(p->maxAge);
    spinGrowth->setValue(p->growthAge);
    this->addRow("Grow until age of", spinGrowth);

    Scene *s = Scene::activeScene;
    lblBranches = new QLabel(QString::number(s->branches));
    lblSpheres = new QLabel(QString::number(s->spheres));
    lblLeaves =  new QLabel(QString::number(s->leaves));
    lblPolygons = new QLabel(QString::number((s->spheres + s->branches) * p->segments * p->slices));
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
    connect(clrTreeColor,SIGNAL(clicked()),this,SLOT(changeBranchColor()));
    connect(cbxLeaves,SIGNAL(toggled(bool)),this,SLOT(switchLeaves(bool)));
    connect(cbxBranchCaps,SIGNAL(toggled(bool)),this,SLOT(switchBranchCaps(bool)));
    connect(cbxConnectors,SIGNAL(toggled(bool)),this,SLOT(switchConnectors(bool)));
    connect(spinSlices,SIGNAL(valueChanged(int)),this,SLOT(changeSlices(int)));
    connect(spinSegments,SIGNAL(valueChanged(int)),this,SLOT(changeSegments(int)));
    connect(spinGrowth,SIGNAL(valueChanged(int)),this,SLOT(changeGrowthAge(int)));

    connect(btnRandomSeed, SIGNAL(clicked()), this, SLOT(updateStats()));
    connect(spinAge, SIGNAL(valueChanged(int)), this, SLOT(updateStats()));
    connect(cbxLeaves, SIGNAL(toggled(bool)), this, SLOT(updateStats()));
    connect(cbxBranchCaps, SIGNAL(toggled(bool)), this, SLOT(updateStats()));
    connect(cbxConnectors, SIGNAL(toggled(bool)), this, SLOT(updateStats()));
    connect(spinSlices, SIGNAL(valueChanged(int)), this, SLOT(updateStats()));
    connect(spinSegments, SIGNAL(valueChanged(int)), this, SLOT(updateStats()));
    connect(spinGrowth, SIGNAL(valueChanged(int)), this, SLOT(updateStats()));
}

void OptionsFormLayout::randomSeed()
{
    spinSeed->setValue(qrand());
    // no need to call redraw because spinner emits change signal
}

void OptionsFormLayout::changeMaxAge()
{
    Plant::activePlant->scaleMaxAgeTo(spinAge->value());
    Scene::activeScene->initScene(Plant::activePlant);
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
        Scene::activeScene->initScene(Plant::activePlant);
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
        Scene::activeScene->initScene(Plant::activePlant);
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
        Scene::activeScene->initScene(Plant::activePlant);
    }
}

void OptionsFormLayout::changeSlices(int slices)
{
    Plant::activePlant->slices = slices;
    Scene::activeScene->initScene(Plant::activePlant);
}

void OptionsFormLayout::changeSegments(int segments)
{
    Plant::activePlant->segments = segments;
    Scene::activeScene->initScene(Plant::activePlant);
}

void OptionsFormLayout::switchLeaves(bool toggle)
{
    Plant::activePlant->drawLeaves=toggle;
    Scene::activeScene->initScene(Plant::activePlant);
}

void OptionsFormLayout::switchBranchCaps(bool toggle)
{
    Plant::activePlant->drawCaps=toggle;
    Scene::activeScene->initScene(Plant::activePlant);
}

void OptionsFormLayout::switchConnectors(bool toggle)
{
    Plant::activePlant->drawConnectors=toggle;
    Scene::activeScene->initScene(Plant::activePlant);
}

void OptionsFormLayout::changeGrowthAge(int age)
{
    Plant::activePlant->growthAge = age;
    Scene::activeScene->initScene(Plant::activePlant);
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
    Scene::activeScene->initScene(Plant::activePlant);
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
