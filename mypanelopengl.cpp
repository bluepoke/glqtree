#include "mypanelopengl.h"
#include <math.h>
#include <QMouseEvent>
#include <QDebug>
#include <QList>
#include "plant.h"

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QGLWidget(parent)
{
    // mouse navigation values
    mouseZoomDistance = 50.0f;
    // initial rotation and movement here, none by default
    modelBaseRotation = QPointF();
    modelBaseMovement = QPointF();
    // initially, no accumulated rotation or movement given
    modelAccumulatedRotation = QPointF();
    modelAccumulatedMovement = QPointF();
}

void MyPanelOpenGL::initializeGL() {
    // everything regarding the openGL initialization should be done here
    glShadeModel(GL_SMOOTH);

    // alpha blending for possible transparency
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // this would be the background colour of sorts
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClearDepth(1.0f);

    // depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // setting material attributes
    GLfloat mat_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat model_ambient[] = { 0.5, 0.5, 0.5, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

    // enable lighting
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    // enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    // set material properties which will be assigned by glColor
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // load a scene to display
    Scene::activeScene = new Scene(Plant::activePlant, this);
}

void MyPanelOpenGL::resizeGL(int width, int height){
    // the viewport should follow the window size
    glViewport(0, 0, width, height);
    // projectional matrix 4tw
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 40° fov, aspect ratio decided by the viewport, near and far clipping values
    gluPerspective(40, (float)width/height, 0.01, 10000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // add a camera with vectors:(where, to, up)
    gluLookAt(0,0,50, 0,0,0, 0,1,0);
}

void MyPanelOpenGL::paintGL(){
    // basic display settings
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel (GL_SMOOTH);
    // move the model, load default identity
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // calculate resulting movement
    QPointF modelRotation = modelBaseRotation + modelAccumulatedRotation;
    QPointF modelMovement = modelBaseMovement + modelAccumulatedMovement;

    // move and rotate this basic model according to mouse actions
    glTranslatef(modelMovement.x(), -modelMovement.y(), -mouseZoomDistance);
    glRotatef(modelRotation.x(), 1, 0, 0);
    glRotatef(modelRotation.y(), 0, 1, 0);

    // load the scene in red
    glColor4f(1, 0, 0, 1);

    // render scene, starting with the root
    if (Scene::activeScene->root != 0) {
        // y-axis should now be upwards :)
        glPushMatrix();
        glRotatef(-90.0, 1.0, 0.0, 0.0);

        renderObject(Scene::activeScene->root);
        glPopMatrix();
    }

    glFlush();
}

void MyPanelOpenGL::renderObject(SceneObject *obj) {
    glPushMatrix();

    // do all local transformations
    QVector3D *v = obj->translation;
    glTranslatef(v->x(), v->y(), v->z());

    v = obj->rotation;
    if (v->x() != 0) glRotatef(v->x(), 1, 0, 0);
    if (v->y() != 0) glRotatef(v->y(), 0, 1, 0);
    if (v->z() != 0) glRotatef(v->z(), 0, 0, 1);

    // render the parent
    obj->render();
    // render all children
    for(int i=0; i < obj->children->size(); i++) {
        SceneObject *child;
        child = obj->children->at(i);
        renderObject(child);
    }

    glPopMatrix();
}

void MyPanelOpenGL::mouseMoveEvent(QMouseEvent *event) {
    // should someone else handle this event?
    QGLWidget::mouseMoveEvent(event);
    if (event->isAccepted())
        return;

    if (event->buttons() & Qt::LeftButton) {
        // the mouse moved how far ?
        QPointF delta = event->globalPos() - mouseLastRotationPosition;

        // calculate an appropriate rotation vector
        modelAccumulatedRotation = 0.5 * QPointF(delta.y(), delta.x());
        // we do not want to tilt the model over the top or the bottom to prevent confusion with the x axis
        if ((modelBaseRotation.x() + modelAccumulatedRotation.x()) > MAX_TILT_ANGLE)
            modelAccumulatedRotation.setX(MAX_TILT_ANGLE - modelBaseRotation.x());
        else if ((modelBaseRotation.x() + modelAccumulatedRotation.x()) < -1 * MAX_TILT_ANGLE)
            modelAccumulatedRotation.setX((-1 * MAX_TILT_ANGLE) - modelBaseRotation.x());
    }
    else if (event->buttons() & Qt::RightButton) {
        // the mouse moved how far ?
        QPointF delta = event->globalPos() - mouseLastMovementPosition;
        // calculate an appropriate movement vector
        modelAccumulatedMovement = 0.1 * QPointF(delta.x(), delta.y());
    }

    // execute
    event->accept();
    updateGL();
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

    // we're done
    event->accept();
    updateGL();
}

void MyPanelOpenGL::mousePressEvent(QMouseEvent *event) {
    // should someone else handle this event?
    QGLWidget::mousePressEvent(event);
    if (event->isAccepted())
        return;

    if (event->buttons() & Qt::LeftButton) {
        // remember the position of the 1st click
        mouseLastRotationPosition = event->globalPos();
        modelAccumulatedRotation = QPointF();
    }
    else if (event->buttons() & Qt::RightButton) {
        // remember the position of the 1st click
        mouseLastMovementPosition = event->globalPos();
        modelAccumulatedMovement = QPointF();
    }

    // execute
    event->accept();
}

void MyPanelOpenGL::mouseReleaseEvent(QMouseEvent *event) {
    // should someone else handle this event?
    QGLWidget::mouseReleaseEvent(event);
    if (event->isAccepted())
        return;

    // add up to get a final movement and rotation
    modelBaseRotation += modelAccumulatedRotation;
    modelBaseMovement += modelAccumulatedMovement;
    // reset accumulation
    modelAccumulatedRotation = QPointF();
    modelAccumulatedMovement = QPointF();

    // we're done
    event->accept();
    updateGL();
}
