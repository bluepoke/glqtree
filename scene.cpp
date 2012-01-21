#include "scene.h"

Scene* Scene::activeScene = 0;

SceneObject::SceneObject(SceneObject *parent) : parent(parent)
{
    children = new QList<SceneObject*>;
    rotation = new QVector3D;
    translation = new QVector3D;
    isRendered = true;
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
    if (isRendered) {
        GLUquadricObj *qobj;
        qobj = gluNewQuadric();

        //    not really needed
        //    gluQuadricCallback(qobj, GLU_ERROR, NULL);
        //    gluQuadricDrawStyle(qobj, GLU_FILL);
        //    gluQuadricNormals(qobj, GLU_SMOOTH);

        // build all along the positive z-axis
        QColor branch = Plant::activePlant->branchColor;
        glColor3f(branch.redF(), branch.greenF(), branch.blueF());
        gluCylinder(qobj, radBottom, radTop, length,
                    Plant::activePlant->slices, Plant::activePlant->segments);
    }
}

void EndSection::render()
{
    if (isRendered) {
        GLUquadricObj *qobj;
        qobj = gluNewQuadric();
        // build an end cap
        QColor branch = Plant::activePlant->branchColor;
        glColor3f(branch.redF(), branch.greenF(), branch.blueF());
        gluSphere(qobj, radius,
                  Plant::activePlant->slices, Plant::activePlant->segments);
    }
}

void Leaf::render()
{
    if (isRendered) {
        QColor prim = Plant::activePlant->primLeafColor;
        QColor sec = Plant::activePlant->secLeafColor;
        glFrontFace(GL_CW);

        // left half of leaf
        glBegin(GL_TRIANGLE_STRIP);
        glColor3f(prim.redF(), prim.greenF(), prim.blueF());
        glVertex3f(-width/2, 0.0f, 0.25*length);
        glColor3f(sec.redF(), sec.greenF(), sec.blueF());
        glVertex3f(-width/2, 0.0f, 0.75*length);
        glColor3f(prim.redF(), prim.greenF(), prim.blueF());
        glVertex3f(0.0f, 0.0f, 0.0f);
        glColor3f(sec.redF(), sec.greenF(), sec.blueF());
        glVertex3f(0.0f, 0.0f, length);
        glEnd();

        // right half of leaf
        glBegin(GL_TRIANGLE_STRIP);
        glColor3f(prim.redF(), prim.greenF(), prim.blueF());
        glVertex3f(width/2, 0.0f, 0.25*length);
        glColor3f(prim.redF(), prim.greenF(), prim.blueF());
        glVertex3f(0.0f, 0.0f, 0.0f);
        glColor3f(sec.redF(), sec.greenF(), sec.blueF());
        glVertex3f(width/2, 0.0f, 0.75*length);
        glColor3f(sec.redF(), sec.greenF(), sec.blueF());
        glVertex3f(0.0f, 0.0f, length);
        glEnd();
    }
}

Scene::Scene(Plant *plant, QWidget *oglPanel) : oglPanel(oglPanel) {
    initScene(plant);
}

void Scene::initScene(Plant *plant) {
    // no plant: no scene root
    if (plant == 0) {
        root = 0;
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
        if (plant->drawCaps) {
            SceneObject *cap = constructEndSection(plant, parent, age, plant->drawLeaves);
            // do not render if age is larger than deserved
            if (age > plant->growthAge) {
                cap->isRendered=false;
            }
            end->append(cap);
        }
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
            // do not render if age is larger than deserved growth
            if (age > plant->growthAge) {
                branch->isRendered = false;
            }
            // apply off-the-divided-axis rotation to branch:
            int randRotationAngle = plant->coinflip() * plant->getBranchingRotationAt(branchAge);
            branch->rotation = new QVector3D(0, plant->getBranchingAngle(branchAge),
                                            360/actualBranches * i + randRotationAngle);

            // let the branch grow further with possible interruption (recursion)
            QList<SceneObject*> *nextBranchChildren = createSceneObject(plant, branch, branchAge + 1);
            // append the next branches' children to this branch
            branch->children->append(*nextBranchChildren);

            // create leaves for this branch if any
            if (plant->drawLeaves)
            {
                QList<SceneObject*> *leaves = createLeaves(plant, branch, branchAge);
                branch->children->append(*leaves);
            }

            // append this branch to the list of children
            children->append(branch);
        }
    }

    // continue main branch if needed
    if (plant->continueMainBranchAt(age)) {
        SceneObject *mainBranch = constructBranchSection(plant, parent, ((BranchSection*)parent)->radTop, age);
        // do not render if age is larger than deserved growth
        if (age > plant->growthAge) {
            mainBranch->isRendered = false;
        }
        // create all possible next children of the continued main branch
        QList<SceneObject*> *nextChildren = createSceneObject(plant, mainBranch, age + 1);
        mainBranch->children->append(*nextChildren);

        // create leaves for the mainbranch if any
        if (plant->drawLeaves)
        {
            QList<SceneObject*> *leaves = createLeaves(plant, mainBranch, age);
            mainBranch->children->append(*leaves);
        }

        // append the continued main branch to the parent's children list
        children->append(mainBranch);

        // put a cap to the branches end to smooth string wobbliness and branching angles
        if (plant->drawConnectors) {
            SceneObject *cap = constructEndSection(plant, parent, age, false);
            children->append(cap);
        }
    }
    // append only an end cap otherwise
    else {
        if (plant->drawCaps) {
            SceneObject *current = constructEndSection(plant, parent, age, plant->drawLeaves);
            children->append(current);
        }
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

SceneObject* Scene::constructEndSection(Plant *plant, SceneObject* parent, int age, bool hasLeaf)
{
    // add a cap to a parent
    SceneObject *current = new EndSection(parent, ((BranchSection*)parent)->radTop);
    current->translation = new QVector3D(0, 0, ((BranchSection*)parent)->length);

    if (age>Plant::activePlant->growthAge) {
        current->isRendered=false;
    }

    // if the cap should sport a single leaf at its top (like at the end of branches, do so
    if (hasLeaf) {
        // move translation out of the object itself, to make leaves universally usable
        QVector3D *translation = new QVector3D(0, 0, ((EndSection*)current)->radius);
        SceneObject *leaf = constructLeaf(plant, current, age, translation);
        current->children->append(leaf);
    }

    return current;
}

QList<SceneObject*> *Scene::createLeaves(Plant *plant, SceneObject *parent, int age)
{
    QList<SceneObject*> *leaves = new QList<SceneObject*>;

    // TODO, create leaves

    return leaves;
}

SceneObject* Scene::constructLeaf(Plant *plant, SceneObject* parent, int age, QVector3D *translation)
{
    // construct and return a leaf
    SceneObject *leaf = new Leaf(parent, plant->getLeafWidthAt(age), plant->getLeafLengthAt(age));
    if (age>Plant::activePlant->growthAge) {
        leaf->isRendered=false;
    }
    leaf->translation = translation;
    return leaf;
}


