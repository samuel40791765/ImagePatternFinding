#define IMAGEWINDOW_H  

#include <QMainWindow>
#include "ui_ImageWindow.h"
#include <QtOpenGL/QGLWidget>  
#include <QtGui/QtGui>  
#include <QtOpenGL/QtOpenGL>  
#include <GL/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib") 
#include "ui_ImageModel.h"
#include "ImageView.h"
#include "Utilites\ArcBallCam.H"

class ImageWindow : public QMainWindow
{


public:
	ImageWindow(QWidget *parent = Q_NULLPTR);
	~ImageWindow();
	bool canpan;
	bool isHover;
	bool yplane;
	ImageView*	trainview;
	void setImageMap(unsigned char* image, int width, int height);

private:
	Ui::ImageWindow ui;
	unsigned char* imagemap;

protected:
	bool eventFilter(QObject *watched, QEvent *e);
};
