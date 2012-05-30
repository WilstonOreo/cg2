#include "mainwindowex2.h"
#include "glwidgetex2.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QDockWidget>
#include <QGridLayout>

void MainWindowEx2::setupUi() {
	//glWidget = new GLWidgetEx2();
	//glWidget->setMouseTracking(true);

	//setCentralWidget(glWidget);
	resize(950, 670);

	setFocusPolicy(Qt::NoFocus);
	setContextMenuPolicy(Qt::ActionsContextMenu);
	setAutoFillBackground(true);

	uiRenderSettings = new QDockWidget();
	addDockWidget(Qt::RightDockWidgetArea,uiRenderSettings);

	QGridLayout * layout = new QGridLayout();

	layout->addWidget(uiRenderSettings);

	uiRenderKDTree = new QCheckBox("Render KDTree");
	layout->addWidget(uiRenderKDTree);

	uiPointSize = new QDoubleSpinBox();
	uiPointSize->setValue(2);
	uiPointSize->setSingleStep(0.2);
	layout->addWidget(uiPointSize);

	uiRenderSettings->setLayout(layout);

	uiPointSizeLabel = new QLabel("Point size");

} // setupUi

MainWindowEx2::MainWindowEx2(QMainWindow * parent) : QMainWindow(parent) {
	//connect(uiPointSize,SIGNAL(editingFinished()), glWidget, SLOT(setPointSize()));
	connect(uiPointSize,SIGNAL(valueChanged(double)), glWidget, SLOT(setPointSize(double)));
	connect(uiRenderKDTree,SIGNAL(stateChanged(int)), glWidget, SLOT(setDrawKDTree(int)));
}

MainWindowEx2::~MainWindowEx2() {
}
