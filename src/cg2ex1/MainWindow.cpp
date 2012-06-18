#include "MainWindow.h"
#include <tbd/log.h>

MainWindow::MainWindow(QMainWindow * parent) : QMainWindow(parent)
{
  setupUi(this);

  connect(btnInRadius,SIGNAL(clicked()), this, SLOT(setSelectionMode()));
  connect(btnKNearest,SIGNAL(clicked()), this, SLOT(setSelectionMode()));
  connect(spinBox,SIGNAL(editingFinished()), this, SLOT(setKNearest()));
  connect(doubleSpinBox,SIGNAL(editingFinished()), this, SLOT(setRadius()));
  connect(doubleSpinBox_3,SIGNAL(editingFinished()), this, SLOT(setPointSize()));
  connect(spinBox,SIGNAL(valueChanged(int)), this, SLOT(setKNearest()));
  connect(boxVertexId,SIGNAL(valueChanged(int)), this, SLOT(setVertexId()));
  connect(doubleSpinBox,SIGNAL(valueChanged(double)), this, SLOT(setRadius()));
  connect(doubleSpinBox_3,SIGNAL(valueChanged(double)), this, SLOT(setPointSize()));
  connect(boxRenderKDTree,SIGNAL(clicked()), this, SLOT(setDrawKDTree()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::setPointSize()
{
  glWidget->pointSize = doubleSpinBox_3->value();
  glWidget->update();
}

void MainWindow::setRadius()
{
  glWidget->radius = doubleSpinBox->value();
  setVertexId();
}

void MainWindow::setDrawKDTree()
{
  glWidget->pointCloud.drawKDTree(boxRenderKDTree->isChecked());
  glWidget->update();
}

void MainWindow::setKNearest()
{
  glWidget->kNearest = spinBox->value();
  setVertexId();
}

void MainWindow::setVertexId()
{
  int value = boxVertexId->value();
  if (value < 0)
  {
    value = 0;
  }
  if (value >= (int)glWidget->pointCloud.size())
  {
    value = (int)glWidget->pointCloud.size()-1;
  }

  glWidget->selection = glWidget->pointCloud.vertices[value].v;

  if (glWidget->selectionMode == GLWidget::SELECT_RADIUS)
  {
    glWidget->pointCloud.collectInRadius(glWidget->selection,glWidget->radius);
  }
  else if (glWidget->selectionMode == GLWidget::SELECT_KNEAREST)
  {
    glWidget->pointCloud.collectKNearest(glWidget->selection,glWidget->kNearest);
  }

  glWidget->update();
}

void MainWindow::setSelectionMode()
{
  glWidget->selectionMode = (btnInRadius->isChecked()) ? GLWidget::SELECT_RADIUS : GLWidget::SELECT_KNEAREST;
  setVertexId();
}

