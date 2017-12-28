/********************************************************************************
** Form generated from reading UI file 'About.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QLabel *label;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QStringLiteral("About"));
        About->resize(251, 113);
        label = new QLabel(About);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 0, 241, 101));

        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "About", Q_NULLPTR));
        label->setText(QApplication::translate("About", "<html><head/><body><p>\345\246\202\346\236\234\346\234\211\346\241\206\346\236\266\344\270\212\347\232\204Bug\346\210\226\350\221\227\344\273\273\344\275\225\346\212\200\350\241\223\344\270\212\347\232\204\345\225\217\351\241\214</p><p>\346\255\241\350\277\216\345\257\204\344\277\241\345\210\260\345\212\251\346\225\231\344\277\241\347\256\261</p><p><a href=\"M10315001@mail.ntust.edu.tw\"><span style=\" text-decoration: underline; color:#0000ff;\">\350\221\211\350\207\264\347\245\245(M10315001@mail.ntust.edu.tw)</span></a></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
