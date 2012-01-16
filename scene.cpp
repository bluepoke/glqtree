#include "scene.h"
#include <GL/gl.h>
#include <GL/glu.h>

SceneObject::SceneObject(SceneObject *parent) : parent(parent)
{
    children = new QList<SceneObject>;
    rotationAxis = new QVector3D;
    rotationAngles = new QVector3D;
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

    // centered on the y-axis for now
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, length/-2.0);
    gluCylinder(qobj, radBottom, radTop, length, 15, 5);
}
