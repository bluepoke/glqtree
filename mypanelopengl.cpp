#include "mypanelopengl.h"
#include <math.h>
#include <QMouseEvent>

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QGLWidget(parent)
{
    // mouse navigation values
    mouseZoomDistance= 5.0f;
    modelRotation = QPointF();
}

void MyPanelOpenGL::initializeGL() {
    // everything regarding the openGL initialization should be done here
    glShadeModel(GL_SMOOTH);
    // this would be the background colour of sorts
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // enable lighting
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    // enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    // set material properties which will be assigned by glColor
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void MyPanelOpenGL::resizeGL(int width, int height){
    // the viewport should follow the window size
    glViewport(0, 0, width, height);
    // projectional matrix 4tw
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 40° fov, aspect ratio decided by the viewport, near and far clipping values
    gluPerspective(40, (float)width/height, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // add a camera with vectors:(where, to, up)
    gluLookAt(0,0,5, 0,0,0, 0,1,0);
}

void MyPanelOpenGL::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // move and rotate this basic model according to mouse actions
    glTranslatef(0, 0, -mouseZoomDistance);
    glRotatef(modelRotation.x(), 1, 0, 0);
    glRotatef(modelRotation.y(), 0, 1, 0);

    glBegin(GL_QUADS);

        GLfloat p = 0.5;

        // testing normal vectors on this face
        glColor4f(1,0,0,1);
        glNormal3f(-1,-1,-1);
        glVertex3f(-p,-p,-p);
        glNormal3f( 1,-1,-1);
        glVertex3f( p,-p,-p);
        glNormal3f( 1, 1,-1);
        glVertex3f( p, p,-p);
        glNormal3f(-1, 1,-1);
        glVertex3f(-p, p,-p);

        // the rest adds up to a standard cube
        glNormal3f( 0, 0, 1);
        glVertex3f(-p,-p, p);
        glVertex3f( p,-p, p);
        glVertex3f( p, p, p);
        glVertex3f(-p, p, p);

        glColor4f(0,1,0,1);
        glNormal3f(-1, 0, 0);
        glVertex3f(-p,-p, p);
        glVertex3f(-p,-p,-p);
        glVertex3f(-p, p,-p);
        glVertex3f(-p, p, p);

        glNormal3f( 1, 0, 0);
        glVertex3f( p,-p, p);
        glVertex3f( p,-p,-p);
        glVertex3f( p, p,-p);
        glVertex3f( p, p, p);

        glColor4f(0,0,1,1);
        glNormal3f( 0,-1, 0);
        glVertex3f(-p,-p, p);
        glVertex3f(-p,-p,-p);
        glVertex3f( p,-p,-p);
        glVertex3f( p,-p, p);

        glNormal3f( 0, 1, 0);
        glVertex3f(-p, p, p);
        glVertex3f(-p, p,-p);
        glVertex3f( p, p,-p);
        glVertex3f( p, p, p);

    glEnd();
}

void MyPanelOpenGL::mouseMoveEvent(QMouseEvent *event) {
    // should someone else handle this event?
    QGLWidget::mouseMoveEvent(event);
    if (event->isAccepted())
        return;

    // only handle model movement if the LMB is pressed
    if (event->buttons() & Qt::LeftButton) {
        // the mouse moved how far ?
        QPointF delta = event->globalPos() - mouseLastPosition;

        // calculate an appropriat rotation vector
        modelRotation += QPointF(delta.y() * 0.02, delta.x() * 0.02);
        // we do not want to tilt the model over the top or the bottom to prevent confusion with the x axis
        if (modelRotation.x() > MAX_TILT_ANGLE)
            modelRotation.setX(MAX_TILT_ANGLE);
        else if (modelRotation.x() < -1 * MAX_TILT_ANGLE)
            modelRotation.setX(-1 * MAX_TILT_ANGLE);

        // execute
        event->accept();
        updateGL();
    }
    // TODO handle RMB movement for panning the camera
    else if (event->button() & Qt::RightButton) {

    }
}

void MyPanelOpenGL::wheelEvent(QWheelEvent *event) {
    // should someone else handle this event?
    QGLWidget::wheelEvent(event);
    if (event->isAccepted())
        return;

    // calculate an appropriate zoom vector
    mouseZoomDistance *= pow(1.2, -event->delta() / 120);

    // prevent zooming inside or too far outside the model
    if (mouseZoomDistance < MIN_ZOOM_DISTANCE)
        mouseZoomDistance = MIN_ZOOM_DISTANCE;
    else if (mouseZoomDistance > MAX_ZOOM_DISTANCE)
        mouseZoomDistance = MAX_ZOOM_DISTANCE;

    // execute
    event->accept();
    updateGL();
}

void MyPanelOpenGL::mousePressEvent(QMouseEvent *event) {
    // should someone else handle this event?
    QGLWidget::mousePressEvent(event);
    if (event->isAccepted())
        return;

    // remember the position of the 1st click
    mouseLastPosition = event->globalPos();

    // execute
    event->accept();
    emit statusChanged("LMB clicked.");
}

void MyPanelOpenGL::mouseReleaseEvent(QMouseEvent *event) {
    // should someone else handle this event?
    QGLWidget::mouseReleaseEvent(event);
    if (event->isAccepted())
        return;

    // TODO handle RMB movement for panning the camera
    // we're done
    event->accept();
    emit statusChanged("LMB released.");
    updateGL();
}
