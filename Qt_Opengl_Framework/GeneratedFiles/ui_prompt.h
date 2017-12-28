/********************************************************************************
** Form generated from reading UI file 'prompt.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROMPT_H
#define UI_PROMPT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QPushButton *GLButton;
    QPushButton *DirectXButton;
    QLabel *label;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->setWindowModality(Qt::ApplicationModal);
        Dialog->resize(401, 228);
        Dialog->setContextMenuPolicy(Qt::NoContextMenu);
        Dialog->setSizeGripEnabled(true);
        Dialog->setModal(true);
        GLButton = new QPushButton(Dialog);
        GLButton->setObjectName(QStringLiteral("GLButton"));
        GLButton->setGeometry(QRect(20, 80, 361, 51));
        DirectXButton = new QPushButton(Dialog);
        DirectXButton->setObjectName(QStringLiteral("DirectXButton"));
        DirectXButton->setGeometry(QRect(20, 140, 361, 51));
        label = new QLabel(Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 371, 20));
        label->setLayoutDirection(Qt::RightToLeft);
        label->setAlignment(Qt::AlignCenter);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", Q_NULLPTR));
        GLButton->setText(QApplication::translate("Dialog", "OpenGL", Q_NULLPTR));
        DirectXButton->setText(QApplication::translate("Dialog", "DirectX", Q_NULLPTR));
        label->setText(QApplication::translate("Dialog", "Choose Render", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROMPT_H
