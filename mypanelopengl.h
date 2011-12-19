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
    void mouseMoveEvent(QMouseEvent *event);

private:
    int sides;
    double radius;
    double xRotate;
    double yRotate;
    double zRotate;

signals:

public slots:
    void changeSides(int s);
    void changeRadius(double r);
    void rotateX(int angle);
    void rotateY(int angle);
    void rotateZ(int angle);

};

#endif // MYPANELOPENGL_H
