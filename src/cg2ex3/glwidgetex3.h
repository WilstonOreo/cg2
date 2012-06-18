#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "cg2/ImpliciteSurface.hpp"

class GLWidgetEx3 : public QGLWidget
{
  Q_OBJECT
public:
  explicit GLWidgetEx3(QWidget * parent = 0);

  cg2::ImpliciteSurface impliciteSurface;

  void mouseMoveEvent(QMouseEvent * event);
  void mousePressEvent(QMouseEvent * event);

protected:
  virtual void initializeGL();
  virtual void resizeGL(int w, int h);
  virtual void paintGL();

private:
  void recalc();

  std::vector<cg2::Ray> generateRays();


  float pointSizeSource;

  float pointSizeGrid;
  int gridSize;
  int renderMode;

  // some stateholders for mouse motion
  // last mouse position in window
  int old_x, old_y;
  float yaw, pitch;

  cg2::Point3f lightPos_;

  std::vector<cg2::Ray> rays_;

  bool drawGrid;
signals:

public slots:
  void setPointSizeSource(double size);

  void setPointSizeGrid(double size);
  void setGridSize(int size);

  void setDrawGrid(int state);
  void setRenderMode(int state);

  void raytrace();
  void spheretrace();
};

#endif // GLWIDGET_H
