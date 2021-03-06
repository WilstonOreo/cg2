#include "cg2/Vector.hpp"

#include "glwidgetex2.h"

#include <GL/glu.h>
#include <qnumeric.h>
#include <QMouseEvent>

#include <iostream>

using namespace cg2;

GLWidgetEx2::GLWidgetEx2(QWidget * parent) :
  QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering), parent),
  gridSize(3), gridSizeCasteljau(3)
{
}

void GLWidgetEx2::setPointSizeSource(double size)
{
  pointSizeSource = size;
  updateGL();
}

void GLWidgetEx2::setPointSizeGrid(double size)
{
  pointSizeGrid = size;
  updateGL();
}

void GLWidgetEx2::setGridSize(int size)
{
  gridSize = size;
  recalc();
  updateGL();
}

void GLWidgetEx2::setPointSizeGridCasteljau(double size)
{
  pointSizeGridCasteljau = size;
  updateGL();
}

void GLWidgetEx2::setGridSizeCasteljau(int size)
{
  gridSizeCasteljau = size;
  recalc();
  updateGL();
}

void GLWidgetEx2::setDrawKDTree(int state)
{
  pointCloud.drawKDTree(state != Qt::Unchecked);
  updateGL();
}

void GLWidgetEx2::setRenderMode(int mode)
{
  renderMode = mode;
  updateGL();
}

void GLWidgetEx2::recalc()
{
  pointCloud.read("franke4.off");
  //void makeGrid(PointCloud & out, PointCloud const & in, int gridSize);

  pointGrid.width(gridSize);
  pointGrid.height(gridSize);
  pointGrid.generateGrid(pointCloud);

  pointGridCasteljau.width(gridSizeCasteljau);
  pointGridCasteljau.height(gridSizeCasteljau);
  pointGridCasteljau.generateCasteljauGrid(pointGrid);

  //makeGrid(pointGrid, pointCloud, gridSize);
}

void GLWidgetEx2::initializeGL()
{
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

void GLWidgetEx2::resizeGL(int w, int h)
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
  GLdouble eyeY=-1.5*pointCloud.boundingBox().size().length();
  GLdouble eyeZ=0;
  GLdouble upX=0;
  GLdouble upY=0;
  GLdouble upZ=1;

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

void GLWidgetEx2::paintGL()
{
  cg2::Vec3f center = 0.5*(pointCloud.boundingBox().max.vec3f() + pointCloud.boundingBox().min.vec3f());

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  glRotatef(pitch, 1, 0, 0);
  glRotatef(yaw, 0, 0, 1);
  glTranslatef(-center.x,-center.y,-center.z);

  if (pointSizeSource)
  {
    glPointSize(pointSizeSource);
    pointCloud.draw(cg2::Color(0.8,0.5,0.0));
  }
  switch (renderMode)
  {
  case 0:
    if (pointSizeGrid)
    {
      glPointSize(pointSizeGrid);
      pointGrid.draw(cg2::Color(0.0,0.5,1.0));
    }
    break;

  case 1:
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_BACK, GL_LINE);
    glPolygonMode(GL_FRONT, GL_FILL);
    glDisable(GL_CULL_FACE);

    pointGrid.drawSurface(Color(1,1,1));

    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_LINE);
    glColor4f(0,0,0,0.125);
    glEnable(GL_CULL_FACE);

    pointGrid.drawSurface(Color(0.25,0.25,0.25));
    break;

  case 2:
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_BACK, GL_LINE);
    glPolygonMode(GL_FRONT, GL_FILL);
    glDisable(GL_CULL_FACE);

    pointGridCasteljau.drawSurface(Color(1,1,1));

    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_LINE);
    glColor4f(0,0,0,0.125);
    glEnable(GL_CULL_FACE);

    pointGridCasteljau.drawSurface(Color(0.25,0.25,0.25));
    break;

  case 3:
    if (pointSizeGridCasteljau)
    {
      glPointSize(pointSizeGridCasteljau);
      pointGridCasteljau.draw(cg2::Color(0.0,0.5,1.0));
    }
    break;
  }
}



// mouse motion
void GLWidgetEx2::mouseMoveEvent(QMouseEvent * event)
{
  if (event->buttons() != Qt::NoButton)
  {
    yaw += event->x() - old_x;
    pitch += event->y() - old_y;
    if (pitch > 90)
    {
      pitch = 90;
    }
    if (pitch < -90)
    {
      pitch = -90;
    }
    updateGL();

    old_x = event->x();
    old_y = event->y();
  }
}

// mouse callback
void GLWidgetEx2::mousePressEvent(QMouseEvent * event)
{
  if (event->button() != Qt::NoButton)
  {
    old_x = event->x();
    old_y = event->y();

    updateGL();
  }
}

double wendland(double d, double h)
{
  /*x *= h;
  const double Q_PI = 3.14159265358979323846;
  return (1/sqrt(2*Q_PI))*exp(-0.5*d*d); // gauss, not wendland*/
  return std::pow(1-d/h, 4)*(4*d/h+1);
}

void makeGrid(PointCloud & out, PointCloud const & in, int gridSize)
{
  BoundingBox box = in.boundingBox();
  int const width = gridSize, height = gridSize;
  float const xdist = box.size().x / width, ydist = box.size().y / height;
  float const xmin = box.min.x, ymin = box.min.y;

  float const radius = sqrt(xdist*xdist + ydist*ydist)*2;
  //float const radius = 2;


  //vector<float> grid;  // Den Typ Array2D gibts nich, analog kann man auch vector< vector<float> > nehmen
  //grid.resize(width*height);

  out.vertices.clear();
  for (int xp = 0; xp < width; xp++)
  {
    float x = xmin + xp*xdist;
    for (int yp = 0; yp < height; yp++)
    {
      float y = ymin + yp*ydist;

      Point3f currentXY(x, y, 0);
      int nPoints = 0;
      double sumWeight = 0;
      double currentZ = 0;
      foreach(Vertex const * v, in.collectInRadius(currentXY, radius))
      {
        Point3f const & p = v->v;
        Point3f pFlat(p);
        pFlat.z = 0;

        if ((currentXY-p).length() > radius)
        {
          std::cout << "point exceeds radius: " << (currentXY-p).length() << std::endl;
        }
        double distance = (currentXY-pFlat).length(); // Distanz zwischen P und (x,y,0)
        double weight = wendland(distance, radius);

        sumWeight += weight;
        currentZ += weight * p.z;

        nPoints++;
      }
      if (nPoints && sumWeight)
      {
        /*std::cout
                << "nPoints=" << nPoints
                << " sumWeight=" << sumWeight
                << " currentZ=" << currentZ;*/

        //current /= nPoints;
        currentZ /= sumWeight;
        /*std::cout
                << " after=" << currentZ
                << std::endl;*/
      }

      //grid[y*width+x] = current;

      currentXY.z = currentZ;
      out.vertices.push_back(currentXY);
    }
  }
  out.update();
}
