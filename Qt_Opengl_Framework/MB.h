#include <QtGui>
#include <QMainWindow>
#include <QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>



class Ui_MyMessageBox :public QWidget
{
	Q_OBJECT

public:
	Ui_MyMessageBox(QWidget *parent=0);
	~Ui_MyMessageBox();
	QPushButton *pushButton;
	QPushButton *pushButton_2;
	QLabel *label;
	QLineEdit *lineEdit;
	float getMyFloat();
	int getMyInt();
public slots:
	void checkvalue(const QString &text);
	
};