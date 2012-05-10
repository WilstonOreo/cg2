#pragma once

#include "ui_MainWindow.h"
 
class MainWindow : public QMainWindow, public Ui::MainWindow
{
        Q_OBJECT
 private:
        public:
                MainWindow (QMainWindow *parent = 0);
                ~MainWindow();

        private slots:
                void setPointSize();
                void setRadius();
                void setDrawKDTree();
                void setKNearest();
                void setSelectionMode();
                void setVertexId();

};

