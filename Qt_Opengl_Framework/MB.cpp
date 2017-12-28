#include "MB.h"
#include <QtGui>
Ui_MyMessageBox::Ui_MyMessageBox( QWidget *parent/*=0*/ )
{
	this->resize(220,100);
	this->setWindowTitle("InputWindow");
	pushButton = new QPushButton(this);
	pushButton->setObjectName(QStringLiteral("pushButton"));
	pushButton->setEnabled(false);
	pushButton->setText("Ok");
	pushButton->setGeometry(QRect(20, 60, 75, 23));
	pushButton_2 = new QPushButton(this);
	pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
	pushButton_2->setText("Cancel");
	pushButton_2->setGeometry(QRect(130, 60, 75, 23));
	
	label = new QLabel(this);
	label->setObjectName(QStringLiteral("label"));
	label->setGeometry(QRect(45, 15, 61, 20));
	label->setText("Value");
	lineEdit = new QLineEdit(this);
	lineEdit->setObjectName(QStringLiteral("lineEdit"));
	lineEdit->setGeometry(QRect(120, 15, 50, 20));
	connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(checkvalue(const QString &)));
	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
}

Ui_MyMessageBox::~Ui_MyMessageBox()
{

}

void Ui_MyMessageBox::checkvalue(const QString &text )
{
	pushButton->setEnabled(!text.isEmpty());
}



float Ui_MyMessageBox::getMyFloat()
{
	return lineEdit->text().toFloat();
}

int Ui_MyMessageBox::getMyInt()
{
	return lineEdit->text().toInt();
}

