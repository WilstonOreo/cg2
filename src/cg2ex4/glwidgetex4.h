#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "cg2/Mesh.hpp"

class GLWidgetEx4 : public QGLWidget
{
  Q_OBJECT
public:
  explicit GLWidgetEx4(QWidget * parent = 0);


  void mouseMoveEvent(QMouseEvent * event);
  void mousePressEvent(QMouseEvent * event);

protected:
  virtual void initializeGL();
  virtual void resizeGL(int w, int h);
  virtual void paintGL();

private:
  void recalc();

  float pointSizeSource;

  float pointSizeGrid;
  int gridSize;
  int renderMode;

  // some stateholders for mouse motion
  // last mouse position in window
  int old_x, old_y;
  float yaw, pitch;
  float lightYaw, lightPitch;

  cg2::Point3f lightPos_;

  cg2::Mesh mesh;
  bool drawGrid;
signals:

public slots:
  void setPointSizeSource(double size);

  void setPointSizeGrid(double size);
  void setGridSize(int size);

  void setDrawGrid(int state);
  void setRenderMode(int state);
};

#endif // GLWIDGET_H
