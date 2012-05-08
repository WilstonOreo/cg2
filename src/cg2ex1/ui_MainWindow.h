/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Tue May 8 22:16:44 2012
**      by: Qt User Interface Compiler version 4.8.1
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

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QWidget *widget;
    QGroupBox *grpSelectionMethod;
    QRadioButton *btnInRadius;
    QRadioButton *btnKNearest;
    QSpinBox *spinBox;
    QDoubleSpinBox *doubleSpinBox;
    QGroupBox *grpRenderSettings;
    QCheckBox *boxRenderKDTree;
    QDoubleSpinBox *doubleSpinBox_3;
    QLabel *label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1097, 886);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setFocusPolicy(Qt::NoFocus);
        MainWindow->setContextMenuPolicy(Qt::ActionsContextMenu);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(widget, 0, 0, 2, 1);

        grpSelectionMethod = new QGroupBox(centralwidget);
        grpSelectionMethod->setObjectName(QString::fromUtf8("grpSelectionMethod"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(grpSelectionMethod->sizePolicy().hasHeightForWidth());
        grpSelectionMethod->setSizePolicy(sizePolicy2);
        grpSelectionMethod->setMinimumSize(QSize(300, 0));
        grpSelectionMethod->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        grpSelectionMethod->setFlat(false);
        grpSelectionMethod->setCheckable(false);
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

        gridLayout->addWidget(grpSelectionMethod, 0, 1, 1, 1);

        grpRenderSettings = new QGroupBox(centralwidget);
        grpRenderSettings->setObjectName(QString::fromUtf8("grpRenderSettings"));
        QSizePolicy sizePolicy3(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(grpRenderSettings->sizePolicy().hasHeightForWidth());
        grpRenderSettings->setSizePolicy(sizePolicy3);
        grpRenderSettings->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        grpRenderSettings->setFlat(false);
        grpRenderSettings->setCheckable(false);
        boxRenderKDTree = new QCheckBox(grpRenderSettings);
        boxRenderKDTree->setObjectName(QString::fromUtf8("boxRenderKDTree"));
        boxRenderKDTree->setGeometry(QRect(10, 30, 191, 22));
        doubleSpinBox_3 = new QDoubleSpinBox(grpRenderSettings);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
        doubleSpinBox_3->setGeometry(QRect(140, 60, 91, 27));
        doubleSpinBox_3->setValue(2);
        label = new QLabel(grpRenderSettings);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 60, 66, 17));
        boxRenderKDTree->raise();
        doubleSpinBox_3->raise();
        label->raise();
        widget->raise();

        gridLayout->addWidget(grpRenderSettings, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        grpSelectionMethod->setTitle(QApplication::translate("MainWindow", "Selection method", 0, QApplication::UnicodeUTF8));
        btnInRadius->setText(QApplication::translate("MainWindow", "In Radius:", 0, QApplication::UnicodeUTF8));
        btnKNearest->setText(QApplication::translate("MainWindow", "k Nearest:", 0, QApplication::UnicodeUTF8));
        grpRenderSettings->setTitle(QApplication::translate("MainWindow", "Render settings", 0, QApplication::UnicodeUTF8));
        boxRenderKDTree->setText(QApplication::translate("MainWindow", "Render KDTree", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Point size:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
