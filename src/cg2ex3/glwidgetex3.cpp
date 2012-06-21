#include "glwidgetex3.h"

#include "cg2/Vector.hpp"
#include "cg2/Image.hpp"

#include <tbd/log.h>

#include <GL/glu.h>
#include <qnumeric.h>
#include <QMouseEvent>

#include <iostream>

using namespace cg2;

GLWidgetEx3::GLWidgetEx3(QWidget * parent) :
  QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering), parent),
  gridSize(4), drawGrid(true)
{

}

void GLWidgetEx3::setPointSizeSource(double size)
{
  pointSizeSource = size;
  updateGL();
}

void GLWidgetEx3::setPointSizeGrid(double size)
{
  pointSizeGrid = size;
  updateGL();
}

void GLWidgetEx3::setGridSize(int size)
{
  gridSize = size;
  recalc();
  updateGL();
}

void GLWidgetEx3::setDrawGrid(int state)
{
  drawGrid = state != Qt::Unchecked;
  updateGL();
}

void GLWidgetEx3::setRenderMode(int mode)
{
  renderMode = mode;
  recalc();
  updateGL();
}

std::vector<cg2::Ray> GLWidgetEx3::generateRays()
{
  int viewport[4];
  double projection[16], modelview[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview );
  glGetDoublev(GL_PROJECTION_MATRIX, projection );
  glGetIntegerv(GL_VIEWPORT, viewport );
  int w = viewport[2], h = viewport[3];

  std::vector<cg2::Ray> _rays;
  _rays.resize(w*h);

  double cx,cy,cz;
  gluUnProject((viewport[2]-viewport[0])/2 , (viewport[3]-viewport[1])/2,0.0,
               modelview, projection, viewport, &cx,&cy,&cz);
  Point3f rayOrg(cx,cy,cz);

  double rayDirTopLeft[3];
  double rayDirTopRight[3];
  double rayDirBottomLeft[3];

  gluUnProject(0, h, 1.0, modelview, projection, viewport,
               &rayDirTopLeft[0],&rayDirTopLeft[1],&rayDirTopLeft[2]);
  gluUnProject(w, h, 1.0, modelview, projection, viewport,
               &rayDirTopRight[0],&rayDirTopRight[1],&rayDirTopRight[2]);
  gluUnProject(0, 0, 1.0, modelview, projection, viewport,
               &rayDirBottomLeft[0],&rayDirBottomLeft[1],&rayDirBottomLeft[2]);

  for (int i = 0; i < 3; i++)
  {
    rayDirTopRight[i] 	-= rayDirTopLeft[i];
    rayDirBottomLeft[i] -= rayDirTopLeft[i];
    rayDirTopLeft[i] 	-= rayOrg[i];
  }

  float invH = 1.0f/h, invW = 1.0f/w;
  for (int y = 0; y < h; y++)
  {
    float diffY = float(y)*invH;
    for (int x = 0; x < w; x++)
    {
      Ray* ray = &_rays[y*w+x];
      ray->org_ = rayOrg;
      ray->tMin_ = 0; ///screenMotion.znear;
      ray->tMax_ = INF;

      float diffX = float(x)*invW;
      for (int i = 0; i < 3; i++)
        ray->dir_[i] = rayDirTopLeft[i]+diffX*rayDirTopRight[i]
                      +diffY*rayDirBottomLeft[i];
      //ray->dir *= ray->tmax / 100000.0;
    }
  }
  return _rays;
}

void GLWidgetEx3::raytrace()
{
  int viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport );
  int w = viewport[2], h = viewport[3];

  Image image(w,h);
  vector<Ray> _rays = generateRays();

  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
    {
      Ray& ray = _rays[y*w+x];
      image.set(x,y,Color3f(0,0,0));
      Vec3f normal;

      if (impliciteSurface.intersect(ray,&normal))
      {
        normal.normalize();
        Point3f iPoint = ray.org_ + ray.tMin_ * ray.dir_;

        float invlightDist = 1.0f / (lightPos_ - iPoint).length();
        Vec3f L = (lightPos_ - iPoint).normalized();
        Color3f color(0.1,0.1,0.1);
        float angle = normal.dot(L);
        if (angle < 0.0) angle = 0.0;

        color += Color3f(angle,angle,angle);
        image.set(x,y,color);
      }
    }

  image.save_ppm("raytrace.ppm");
}

void GLWidgetEx3::spheretrace()
{

}



void GLWidgetEx3::recalc()
{
  impliciteSurface.size(gridSize,gridSize,gridSize);

}

void GLWidgetEx3::initializeGL()
{
  impliciteSurface.read("cow.off");
  recalc();

  // Set up the rendering context, define display lists etc.:
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

  //glEnable(GL_LIGHTING);
  //glEnable(GL_CULL_FACE);

  // light and material
  //glEnable(GL_COLOR_MATERIAL);
  //GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
  //GLfloat mat_specular[] = {0.6, 0.6, 0.6, 1.0};
  //GLfloat mat_shininess[] = { 3.0 };
  GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat model_ambient[] = { 0.3, 0.3, 0.3 };
  GLfloat light_position[] = { 0.0, 0.0, 2.0, 1.0 };

  lightPos_(0.0,4.0,6.0);


  /*glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);*/
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHT1);
  //glEnable(GL_NORMALIZE);

  // fix outlines z-fighting withthe quads
  glPolygonOffset(1, 1);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glClearColor(0.0,0.0,0.0,1.0);
}

void GLWidgetEx3::resizeGL(int w, int h)
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
  GLdouble eyeZ=-1.5*impliciteSurface.bounds().size().length();
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

void GLWidgetEx3::paintGL()
{
  cg2::Vec3f center = 0.5*(impliciteSurface.bounds().max().vec() + impliciteSurface.bounds().min().vec());

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  glRotatef(pitch, 1, 0, 0);
  glRotatef(yaw, 0, 1, 0);
  glTranslatef(-center.x(),-center.y(),-center.z());

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

    glDisable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);


    break;

  case 3:

    glEnable(GL_LIGHTING);
    glPolygonMode(GL_BACK, GL_FILL);
    glPolygonMode(GL_FRONT, GL_FILL);
    glDisable(GL_CULL_FACE);

    glLightfv(GL_LIGHT1, GL_POSITION, lightPos_.p());
    impliciteSurface.draw(Color4f(1,1,1));

    break;
  }
}



// mouse motion
void GLWidgetEx3::mouseMoveEvent(QMouseEvent * event)
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
void GLWidgetEx3::mousePressEvent(QMouseEvent * event)
{
  if (event->button() != Qt::NoButton)
  {
    old_x = event->x();
    old_y = event->y();

    updateGL();
  }
}


