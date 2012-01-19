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
//    gluSphere(qobj, radTop, 35, 15);
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

    // reset seed before drawing
    plant->reseed();

    // create a root starting at age 0
    SceneObject *root;
    int minAge = 0;

    root = new BranchSection(0, plant->getBranchThicknessAt(minAge),
                             plant->getBranchThicknessAt(minAge + 1),
                             plant->getBranchLengthAt(minAge));
    // try creating the successor
    QList<SceneObject*> *children = createSceneObject(plant, root, minAge + 1);
    // append all children
    root->children->append(*children);

    // set root for scene
    Scene::root = root;
    // call redraw
    ((QGLWidget*)oglPanel)->update();
}

QList<SceneObject*> *Scene::createSceneObject(Plant *plant, SceneObject *parent, int age)
{
    if (age >= plant->maxAge) return new QList<SceneObject*>;

    QList<SceneObject*> *children = new QList<SceneObject*>;
    // create SceneObjects for all possible children

    // is tree branching here?
    bool isBranching = plant->isBranchingAt(age);
    // how many branches
    int branchCount = plant->getBranchingAt(age);

    if (isBranching) {
        for (int i=0; i<branchCount; i++) {
            // is growth interrupted
            int branchAge = age;
            bool isGrowthInterrupted = plant->isGrowthInterruptingAt(age);
            int delay = plant->getGrowthInterruptionAt(age);
            if (isGrowthInterrupted) {
                // growth is delayed. that means, following objects
                branchAge += delay;
            }
            // create new branch
            SceneObject *branch = constructBranchSection(plant,parent,branchAge);
            // apply rotation to branch
            QVector3D *rotB;
            if (plant->coinflip())
            rotB = new QVector3D(0, plant->getBranchingAngle(branchAge),
                                 (360/branchCount*i)+plant->getBranchingRotationAt(branchAge));
            else
                rotB = new QVector3D(plant->getBranchingAngle(branchAge), 0,
                                     (360/branchCount*i)+plant->getBranchingRotationAt(branchAge));

            *(branch->rotation) += *rotB;
            // let the branch grow further with possible interruption (recursion)
            QList<SceneObject*> *nextBranchChildren = createSceneObject(plant, branch, branchAge+1);
            // append the children to this branch
            branch->children->append(*nextBranchChildren);
            // append this branch to the list of children
            children->append(branch);
        }
    }
    // main branch
    // is branch growing further? it should if there are less than 2 branches
    if (!isBranching || (branchCount<2) || (plant->continueMainBranchAt(age))) {
        SceneObject *current = constructBranchSection(plant,parent,age);

        // create all possible next children of each child
        QList<SceneObject*> *nextChildren = createSceneObject(plant, current, age + 1);
        current->children->append(*nextChildren);

        // append all possible children to the parent's children list
        children->append(current);
    }
    return children;
}

SceneObject* Scene::constructBranchSection(Plant* plant, SceneObject* parent, int age) {
    SceneObject *current = new BranchSection(parent,
                                             ((BranchSection*)parent)->radTop,
                                             plant->getBranchThicknessAt(age + 1),
                                             plant->getBranchLengthAt(age));

    // apply wobbliness
    if (plant->isBranchWobblinessAt(age)) {
        QVector3D *wobble;
        if (plant->coinflip())
            wobble = new QVector3D(0, plant->getBranchWobblinessAt(age),
                                   plant->getRandomRotation360());
        else
            wobble = new QVector3D(plant->getBranchWobblinessAt(age), 0,
                                   plant->getRandomRotation360());

        current->rotation = wobble;
    }
    current->translation = new QVector3D(0, 0, ((BranchSection*)parent)->length);
    return current;
}
