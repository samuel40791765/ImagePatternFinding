/********************************************************************************
** Form generated from reading UI file 'ImageModel.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEMODEL_H
#define UI_IMAGEMODEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageModel
{
public:

    void setupUi(QWidget *ImageModel)
    {
        if (ImageModel->objectName().isEmpty())
            ImageModel->setObjectName(QStringLiteral("ImageModel"));
        ImageModel->resize(400, 300);

        retranslateUi(ImageModel);

        QMetaObject::connectSlotsByName(ImageModel);
    } // setupUi

    void retranslateUi(QWidget *ImageModel)
    {
        ImageModel->setWindowTitle(QApplication::translate("ImageModel", "ImageModel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ImageModel: public Ui_ImageModel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEMODEL_H
