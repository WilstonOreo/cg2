#include "tbd/log.h"
#include "cg2/Vector.hpp"

#include "glwidget.h"

#include "helper.h"
#include <GL/glu.h>

LOG_INIT;

using namespace cg2;

GLWidget::GLWidget(QWidget *parent) :
        QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering), parent)
{
  old_x = 0;
  old_y = 0;
  lbutton = false;
  angle = 0.0;
  pointSize = 2.0;
  kNearest = 10;
  radius = 2.0;
  selectionMode = SELECT_RADIUS;
}

void GLWidget::initializeGL()
{
  pointCloud.read("cow.off");

    // Set up the rendering context, define display lists etc.:
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_CULL_FACE);

    // fix outlines z-fighting withthe quads
    glPolygonOffset(1, 1);
    glEnable(GL_POLYGON_OFFSET_FILL);
    
}

void GLWidget::update()
{
  if (selectionMode == SELECT_RADIUS) 
    pointCloud.collectInRadius(selection,radius); else
  if (selectionMode == SELECT_KNEAREST)
    pointCloud.collectKNearest(selection,kNearest);
  
  paintGL();
}

void GLWidget::resizeGL(int w, int h)
{

  w = w & ~1; h = h & ~1;
    // setup viewport, projection etc.:
    glViewport(0, 0, (GLint)w, (GLint)h);

	// reshaped window aspect ratio
	float aspect = (float) w / (float) h;

// restore view definition after window reshape
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  // perspective projection
	gluPerspective(60.0, aspect, 1.0, 100.0);
	  
	GLdouble centerX= 0;
	GLdouble centerY= 0;
	GLdouble centerZ= 0;
  // set camera parameters
	GLdouble eyeX=pointCloud.boundingBox().size().length()*cos(angle/100.0);
	GLdouble eyeY=pointCloud.boundingBox().size().y*1.5;
	GLdouble eyeZ=pointCloud.boundingBox().size().length()*sin(angle/100.0); 
	GLdouble upX=0;
	GLdouble upY=1;
	GLdouble upZ=0;

    gluLookAt(eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ);
	   
    // clear background and depth buffer
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

}

Point3f unProject(QPoint const & pos)
{
    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    GLdouble modelView[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    double winX = pos.x();
    double winY = viewport[3] - pos.y();

    GLfloat winZ;
    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    if (winZ >= 1.0f)
        return Point3f(qQNaN(), qQNaN(), qQNaN());

    GLdouble x,y,z;
    gluUnProject(winX, winY, winZ, modelView, projection, viewport, &x, &y, &z);
    return Point3f(x,y,z);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPointSize(pointSize);
    glLoadIdentity();
  
    cg2::Vec3f c = 0.5*(pointCloud.boundingBox().max.vec3f() + pointCloud.boundingBox().min.vec3f());
    glTranslatef(-c.x,-c.y,-c.z);
    pointCloud.draw(cg2::Color(0.8,0.5,0.0));

    glPointSize(pointSize*4.0);
    glBegin(GL_POINTS);
    glColor3f(1.0,0.0,0.0);
    glVertex3f(selection.x,selection.y,selection.z);
    glEnd();

    swapBuffers();
}



// mouse motion
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (lbutton && event->buttons() != Qt::NoButton)
  {
    angle += event->x() - old_x;
    //int motionY = event->y() - old_y;
    //mouseMotion(motionX, motionY);
    //
    resizeGL(this->width(),this->height());
    paintGL();

    old_x = event->x();
    old_y = event->y();
  }
}

// mouse callback
void GLWidget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() != Qt::NoButton)
  {
    cg2::Point3f newSelection = unProject(event->pos());
    if (qIsNaN(newSelection.x)) {
        old_x = event->x(); old_y = event->y();
        lbutton = true;
        cerr << "nan" << endl;
    }
    else {
        cerr << "not nan" << endl;
        selection = newSelection;
    }

    update();
  }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() != Qt::NoButton)
  {
    lbutton = false;
  }
}
