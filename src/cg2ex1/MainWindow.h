#pragma once

#include "ui_MainWindow.h"
 
class MainWindow : public QMainWindow, public Ui::MainWindow
{
        Q_OBJECT
 
        public:
                MainWindow (QMainWindow *parent = 0);
                ~MainWindow();
};

