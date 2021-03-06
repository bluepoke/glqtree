#include "scene.h"
#include <QDebug>

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
    if (this->age <= Plant::activePlant->growthAge) {
        GLUquadricObj *qobj;
        qobj = gluNewQuadric();
        Scene::activeScene->branches++;

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
    if (this->age <= Plant::activePlant->growthAge) {
        GLUquadricObj *qobj;
        qobj = gluNewQuadric();
        // build an end cap
        QColor branch = Plant::activePlant->branchColor;
        glColor3f(branch.redF(), branch.greenF(), branch.blueF());
        gluSphere(qobj, radius,
                  Plant::activePlant->slices, Plant::activePlant->segments);
        Scene::activeScene->spheres++;
    }
}

void Leaf::render()
{
    if (this->age <= Plant::activePlant->growthAge) {
        QColor prim = Plant::activePlant->primLeafColor;
        QColor sec = Plant::activePlant->secLeafColor;
        glFrontFace(GL_CW);

        int leafStemLength = 0.5f*length;

        // the leaf's stem
        glColor3f(prim.redF(), prim.greenF(), prim.blueF());
        glBegin(GL_TRIANGLE_STRIP);
        glNormal3f(0, 1, 0);
        glVertex3f(-0.1f*width/10, 0.0f, 0.0f);
        glNormal3f(0, 1, 0);
        glVertex3f(0.0f, 0.0f, leafStemLength);
        glNormal3f(0, 1, 0);
        glVertex3f(0.1f*width/10, 0.0f, 0.0f);
        glEnd();

        // left half of leaf
        glBegin(GL_TRIANGLE_STRIP);
        glNormal3f(0, 1, 0);
        glColor3f(prim.redF(), prim.greenF(), prim.blueF());
        glVertex3f(-0.5f*width, 0.0f, leafStemLength+0.25f*length);
        glNormal3f(0, 1, 0);
        glColor3f(sec.redF(), sec.greenF(), sec.blueF());
        glVertex3f(-0.5f*width, 0.0f, leafStemLength+0.75f*length);
        glNormal3f(0, 1, 0);
        glColor3f(prim.redF(), prim.greenF(), prim.blueF());
        glVertex3f(0.0f, 0.0f, leafStemLength);
        glNormal3f(0, 1, 0);
        glColor3f(sec.redF(), sec.greenF(), sec.blueF());
        glVertex3f(0.0f, 0.0f, leafStemLength+length);
        glEnd();

        // right half of leaf
        glBegin(GL_TRIANGLE_STRIP);
        glNormal3f(0, 1, 0);
        glColor3f(prim.redF(), prim.greenF(), prim.blueF());
        glVertex3f(0.5f*width, 0.0f, leafStemLength+0.25f*length);
        glNormal3f(0, 1, 0);
        glColor3f(prim.redF(), prim.greenF(), prim.blueF());
        glVertex3f(0.0f, 0.0f, leafStemLength);
        glNormal3f(0, 1, 0);
        glColor3f(sec.redF(), sec.greenF(), sec.blueF());
        glVertex3f(0.5f*width, 0.0f, leafStemLength+0.75f*length);
        glNormal3f(0, 1, 0);
        glColor3f(sec.redF(), sec.greenF(), sec.blueF());
        glVertex3f(0.0f, 0.0f, leafStemLength+length);
        glEnd();

        Scene::activeScene->leaves++;
    }
}

Scene::Scene(QWidget *oglPanel) : oglPanel(oglPanel) {
    initScene();
}

void Scene::initScene() {
    // no plant: no scene root
    if (Plant::activePlant == 0) {
        root = 0;
        return;
    }

    // reset seed before drawing
    Plant::activePlant->reseed();

    // create a root starting at age 0
    SceneObject *root;
    int minAge = 0;

    root = new BranchSection(0, Plant::activePlant->getBranchThicknessAt(minAge),
                             Plant::activePlant->getBranchThicknessAt(minAge + 1),
                             Plant::activePlant->getBranchLengthAt(minAge));
    // try creating the successor
    QList<SceneObject*> *children = createSceneObject(Plant::activePlant, root, minAge + 1);
    // append all children
    root->children->append(*children);

    // set root for scene
    Scene::root = root;
    // call redraw
    update();
}

void Scene::update() {
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
                continue;
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
                if (branchAge > plant->maxAge) {
                    // if growth interruption results in branches > maxAge
                    // then there is no branch as well
                    actualBranches--;
                    continue;
                }
            }

            SceneObject *branch = constructBranchSection(plant, parent, parentRadius, branchAge);

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
    current->age = age;

//    // do not render if age is larger than deserved growth
//    if (age <= Scene::activeScene->drawUntilAge) {
//        current->isRendered = false;
//    } else {
//        // for statistical value only
//        branches++;
//    }

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
    SceneObject *end = new EndSection(parent, ((BranchSection*)parent)->radTop);
    end->age = age;
    end->translation = new QVector3D(0, 0, ((BranchSection*)parent)->length);

//    // do not render if age is larger than deserved growth
//    if (age > plant->growthAge) {
//        end->isRendered=false;
//    } else {
//        // for statistical value only
//        spheres++;
//    }

    // if the cap should sport a single leaf at its top (like at the end of branches, do so
    if (hasLeaf) {
        // move translation and rotation away from the object itself, to make leaves universally usable
        QVector3D *translation = new QVector3D(0, 0, ((EndSection*)end)->radius);
        QVector3D *rotation = new QVector3D();
        SceneObject *leaf = constructLeaf(plant, end, age, translation, rotation);
        end->children->append(leaf);
    }

    return end;
}

QList<SceneObject*> *Scene::createLeaves(Plant *plant, SceneObject *parent, int age)
{
    QList<SceneObject*> *leaves = new QList<SceneObject*>;

    int levels = plant->getLeafLevelsAt(age);
    int countPerLevel = plant->getLeafCountPerLevelAt(age);
    int angle = plant->getLeafAngleAt(age);

    // no leaves
    if (levels == 0 || countPerLevel == 0) return leaves;

    // base rotation around the branch for each level
    double baseRotation = 360.0 / (double)levels;
    // adjust base rotation by number of leaves per level
    baseRotation = baseRotation / (double)countPerLevel;

    // how many levels
    for (int level=0; level < levels; level++) {
        // rotation for this level
        double levelRotation = 360.0 / countPerLevel;
        // level translation along z-axis
        double zTranslation = ((BranchSection*)parent)->length / (levels + 1) * (level + 1);
        // a base translation towards outside of the branch
        double radius = ((BranchSection*)parent)->radTop;

        // how many per level
        for (int count=0; count < countPerLevel; count++) {
            QVector3D *rotation = new QVector3D(angle, 0, levelRotation * count + baseRotation * level);
            QVector3D *translation = new QVector3D(radius * sin(rotation->z() * PI_180), -radius * cos(rotation->z() * PI_180), zTranslation);
            SceneObject *leaf = constructLeaf(plant, parent, age, translation, rotation);

            leaves->append(leaf);
        }
    }
    // all leaves done
    return leaves;
}

SceneObject* Scene::constructLeaf(Plant *plant, SceneObject* parent, int age, QVector3D *translation, QVector3D *rotation)
{
    // construct and return a leaf
    SceneObject *leaf = new Leaf(parent, plant->getLeafWidthAt(age), plant->getLeafLengthAt(age));
    leaf->age = age;
//    // do not render if age is larger than deserved growth
//    if (age > plant->growthAge) {
//        leaf->isRendered=false;
//     } else {
//        // for statistical value only
//        leaves++;
//    }

    leaf->translation = translation;
    leaf->rotation = rotation;
    return leaf;
}


