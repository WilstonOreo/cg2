#pragma once

#include <QMainWindow>

class MainWindowEx2 : public QMainWindow {
		Q_OBJECT
	private:
		void setupUi();

		class GLWidgetEx2 * glWidget;
		class QDockWidget * uiRenderSettings;

		class QCheckBox * uiRenderKDTree;
		class QRadioButton * uiRenderMode[3];

		class QDoubleSpinBox * uiPointSizeSource;
		class QLabel * uiPointSizeSourceLabel;

		class QDoubleSpinBox * uiPointSizeGrid;
		class QLabel * uiPointSizeGridLabel;

		class QSpinBox * uiGridSize;
		class QLabel * uiGridSizeLabel;

		class QDoubleSpinBox * uiPointSizeGridCasteljau;
		class QLabel * uiPointSizeGridCasteljauLabel;

		class QSpinBox * uiGridSizeCasteljau;
		class QLabel * uiGridSizeCasteljauLabel;

	public:
		MainWindowEx2(QMainWindow * parent = 0);
		~MainWindowEx2() {}

	private slots:
};
