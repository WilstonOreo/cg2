#include "mainwindowex2.h"
#include "glwidgetex2.h"

#include <QCheckBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QDockWidget>
#include <QGridLayout>
#include <QSignalMapper>

void MainWindowEx2::setupUi() {
	glWidget = new GLWidgetEx2();
	//glWidget->setMouseTracking(true);

	setCentralWidget(glWidget);
	resize(950, 670);

	setFocusPolicy(Qt::NoFocus);
	setContextMenuPolicy(Qt::ActionsContextMenu);
	setAutoFillBackground(true);

	QGridLayout * layout = new QGridLayout();


	uiRenderKDTree = new QCheckBox("Render KDTree");
	layout->addWidget(uiRenderKDTree, 0, 0, 1, 2);

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

	uiPointSizeGridCasteljauLabel = new QLabel("dC. Grid Point size");
	layout->addWidget(uiPointSizeGridCasteljauLabel, 4, 0);

	uiPointSizeGridCasteljau = new QDoubleSpinBox();
	uiPointSizeGridCasteljau->setSingleStep(0.2);
	layout->addWidget(uiPointSizeGridCasteljau, 4, 1);

	uiGridSizeCasteljauLabel = new QLabel("dC. Grid size");
	layout->addWidget(uiGridSizeCasteljauLabel, 5, 0);

	uiGridSizeCasteljau = new QSpinBox();
	uiGridSizeCasteljau->setSingleStep(1);
	layout->addWidget(uiGridSizeCasteljau, 5, 1);

	uiRenderMode[0] = new QRadioButton("Render points");
	layout->addWidget(uiRenderMode[0], 6, 0, 1, 2);
	uiRenderMode[1] = new QRadioButton("Render interpolated grid");
	layout->addWidget(uiRenderMode[1], 7, 0, 1, 2);
	uiRenderMode[2] = new QRadioButton("Render with de Casteljau");
	layout->addWidget(uiRenderMode[2], 8, 0, 1, 2);

	uiRenderSettings = new QDockWidget();

	QWidget * tmpWidget = new QWidget();
	uiRenderSettings->setWidget(tmpWidget);
	tmpWidget->setLayout(layout);

	addDockWidget(Qt::RightDockWidgetArea,uiRenderSettings);
} // setupUi

MainWindowEx2::MainWindowEx2(QMainWindow * parent) : QMainWindow(parent) {
	setupUi();

	connect(uiPointSizeSource,SIGNAL(valueChanged(double)), glWidget, SLOT(setPointSizeSource(double)));
	connect(uiPointSizeGrid,SIGNAL(valueChanged(double)), glWidget, SLOT(setPointSizeGrid(double)));
	connect(uiGridSize,SIGNAL(valueChanged(int)), glWidget, SLOT(setGridSize(int)));
	connect(uiPointSizeGridCasteljau,SIGNAL(valueChanged(double)), glWidget, SLOT(setPointSizeGridCasteljau(double)));
	connect(uiGridSizeCasteljau,SIGNAL(valueChanged(int)), glWidget, SLOT(setGridSizeCasteljau(int)));
	connect(uiRenderKDTree,SIGNAL(stateChanged(int)), glWidget, SLOT(setDrawKDTree(int)));
	QSignalMapper * qsm = new QSignalMapper(this);
	for (int i = 0; i < 3; ++i) {
		qsm->setMapping(uiRenderMode[i], i);
		connect(uiRenderMode[i],SIGNAL(clicked()), qsm, SLOT(map()));
	}
	connect(qsm,SIGNAL(mapped(int)), glWidget, SLOT(setRenderMode(int)));

	uiPointSizeSource->setValue(2);

	uiPointSizeGrid->setValue(1);
	uiGridSize->setValue(10);

	uiPointSizeGridCasteljau->setValue(1);
	uiGridSizeCasteljau->setValue(25);

	uiRenderMode[1]->setChecked(true);
}

