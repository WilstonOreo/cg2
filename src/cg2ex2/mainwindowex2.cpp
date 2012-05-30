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

	uiPointSizeLabel = new QLabel("Point size");
	layout->addWidget(uiPointSizeLabel, 1, 0);

	uiPointSize = new QDoubleSpinBox();
	uiPointSize->setSingleStep(0.2);
	layout->addWidget(uiPointSize, 1, 1);

	uiRenderSettings = new QDockWidget();

	QWidget * tmpWidget = new QWidget();
	uiRenderSettings->setWidget(tmpWidget);
	tmpWidget->setLayout(layout);

	addDockWidget(Qt::RightDockWidgetArea,uiRenderSettings);
} // setupUi

MainWindowEx2::MainWindowEx2(QMainWindow * parent) : QMainWindow(parent) {
	setupUi();

	connect(uiPointSize,SIGNAL(valueChanged(double)), glWidget, SLOT(setPointSize(double)));
	connect(uiRenderKDTree,SIGNAL(stateChanged(int)), glWidget, SLOT(setDrawKDTree(int)));

	uiPointSize->setValue(2);
}

MainWindowEx2::~MainWindowEx2() {
}
