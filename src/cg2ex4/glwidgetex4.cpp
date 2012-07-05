#include "glwidgetex4.h"

#include "cg2/Vector.hpp"

#include <tbd/log.h>

#include <GL/glu.h>
#include <qnumeric.h>
#include <QMouseEvent>

#include <iostream>


using namespace cg2;

GLWidgetEx4::GLWidgetEx4(QWidget * parent) :
  QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering), parent),
  gridSize(4), drawGrid(true), yaw(0), pitch(0), old_x(0), old_y(0), lightYaw(0), lightPitch(0)
{

}

void GLWidgetEx4::setPointSizeSource(double size)
{
  pointSizeSource = size;
  updateGL();
}

void GLWidgetEx4::setPointSizeGrid(double size)
{
  pointSizeGrid = size;
  updateGL();
}

void GLWidgetEx4::setGridSize(int size)
{
  gridSize = size;
  recalc();
  updateGL();
}

void GLWidgetEx4::setDrawGrid(int state)
{
  drawGrid = state != Qt::Unchecked;
  updateGL();
}

void GLWidgetEx4::setRenderMode(int mode)
{
  renderMode = mode;
  recalc();
  updateGL();
}


void GLWidgetEx4::recalc()
{
  //impliciteSurface.size(gridSize,gridSize,gridSize);

}

void GLWidgetEx4::initializeGL()
{
 // impliciteSurface.read("cow.off");

  mesh.read("torus.off");
  mesh.simplify();

  recalc();

  // Set up the rendering context, define display lists etc.:
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);


  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_CULL_FACE);

  glMatrixMode(GL_MODELVIEW);
  // set light
  {
    lightPos_(0.0,4.0,6.0);
    Color3f ambient_(0.1,0.1,0.1);
    Color3f diffuse_(1.0,1.0,1.0);
    glEnable(GL_LIGHTING);

    // light and material
    glEnable(GL_COLOR_MATERIAL);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuse_[0]);
    glLightfv(GL_LIGHT0, GL_POSITION, &lightPos_[0]);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, &ambient_[0] );
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
  }
  glEnable(GL_NORMALIZE);

  // fix outlines z-fighting withthe quads
  glPolygonOffset(1, 1);
  glEnable(GL_POLYGON_OFFSET_FILL);

  setAutoBufferSwap(true);
  


  // fix outlines z-fighting withthe quads
  glPolygonOffset(1, 1);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glClearColor(0.0,0.0,0.0,1.0);
}

void GLWidgetEx4::resizeGL(int w, int h)
{
  // round w and h to multiples of 2
  w = w & ~1;
  h = h & ~1;
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
  GLdouble eyeX=0;
  GLdouble eyeZ=-1.5*mesh.bounds().size().length();
  GLdouble eyeY=0;
  GLdouble upX=0;
  GLdouble upZ=0;
  GLdouble upY=1;

  gluLookAt(eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ);

  glMatrixMode(GL_MODELVIEW);

  update();
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
  {
    return Point3f(qQNaN(), qQNaN(), qQNaN());
  }

  GLdouble x,y,z;
  gluUnProject(winX, winY, winZ, modelView, projection, viewport, &x, &y, &z);
  return Point3f(x,y,z);
}

void GLWidgetEx4::paintGL()
{
  cg2::Vec3f center = 0.5*(mesh.bounds().max().vec() + mesh.bounds().min().vec());

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  glRotatef(pitch, 1, 0, 0);
  glRotatef(yaw, 0, 1, 0);
  glTranslatef(-center.x(),-center.y(),-center.z());

/*
  if (drawGrid) impliciteSurface.drawGrid(cg2::Color4f(0.4,0.4,0.4,0.2));

  switch (renderMode)
  {
  case 0:
    if (pointSizeGrid)
    {
      glPointSize(pointSizeGrid);
      impliciteSurface.drawPoints(cg2::Color4f(0.0,0.5,1.0),lightPos_);
    }
    break;

  case 1:
    if (pointSizeGrid)
    {
      glPointSize(pointSizeGrid);
      impliciteSurface.drawValues(cg2::Color4f(0.0,0.5,1.0),lightPos_);
    }
    break;

  case 2:
    glEnable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);


    if (pointSizeGrid)
    {
      glPointSize(pointSizeGrid);
      impliciteSurface.drawValues(cg2::Color4f(0.0,0.5,1.0),lightPos_,false);
    }



    break;

  case 3:

*/
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glMatrixMode(GL_MODELVIEW);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos_.p());
    glEnable(GL_LIGHTING);
//    glPolygonMode(GL_BACK, GL_FILL);
//    glPolygonMode(GL_FRONT, GL_FILL);


  //  impliciteSurface.draw(Color4f(1,1,1));*/
    mesh.draw(Color4f(1,1,1));
  //  break;
    glDisable(GL_LIGHTING);

}



// mouse motion
void GLWidgetEx4::mouseMoveEvent(QMouseEvent * event)
{
  int dx = event->x() - old_x;
  int dy = event->y() - old_y;
  if (event->buttons() == Qt::LeftButton)
  {
    yaw += dx;
    pitch += dy;
    if (pitch > 90)
    {
      pitch = 90;
    }
    if (pitch < -90)
    {
      pitch = -90;
    }
    updateGL();

  }
  if (event->buttons() == Qt::RightButton)
  {
    lightYaw += dx;
    lightPitch += dy;
    if (lightPitch > 90)
    {
      lightPitch = 90;
    }
    if (lightPitch < -90)
    {
      lightPitch = -90;
    }
    
    lightPos_(cos(lightYaw*0.017453292)*6, 4, sin(lightYaw*0.017453292)*6);
    
    updateGL();
  }
  old_x = event->x();
  old_y = event->y();
}

// mouse callback
void GLWidgetEx4::mousePressEvent(QMouseEvent * event)
{
  if (event->button() != Qt::NoButton)
  {
    old_x = event->x();
    old_y = event->y();

    updateGL();
  }
}


