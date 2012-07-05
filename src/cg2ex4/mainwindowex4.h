#pragma once

#include <QMainWindow>

class MainWindowEx4 : public QMainWindow
{
  Q_OBJECT
private:
  void setupUi();

  class GLWidgetEx4 * glWidget;
  class QDockWidget * uiRenderSettings;

  class QCheckBox * uiRenderGrid;
  class QRadioButton * uiRenderMode[4];

  class QDoubleSpinBox * uiPointSizeSource;
  class QLabel * uiPointSizeSourceLabel;

  class QDoubleSpinBox * uiPointSizeGrid;
  class QLabel * uiPointSizeGridLabel;

  class QSpinBox * uiGridSize;
  class QLabel * uiGridSizeLabel;

  class QPushButton * uiRaytraceBtn;
  class QPushButton * uiSpheretraceBtn;

public:
  MainWindowEx4(QMainWindow * parent = 0);
  ~MainWindowEx4() {}

private slots:
};
