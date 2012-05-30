#pragma once

#include <QMainWindow>

class MainWindowEx2 : public QMainWindow {
		Q_OBJECT
	private:
		void setupUi();
		class GLWidgetEx2 * glWidget;
		class QDockWidget * uiRenderSettings;
		class QCheckBox * uiRenderKDTree;
		class QDoubleSpinBox * uiPointSize;
		class QLabel * uiPointSizeLabel;

	public:
		MainWindowEx2(QMainWindow * parent = 0);
		~MainWindowEx2();

	private slots:
};
