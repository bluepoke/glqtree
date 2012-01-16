#ifndef SCENE_H
#define SCENE_H

#include <QVector3D>
#include <QList>

class SceneObject
{
public:
    SceneObject(SceneObject *parent = 0);
    virtual void render() {}

    SceneObject *parent;
    QList<SceneObject> *children;
    QVector3D *rotationAxis;
    QVector3D *rotationAngles;
    QVector3D *translation;
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

#endif // SCENE_H
