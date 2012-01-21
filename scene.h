#ifndef SCENE_H
#define SCENE_H

#include <QVector3D>
#include <QList>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QGLWidget>
#include "plant.h"

class SceneObject
{
public:
    SceneObject(SceneObject *parent = 0);
    virtual void render() {}

    QList<SceneObject*> *children;
    SceneObject *parent;
    QVector3D *rotation;
    QVector3D *translation;
    bool isRendered;
};

class BranchSection : public SceneObject
{
private:
public:
    BranchSection(SceneObject *parent = 0, double radBottom = 0, double radTop = 0, double length = 0);
    void render();

    double radBottom;
    double radTop;
    double length;
};

class EndSection : public SceneObject
{
public:
    EndSection(SceneObject *parent = 0, double radius = 0);
    void render();

    double radius;
};

class Leaf : public SceneObject
{
public:
    Leaf(SceneObject *parent = 0, double width = 0, double length = 0);
    void render();

    double width;
    double length;
};

class Scene
{
public:
    static Scene* activeScene;
    Scene(Plant *plant = 0, QWidget *oglPanel = 0);

    void initScene(Plant *plant = 0);
    SceneObject *root;
    QWidget *oglPanel;

private:
    QList<SceneObject*>* createSceneObject(Plant *plant = 0, SceneObject *parent = 0, int age = 0);
    SceneObject* constructBranchSection(Plant *plant, SceneObject *parent, int parentRadius, int age);
    SceneObject* constructEndSection(Plant *plant, SceneObject *parent, int age, bool hasLeaf);
    QList<SceneObject*>* createLeaves(Plant *plant, SceneObject *parent, int age);
    SceneObject* constructLeaf(Plant *plant, SceneObject *parent, int age, QVector3D *translation);
};

#endif // SCENE_H
