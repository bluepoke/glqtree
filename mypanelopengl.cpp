#include "mypanelopengl.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QGLWidget(parent)
{
    sides = 3;
    radius = 1.0;
    xRotate = 0;
    yRotate = 0;
    zRotate = 0;

    distance= 5.0f;
    lastTime = 0;
    angularMomentum.y = 40.0f;
    time.start();
}

void MyPanelOpenGL::initializeGL() {
    glShadeModel(GL_SMOOTH);
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
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, (float)width/height, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5, 0,0,0, 0,1,0);
}

void MyPanelOpenGL::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    const int delta = time.elapsed() - lastTime;
    rotation += angularMomentum * (delta / 1000.0);
    lastTime += delta;
    glTranslatef(0, 0, -distance);
    glRotatef(rotation.x, 1, 0, 0);
    glRotatef(rotation.y, 0, 1, 0);
    glRotatef(rotation.z, 0, 0, 1);

//    glTranslatef(0,0,-5);
//    glRotatef(xRotate,1,0,0);
//    glRotatef(yRotate,0,1,0);
//    glRotatef(zRotate,0,0,1);

    glBegin(GL_QUADS);

        GLfloat p = radius / 2;

        // testing normal vectors
        glColor4f(1,0,0,1);
        glNormal3f(-1,-1,-1);
        glVertex3f(-p,-p,-p);
        glNormal3f( 1,-1,-1);
        glVertex3f( p,-p,-p);
        glNormal3f( 1, 1,-1);
        glVertex3f( p, p,-p);
        glNormal3f(-1, 1,-1);
        glVertex3f(-p, p,-p);

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

void MyPanelOpenGL::changeSides(int s){
    sides = s;
    updateGL();
}

void MyPanelOpenGL::changeRadius(double r){
    radius = r;
    updateGL();
}

void MyPanelOpenGL::rotateX(int angle){
    xRotate=angle;
    updateGL();
}

void MyPanelOpenGL::rotateY(int angle){
    yRotate=angle;
    updateGL();
}

void MyPanelOpenGL::rotateZ(int angle){
    zRotate=angle;
    updateGL();
} 

void MyPanelOpenGL::mouseMoveEvent(QMouseEvent *event){
    QGLWidget::mouseMoveEvent(event);
    if (event->isAccepted())
        return;
    if (event->buttons() & Qt::LeftButton) {
        const QPointF delta = event->globalPos() - lastPosition;
        const Point3d angularImpulse = Point3d(delta.y(), delta.x(), 0) * 0.1;
        rotation += angularImpulse;
        accumulatedMomentum +=angularImpulse;

        event->accept();
        updateGL();
    }
}

void MyPanelOpenGL::wheelEvent(QWheelEvent *event){
    QGLWidget::wheelEvent(event);
    if (event->isAccepted())
        return;

    distance *= pow(1.2, -event->delta() / 120);
    if (distance < MIN_DISTANCE)
        distance = MIN_DISTANCE;
    else if (distance > MAX_DISTANCE)
        distance = MAX_DISTANCE;
    event->accept();
    updateGL();
}

void MyPanelOpenGL::mousePressEvent(QMouseEvent *event){
    QGLWidget::mousePressEvent(event);
    if (event->isAccepted())
        return;

    lastPosition = event->globalPos();
    mouseEventTime = time.elapsed();
    angularMomentum = accumulatedMomentum = Point3d();
    event->accept();
    emit statusChanged("LMB clicked.");
}

void MyPanelOpenGL::mouseReleaseEvent(QMouseEvent *event){
    QGLWidget::mouseReleaseEvent(event);
    if (event->isAccepted())
        return;

    const int delta = time.elapsed() - mouseEventTime;
    angularMomentum = accumulatedMomentum * (1000.0 / qMax(1, delta));
    event->accept();
    emit statusChanged("LMB released.");
    updateGL();
}
