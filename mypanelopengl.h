#ifndef MYPANELOPENGL_H
#define MYPANELOPENGL_H

#include <QGLWidget>
#include "scene.h"
#include <math.h>
#include <QMouseEvent>

class MyPanelOpenGL : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyPanelOpenGL(QWidget *parent = 0);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent  *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void renderObject(SceneObject *obj);

private:
    static const float MIN_ZOOM_DISTANCE = 3.0f;
    static const float MAX_ZOOM_DISTANCE = 10000.0f;
    static const float MAX_TILT_ANGLE = 90.0f;

    float mouseZoomDistance;
    QPoint mouseLastRotationPosition;
    QPoint mouseLastMovementPosition;
    QPointF modelBaseRotation;
    QPointF modelAccumulatedRotation;
    QPointF modelBaseMovement;
    QPointF modelAccumulatedMovement;
    Scene *scene;

signals:
    void cameraChanged(int x, int y, int zoom);
    void renderingDone();

public slots:
    void changeCamera(int x, int y, int zoom);
};

#endif // MYPANELOPENGL_H
