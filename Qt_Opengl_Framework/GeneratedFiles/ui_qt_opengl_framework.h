/********************************************************************************
** Form generated from reading UI file 'qt_opengl_framework.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_OPENGL_FRAMEWORK_H
#define UI_QT_OPENGL_FRAMEWORK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qt_Opengl_FrameworkClass
{
public:
    QAction *actionExit;
    QAction *actionOpen;
    QAction *actionGray;
    QAction *actionUniform_Quantization;
    QAction *actionPopulosity;
    QAction *actionNaive_Threshold_Dithering;
    QAction *actionBrightness_Preserving_Threshold_Dithering;
    QAction *actionRandom;
    QAction *actionClustered;
    QAction *actionFloyd_Steinberg;
    QAction *actionColor_Floyd_Steinberg;
    QAction *actionBox;
    QAction *actionBartlett;
    QAction *actionGaussian;
    QAction *actionArbitrary_Size_Gaussian;
    QAction *actionHalf;
    QAction *actionDouble;
    QAction *actionArbitrary_Uniform_Scale;
    QAction *actionEdge;
    QAction *actionEnhance;
    QAction *actionRotate;
    QAction *actionResample_src;
    QAction *actionOver;
    QAction *actionIn;
    QAction *actionOut;
    QAction *actionAtop;
    QAction *actionXor;
    QAction *actionSave;
    QAction *actionNPR;
    QAction *actionAbout;
    QAction *actionFind_Pattern;
    QWidget *centralWidget;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuColor;
    QMenu *menuDithering;
    QMenu *menuFiltering;
    QMenu *menuResize;
    QMenu *menuComposing;
    QMenu *menuNPR;
    QMenu *menuPattern;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *Qt_Opengl_FrameworkClass)
    {
        if (Qt_Opengl_FrameworkClass->objectName().isEmpty())
            Qt_Opengl_FrameworkClass->setObjectName(QStringLiteral("Qt_Opengl_FrameworkClass"));
        Qt_Opengl_FrameworkClass->resize(600, 600);
        actionExit = new QAction(Qt_Opengl_FrameworkClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionOpen = new QAction(Qt_Opengl_FrameworkClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionGray = new QAction(Qt_Opengl_FrameworkClass);
        actionGray->setObjectName(QStringLiteral("actionGray"));
        actionUniform_Quantization = new QAction(Qt_Opengl_FrameworkClass);
        actionUniform_Quantization->setObjectName(QStringLiteral("actionUniform_Quantization"));
        actionPopulosity = new QAction(Qt_Opengl_FrameworkClass);
        actionPopulosity->setObjectName(QStringLiteral("actionPopulosity"));
        actionNaive_Threshold_Dithering = new QAction(Qt_Opengl_FrameworkClass);
        actionNaive_Threshold_Dithering->setObjectName(QStringLiteral("actionNaive_Threshold_Dithering"));
        actionBrightness_Preserving_Threshold_Dithering = new QAction(Qt_Opengl_FrameworkClass);
        actionBrightness_Preserving_Threshold_Dithering->setObjectName(QStringLiteral("actionBrightness_Preserving_Threshold_Dithering"));
        actionRandom = new QAction(Qt_Opengl_FrameworkClass);
        actionRandom->setObjectName(QStringLiteral("actionRandom"));
        actionClustered = new QAction(Qt_Opengl_FrameworkClass);
        actionClustered->setObjectName(QStringLiteral("actionClustered"));
        actionFloyd_Steinberg = new QAction(Qt_Opengl_FrameworkClass);
        actionFloyd_Steinberg->setObjectName(QStringLiteral("actionFloyd_Steinberg"));
        actionColor_Floyd_Steinberg = new QAction(Qt_Opengl_FrameworkClass);
        actionColor_Floyd_Steinberg->setObjectName(QStringLiteral("actionColor_Floyd_Steinberg"));
        actionBox = new QAction(Qt_Opengl_FrameworkClass);
        actionBox->setObjectName(QStringLiteral("actionBox"));
        actionBartlett = new QAction(Qt_Opengl_FrameworkClass);
        actionBartlett->setObjectName(QStringLiteral("actionBartlett"));
        actionGaussian = new QAction(Qt_Opengl_FrameworkClass);
        actionGaussian->setObjectName(QStringLiteral("actionGaussian"));
        actionArbitrary_Size_Gaussian = new QAction(Qt_Opengl_FrameworkClass);
        actionArbitrary_Size_Gaussian->setObjectName(QStringLiteral("actionArbitrary_Size_Gaussian"));
        actionHalf = new QAction(Qt_Opengl_FrameworkClass);
        actionHalf->setObjectName(QStringLiteral("actionHalf"));
        actionDouble = new QAction(Qt_Opengl_FrameworkClass);
        actionDouble->setObjectName(QStringLiteral("actionDouble"));
        actionArbitrary_Uniform_Scale = new QAction(Qt_Opengl_FrameworkClass);
        actionArbitrary_Uniform_Scale->setObjectName(QStringLiteral("actionArbitrary_Uniform_Scale"));
        actionEdge = new QAction(Qt_Opengl_FrameworkClass);
        actionEdge->setObjectName(QStringLiteral("actionEdge"));
        actionEnhance = new QAction(Qt_Opengl_FrameworkClass);
        actionEnhance->setObjectName(QStringLiteral("actionEnhance"));
        actionRotate = new QAction(Qt_Opengl_FrameworkClass);
        actionRotate->setObjectName(QStringLiteral("actionRotate"));
        actionResample_src = new QAction(Qt_Opengl_FrameworkClass);
        actionResample_src->setObjectName(QStringLiteral("actionResample_src"));
        actionOver = new QAction(Qt_Opengl_FrameworkClass);
        actionOver->setObjectName(QStringLiteral("actionOver"));
        actionIn = new QAction(Qt_Opengl_FrameworkClass);
        actionIn->setObjectName(QStringLiteral("actionIn"));
        actionOut = new QAction(Qt_Opengl_FrameworkClass);
        actionOut->setObjectName(QStringLiteral("actionOut"));
        actionAtop = new QAction(Qt_Opengl_FrameworkClass);
        actionAtop->setObjectName(QStringLiteral("actionAtop"));
        actionXor = new QAction(Qt_Opengl_FrameworkClass);
        actionXor->setObjectName(QStringLiteral("actionXor"));
        actionSave = new QAction(Qt_Opengl_FrameworkClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionNPR = new QAction(Qt_Opengl_FrameworkClass);
        actionNPR->setObjectName(QStringLiteral("actionNPR"));
        actionAbout = new QAction(Qt_Opengl_FrameworkClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionFind_Pattern = new QAction(Qt_Opengl_FrameworkClass);
        actionFind_Pattern->setObjectName(QStringLiteral("actionFind_Pattern"));
        centralWidget = new QWidget(Qt_Opengl_FrameworkClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 581, 541));
        Qt_Opengl_FrameworkClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Qt_Opengl_FrameworkClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuColor = new QMenu(menuBar);
        menuColor->setObjectName(QStringLiteral("menuColor"));
        menuDithering = new QMenu(menuBar);
        menuDithering->setObjectName(QStringLiteral("menuDithering"));
        menuFiltering = new QMenu(menuBar);
        menuFiltering->setObjectName(QStringLiteral("menuFiltering"));
        menuResize = new QMenu(menuBar);
        menuResize->setObjectName(QStringLiteral("menuResize"));
        menuComposing = new QMenu(menuBar);
        menuComposing->setObjectName(QStringLiteral("menuComposing"));
        menuNPR = new QMenu(menuBar);
        menuNPR->setObjectName(QStringLiteral("menuNPR"));
        menuPattern = new QMenu(menuBar);
        menuPattern->setObjectName(QStringLiteral("menuPattern"));
        Qt_Opengl_FrameworkClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Qt_Opengl_FrameworkClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Qt_Opengl_FrameworkClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuColor->menuAction());
        menuBar->addAction(menuDithering->menuAction());
        menuBar->addAction(menuFiltering->menuAction());
        menuBar->addAction(menuResize->menuAction());
        menuBar->addAction(menuComposing->menuAction());
        menuBar->addAction(menuNPR->menuAction());
        menuBar->addAction(menuPattern->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);
        menuFile->addSeparator();
        menuFile->addAction(actionAbout);
        menuColor->addAction(actionGray);
        menuColor->addAction(actionUniform_Quantization);
        menuColor->addAction(actionPopulosity);
        menuDithering->addAction(actionNaive_Threshold_Dithering);
        menuDithering->addAction(actionBrightness_Preserving_Threshold_Dithering);
        menuDithering->addAction(actionRandom);
        menuDithering->addAction(actionClustered);
        menuDithering->addAction(actionFloyd_Steinberg);
        menuDithering->addAction(actionColor_Floyd_Steinberg);
        menuFiltering->addAction(actionBox);
        menuFiltering->addAction(actionBartlett);
        menuFiltering->addAction(actionGaussian);
        menuFiltering->addAction(actionArbitrary_Size_Gaussian);
        menuFiltering->addAction(actionEdge);
        menuFiltering->addAction(actionEnhance);
        menuResize->addAction(actionHalf);
        menuResize->addAction(actionDouble);
        menuResize->addAction(actionArbitrary_Uniform_Scale);
        menuResize->addAction(actionRotate);
        menuComposing->addAction(actionOver);
        menuComposing->addAction(actionIn);
        menuComposing->addAction(actionOut);
        menuComposing->addAction(actionAtop);
        menuComposing->addAction(actionXor);
        menuNPR->addAction(actionNPR);
        menuPattern->addAction(actionFind_Pattern);

        retranslateUi(Qt_Opengl_FrameworkClass);

        QMetaObject::connectSlotsByName(Qt_Opengl_FrameworkClass);
    } // setupUi

    void retranslateUi(QMainWindow *Qt_Opengl_FrameworkClass)
    {
        Qt_Opengl_FrameworkClass->setWindowTitle(QApplication::translate("Qt_Opengl_FrameworkClass", "Qt_Opengl_Framework", Q_NULLPTR));
        actionExit->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Exit", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Open", Q_NULLPTR));
        actionGray->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Gray", Q_NULLPTR));
        actionUniform_Quantization->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Uniform Quantization", Q_NULLPTR));
        actionPopulosity->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Populosity", Q_NULLPTR));
        actionNaive_Threshold_Dithering->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Naive Threshold Dithering", Q_NULLPTR));
        actionBrightness_Preserving_Threshold_Dithering->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Brightness Preserving Threshold", Q_NULLPTR));
        actionRandom->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Random ", Q_NULLPTR));
        actionClustered->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Clustered ", Q_NULLPTR));
        actionFloyd_Steinberg->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Floyd-Steinberg ", Q_NULLPTR));
        actionColor_Floyd_Steinberg->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Color Floyd-Steinberg ", Q_NULLPTR));
        actionBox->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Box", Q_NULLPTR));
        actionBartlett->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Bartlett", Q_NULLPTR));
        actionGaussian->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Gaussian ", Q_NULLPTR));
        actionArbitrary_Size_Gaussian->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Arbitrary-Size Gaussian ", Q_NULLPTR));
        actionHalf->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Half ", Q_NULLPTR));
        actionDouble->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Double ", Q_NULLPTR));
        actionArbitrary_Uniform_Scale->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Arbitrary Uniform Scale", Q_NULLPTR));
        actionEdge->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Edge", Q_NULLPTR));
        actionEnhance->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Enhance", Q_NULLPTR));
        actionRotate->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Rotate", Q_NULLPTR));
        actionResample_src->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "resample_src", Q_NULLPTR));
        actionOver->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Over", Q_NULLPTR));
        actionIn->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "In", Q_NULLPTR));
        actionOut->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Out", Q_NULLPTR));
        actionAtop->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Atop", Q_NULLPTR));
        actionXor->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Xor", Q_NULLPTR));
        actionSave->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Save", Q_NULLPTR));
        actionNPR->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "NPR", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "About", Q_NULLPTR));
        actionFind_Pattern->setText(QApplication::translate("Qt_Opengl_FrameworkClass", "Find Pattern", Q_NULLPTR));
        label->setText(QString());
        menuFile->setTitle(QApplication::translate("Qt_Opengl_FrameworkClass", "File", Q_NULLPTR));
        menuColor->setTitle(QApplication::translate("Qt_Opengl_FrameworkClass", "Color", Q_NULLPTR));
        menuDithering->setTitle(QApplication::translate("Qt_Opengl_FrameworkClass", "Dithering", Q_NULLPTR));
        menuFiltering->setTitle(QApplication::translate("Qt_Opengl_FrameworkClass", "Filtering", Q_NULLPTR));
        menuResize->setTitle(QApplication::translate("Qt_Opengl_FrameworkClass", "Size&&Rotate", Q_NULLPTR));
        menuComposing->setTitle(QApplication::translate("Qt_Opengl_FrameworkClass", "Composing", Q_NULLPTR));
        menuNPR->setTitle(QApplication::translate("Qt_Opengl_FrameworkClass", "Bouns", Q_NULLPTR));
        menuPattern->setTitle(QApplication::translate("Qt_Opengl_FrameworkClass", "Pattern", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Qt_Opengl_FrameworkClass: public Ui_Qt_Opengl_FrameworkClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_OPENGL_FRAMEWORK_H
