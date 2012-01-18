#include "scene.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QDebug>
#include <QGLWidget>

Scene* Scene::activeScene = 0;

SceneObject::SceneObject(SceneObject *parent) : parent(parent)
{
    children = new QList<SceneObject*>;
    rotation = new QVector3D;
    translation = new QVector3D;
}

BranchSection::BranchSection(SceneObject *parent, double radBottom, double radTop, double length) :
    SceneObject(parent), radBottom(radBottom), radTop(radTop), length(length)
{
}

void BranchSection::render()
{
    GLUquadricObj *qobj;
    qobj = gluNewQuadric();

//    gluQuadricCallback(qobj, GLU_ERROR, NULL);
//    gluQuadricDrawStyle(qobj, GLU_FILL);
//    gluQuadricNormals(qobj, GLU_SMOOTH);

    // build all along the positive z-axis
    gluCylinder(qobj, radBottom, radTop, length, 15, 5);

//    qobj = gluNewQuadric();
//    glPushMatrix();
//    glTranslatef(0, 0, length);
//    gluSphere(qobj, radTop, 15, 5);
//    glPopMatrix();

}


Scene::Scene(Plant *plant, QWidget *oglPanel) : oglPanel(oglPanel) {
    initScene(plant);
}

void Scene::initScene(Plant *plant) {
    // no plant: no scene root
    if (plant == 0) {
        Scene::root = 0;
        return;
    }

    // TODO load from static plant object here
//    qDebug() << "\n" << plant->name;
//    qDebug() << plant->getBranchThicknessAt(0);
//    qDebug() << plant->getBranchThicknessAt(1);
//    qDebug() << plant->getBranchLengthAt(0);

    SceneObject *root;
    root = new BranchSection(0, plant->getBranchThicknessAt(0),
                             plant->getBranchThicknessAt(1),
                             plant->getBranchLengthAt(0));

//    // define a temp root object
//    SceneObject *root;
//    root = new BranchSection(0, 0.5, 0.3, 1);
//    root->rotation = new QVector3D(0, 25, 0);
//    root->translation = new QVector3D(-2, 0, 0);

//    // example child nodes
//    SceneObject *child;
//    child = new BranchSection(root, 0.3, 0.2, 1.5);
//    child->rotation = new QVector3D(-25, 15, 0);
//    child->translation = new QVector3D(0, 0, ((BranchSection*)root)->length);
//    root->children->append(child);

//    child = new BranchSection(root, 0.3, 0.05, 2);
//    child->rotation = new QVector3D(15, -35, 0);
//    child->translation = new QVector3D(0, 0, ((BranchSection*)root)->length);
//    root->children->append(child);

    // set root for scene
    Scene::root = root;
    // call redraw
    ((QGLWidget*)oglPanel)->update();
}
