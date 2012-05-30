#include "mainwindowex2.h"
#include "glwidgetex2.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QDockWidget>
#include <QGridLayout>

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
	connect(uiRenderKDTree,SIGNAL(stateChanged(int)), glWidget, SLOT(setDrawKDTree(int)));

	uiPointSizeSource->setValue(2);
	uiPointSizeGrid->setValue(1);
}

MainWindowEx2::~MainWindowEx2() {
}
