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

EndSection::EndSection(SceneObject *parent, double radius) : SceneObject(parent), radius(radius)
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
    gluCylinder(qobj, radBottom, radTop, length, 5, 3);

    qobj = gluNewQuadric();
}

void EndSection::render()
{
    GLUquadricObj *qobj;
    qobj = gluNewQuadric();
    // build an end cap
    gluSphere(qobj, radius, 5, 3);
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
    // end of recursion, max age reached
    if (age >= plant->maxAge) return new QList<SceneObject*>;

    // will hold all possible branches of this age in addition to the main branch (if necessary)
    QList<SceneObject*> *children = new QList<SceneObject*>;

    // is tree branching here?
    bool isBranchingAtAll = plant->isBranchingAt(age);
    int possibleBranches = plant->getBranchCountAt(age);
    int actualBranches = possibleBranches;

    if (isBranchingAtAll) {
        for (int i=0; i < possibleBranches; i++) {
            // should a possible branch actually grow?
            if (!plant->isBranchingAt(age)) {
                actualBranches--;
                break;
            }
            // create new branch
            int branchAge = age;
            bool isGrowthInterrupted = plant->isGrowthInterruptingAt(age);
            int delay = plant->getGrowthInterruptionAt(age);
            int parentRadius = ((BranchSection*)parent)->radTop;
            if (isGrowthInterrupted) {
                // growth is delayed. all following objects start with higher age
                branchAge += delay;
                parentRadius = plant->getBranchThicknessAt(branchAge - 1);
            }
            SceneObject *branch = constructBranchSection(plant, parent, parentRadius, branchAge);
            // apply off-the-divided-axis rotation to branch:
            int randRotationAngle = plant->coinflip() * plant->getBranchingRotationAt(branchAge);
            QVector3D *rotB = new QVector3D(0, plant->getBranchingAngle(branchAge),
                                            360/actualBranches * i + randRotationAngle);
            branch->rotation = rotB;

            // let the branch grow further with possible interruption (recursion)
            QList<SceneObject*> *nextBranchChildren = createSceneObject(plant, branch, branchAge + 1);
            // append the children to this branch
            branch->children->append(*nextBranchChildren);
            // append this branch to the list of children
            children->append(branch);
        }
    }

    // continue main branch if needed
    if (plant->continueMainBranchAt(age)) {
        SceneObject *current = constructBranchSection(plant, parent, ((BranchSection*)parent)->radTop, age);

        // create all possible next children of the continued main branch
        QList<SceneObject*> *nextChildren = createSceneObject(plant, current, age + 1);
        current->children->append(*nextChildren);

        // append the main branch to the parent's children list
        children->append(current);
    }
    // append an end cap otherwise
    else {
        SceneObject *current = constructEndSection(parent, age);
        children->append(current);
    }
    return children;
}

SceneObject* Scene::constructBranchSection(Plant* plant, SceneObject* parent, int parentRadius, int age)
{
    SceneObject *current = new BranchSection(parent, parentRadius,
                                             plant->getBranchThicknessAt(age + 1),
                                             plant->getBranchLengthAt(age));

    // apply wobbliness
    if (plant->isBranchWobblinessAt(age)) {
        QVector3D *wobble = new QVector3D(plant->getBranchWobblinessAt(age), 0,
                                          plant->getRandomRotationBetween(1, 360));

        current->rotation = wobble;
    }
    current->translation = new QVector3D(0, 0, ((BranchSection*)parent)->length);
    return current;
}

SceneObject* Scene::constructEndSection(SceneObject* parent, int age)
{
    SceneObject *current = new EndSection(parent, ((BranchSection*)parent)->radTop);
    current->translation = new QVector3D(0, 0, ((BranchSection*)parent)->length);
    return current;
}


