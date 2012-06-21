#include "mainwindowex3.h"
#include "glwidgetex3.h"

#include <QCheckBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QDockWidget>
#include <QGridLayout>
#include <QSignalMapper>
#include <QPushButton>

void MainWindowEx3::setupUi()
{
  glWidget = new GLWidgetEx3();
  //glWidget->setMouseTracking(true);

  setCentralWidget(glWidget);
  resize(950, 670);

  setFocusPolicy(Qt::NoFocus);
  setContextMenuPolicy(Qt::ActionsContextMenu);
  setAutoFillBackground(true);

  QGridLayout * layout = new QGridLayout();

  uiRenderGrid = new QCheckBox("Render Grid");
  layout->addWidget(uiRenderGrid, 0, 0, 1, 2);



  uiPointSizeSourceLabel = new QLabel("Point size");
  layout->addWidget(uiPointSizeSourceLabel, 1, 0);

  uiPointSizeSource = new QDoubleSpinBox();
  uiPointSizeSource->setSingleStep(0.2);
  layout->addWidget(uiPointSizeSource, 1, 1);

  uiPointSizeGridLabel = new QLabel("Grid Point size");
  layout->addWidget(uiPointSizeGridLabel, 2, 0);

  uiPointSizeGrid = new QDoubleSpinBox();
  uiPointSizeGrid->setSingleStep(0.2);
  layout->addWidget(uiPointSizeGrid, 2, 1);

  uiGridSizeLabel = new QLabel("Grid size");
  layout->addWidget(uiGridSizeLabel, 3, 0);

  uiGridSize = new QSpinBox();
  uiGridSize->setSingleStep(1);
  layout->addWidget(uiGridSize, 3, 1);

  uiRenderMode[0] = new QRadioButton("Render points");
  layout->addWidget(uiRenderMode[0], 6, 0, 1, 2);
  uiRenderMode[1] = new QRadioButton("Render function values");
  layout->addWidget(uiRenderMode[1], 7, 0, 1, 2);
  uiRenderMode[2] = new QRadioButton("Render function values with normals");
  layout->addWidget(uiRenderMode[2], 8, 0, 1, 2);
  uiRenderMode[3] = new QRadioButton("Render polygons");
  layout->addWidget(uiRenderMode[3], 9, 0, 1, 2);

  uiRaytraceBtn = new QPushButton("Raytrace");
  layout->addWidget(uiRaytraceBtn, 10,0, 1,2);

  uiSpheretraceBtn = new QPushButton("Sphere trace");
  layout->addWidget(uiSpheretraceBtn, 11,0, 1,2);

  class QButton * uiSpheretraceBtn;


  uiRenderSettings = new QDockWidget();

  QWidget * tmpWidget = new QWidget();
  uiRenderSettings->setWidget(tmpWidget);
  tmpWidget->setLayout(layout);

  addDockWidget(Qt::RightDockWidgetArea,uiRenderSettings);
} // setupUi

MainWindowEx3::MainWindowEx3(QMainWindow * parent) : QMainWindow(parent)
{
  setupUi();

  connect(uiPointSizeSource,SIGNAL(valueChanged(double)), glWidget, SLOT(setPointSizeSource(double)));
  connect(uiPointSizeGrid,SIGNAL(valueChanged(double)), glWidget, SLOT(setPointSizeGrid(double)));
  connect(uiGridSize,SIGNAL(valueChanged(int)), glWidget, SLOT(setGridSize(int)));
  connect(uiRenderGrid,SIGNAL(stateChanged(int)), glWidget, SLOT(setDrawGrid(int)));
  connect(uiRaytraceBtn,SIGNAL(clicked()), glWidget, SLOT(raytrace()));
  connect(uiSpheretraceBtn,SIGNAL(clicked()), glWidget, SLOT(spheretrace()));

  QSignalMapper * qsm = new QSignalMapper(this);
  for (int i = 0; i < 4; ++i)
  {
    qsm->setMapping(uiRenderMode[i], i);
    connect(uiRenderMode[i],SIGNAL(clicked()), qsm, SLOT(map()));
  }
  connect(qsm,SIGNAL(mapped(int)), glWidget, SLOT(setRenderMode(int)));

  
  uiRenderGrid->setChecked(false);
  uiPointSizeSource->setValue(2);

  uiPointSizeGrid->setValue(1);
  uiGridSize->setValue(16);

  uiRenderMode[1]->click();
}

