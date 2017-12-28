#include "qt_opengl_framework.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	srand(time(NULL));
	Qt_Opengl_Framework* w;
	w = Qt_Opengl_Framework::getInstance();
	w->show();
	return a.exec();
}
