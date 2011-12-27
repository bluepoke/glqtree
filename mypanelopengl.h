#ifndef MYPANELOPENGL_H
#define MYPANELOPENGL_H

#include <QGLWidget>

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

private:
    int sides;
    double radius;
    double xRotate;
    double yRotate;
    double zRotate;

    static const float MIN_ZOOM_DISTANCE = 3.0f;
    static const float MAX_ZOOM_DISTANCE = 25.0f;
    static const float MAX_TILT_ANGLE = 90.0f;

    float mouseZoomDistance;
    QPoint mouseLastPosition;
    QPointF modelRotation;

signals:
    void statusChanged(QString message);

public slots:
    void changeSides(int s);
    void changeRadius(double r);
    void rotateX(int angle);
    void rotateY(int angle);
    void rotateZ(int angle);

};

#endif // MYPANELOPENGL_H
