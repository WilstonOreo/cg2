/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Wed May 9 13:24:06 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>
#include <QtGui/QDockWidget>

#include "glwidget.h"

#include <iostream>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    GLWidget *glWidget;
    QDockWidget *grpSelectionMethod;
    QDockWidget *grpRenderSettings;
    QRadioButton *btnInRadius;
    QRadioButton *btnKNearest;
    QSpinBox *spinBox;
    QSpinBox *boxVertexId;
    QDoubleSpinBox *doubleSpinBox;
    QCheckBox *boxRenderKDTree;
    QDoubleSpinBox *doubleSpinBox_3;
    QLabel *label;
    QLabel *lbVertexId;

    void setupUi(QMainWindow *MainWindow)
    {
        glWidget = new GLWidget(MainWindow);
        glWidget->resize(950,670);
        glWidget->setMouseTracking(true);

        MainWindow->setCentralWidget(glWidget);
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(950, 670);
        
        MainWindow->setFocusPolicy(Qt::NoFocus);
        MainWindow->setContextMenuPolicy(Qt::ActionsContextMenu);
        MainWindow->setAutoFillBackground(true);



        grpSelectionMethod = new QDockWidget(MainWindow);
        grpRenderSettings = new QDockWidget(MainWindow);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea,grpSelectionMethod);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea,grpRenderSettings);
            
        btnInRadius = new QRadioButton(grpSelectionMethod);
        btnInRadius->setObjectName(QString::fromUtf8("btnInRadius"));
        btnInRadius->setGeometry(QRect(20, 30, 116, 22));
        btnInRadius->setChecked(true);
        btnKNearest = new QRadioButton(grpSelectionMethod);
        btnKNearest->setObjectName(QString::fromUtf8("btnKNearest"));
        btnKNearest->setGeometry(QRect(20, 60, 116, 22));
        btnKNearest->setChecked(false);
        spinBox = new QSpinBox(grpSelectionMethod);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(140, 60, 91, 27));
        spinBox->setValue(10);
        doubleSpinBox = new QDoubleSpinBox(grpSelectionMethod);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setGeometry(QRect(140, 30, 91, 27));
        doubleSpinBox->setValue(2);
        doubleSpinBox->setSingleStep(0.05);

        boxVertexId = new QSpinBox(grpSelectionMethod);
        boxVertexId->setObjectName(QString::fromUtf8("boxVertexId"));
        boxVertexId->setGeometry(QRect(140, 90, 91, 27));
        boxVertexId->setValue(200);
        boxVertexId->setRange(0,10000);
        lbVertexId = new QLabel(grpSelectionMethod);
        lbVertexId->setObjectName(QString::fromUtf8("lbVertexId"));
        lbVertexId->setGeometry(QRect(20, 90, 66, 17));

        boxRenderKDTree = new QCheckBox(grpRenderSettings);
        boxRenderKDTree->setObjectName(QString::fromUtf8("boxRenderKDTree"));
        boxRenderKDTree->setGeometry(QRect(10, 30, 191, 22));
        doubleSpinBox_3 = new QDoubleSpinBox(grpRenderSettings);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
        doubleSpinBox_3->setGeometry(QRect(140, 60, 91, 27));
        doubleSpinBox_3->setValue(2);
        doubleSpinBox_3->setSingleStep(0.2);

        label = new QLabel(grpRenderSettings);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 60, 66, 17));
        boxRenderKDTree->raise();
        doubleSpinBox_3->raise();
        label->raise();
        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
   
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CG2 Projekt von Bjoern, Elmar und Michael", 0, QApplication::UnicodeUTF8));
        grpSelectionMethod->setWindowTitle(QApplication::translate("MainWindow", "Selection method", 0, QApplication::UnicodeUTF8));
        btnInRadius->setText(QApplication::translate("MainWindow", "In Radius:", 0, QApplication::UnicodeUTF8));
        btnKNearest->setText(QApplication::translate("MainWindow", "k Nearest:", 0, QApplication::UnicodeUTF8));
        grpRenderSettings->setWindowTitle(QApplication::translate("MainWindow", "Render settings", 0, QApplication::UnicodeUTF8));
        boxRenderKDTree->setText(QApplication::translate("MainWindow", "Render KDTree", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Point size:", 0, QApplication::UnicodeUTF8));
        lbVertexId->setText(QApplication::translate("MainWindow", "Vertex Id:", 0, QApplication::UnicodeUTF8));
    
      } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
