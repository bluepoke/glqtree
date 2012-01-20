#include "scene.h"

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

Leaf::Leaf(SceneObject *parent, double width, double length) : SceneObject(parent),
    width(width), length(length)
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
    glColor3f(0, 0, 1);
    gluCylinder(qobj, radBottom, radTop, length, 5, 3);
}

void EndSection::render()
{
    GLUquadricObj *qobj;
    qobj = gluNewQuadric();
    // build an end cap
    glColor3f(1, 0, 0);
    gluSphere(qobj, radius, 5, 3);
}

void Leaf::render()
{
    glFrontFace(GL_CW);
    glColor3f(0, 1, 0);

    // left half of leaf
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1, 1, 0);
    glVertex3f(-width/2, 0.0f, 0.25*length);
    glColor3f(0, 1, 0);
    glVertex3f(-width/2, 0.0f, 0.75*length);
    glColor3f(1, 1, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(0, 1, 0);
    glVertex3f(0.0f, 0.0f, length);
    glEnd();

    // right half of leaf
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1, 1, 0);
    glVertex3f(width/2, 0.0f, 0.25*length);
    glColor3f(1, 1, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(0, 1, 0);
    glVertex3f(width/2, 0.0f, 0.75*length);
    glColor3f(0, 1, 0);
    glVertex3f(0.0f, 0.0f, length);
    glEnd();
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
    if (age > plant->maxAge) return new QList<SceneObject*>;
    // apply an endcap to top branches
    if (age == plant->maxAge) {
        QList<SceneObject*> *end = new QList<SceneObject*>;
        SceneObject *cap = constructEndSection(parent, age, true);
        end->append(cap);
        return end;
    }

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
        // put a cap to the branches end to smooth string wobbliness and branching angles
        SceneObject *cap = constructEndSection(parent, age, false);
        children->append(cap);
    }
    // append only an end cap otherwise
    else {
        SceneObject *current = constructEndSection(parent, age, true);
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

SceneObject* Scene::constructEndSection(SceneObject* parent, int age, bool hasLeaf)
{
    // add a cap to a parent
    SceneObject *current = new EndSection(parent, ((BranchSection*)parent)->radTop);
    current->translation = new QVector3D(0, 0, ((BranchSection*)parent)->length);

    // if the cap should sport a single leaf at its top (like at the end of branches, do so
    if (hasLeaf) {
        // move translation out of the object itself, to make leaves universally usable
        QVector3D *translation = new QVector3D(0, 0, ((EndSection*)current)->radius);
        SceneObject *leaf = constructLeaf(current, age, translation);
        current->children->append(leaf);
    }

    return current;
}

SceneObject* Scene::constructLeaf(SceneObject* parent, int age, QVector3D *translation)
{
    // construct and return a leaf
    SceneObject *leaf = new Leaf(parent, Plant::activePlant->getLeafWidthAt(age), Plant::activePlant->getLeafLengthAt(age));
    leaf->translation = translation;
    return leaf;
}


