#pragma once
#include <assert.h>
#include <vector>
#include <QtOpenGL/QGLWidget>
#include <GL/GLU.h>
#include <QtOpenGL/QtOpenGL>
//#pragma comment(lib,"opengl32.lib")
//#pragma comment(lib,"glu32.lib")
#include <GL\GLU.h>
#include "Utilites\ArcBallCam.H"
#include "Utilites\3DUtils.h"
#include "Utilites\Pnt3f.H"
#include <QObject>


class ImageView : public QGLWidget
{

public:
	ImageView(QWidget *parent = 0);
	~ImageView();

public:
	// overrides of important window things
	//virtual int handle(int);
	virtual void paintGL();
	// all of the actual drawing happens in this routine
	// it has to be encapsulated, since we draw differently if
	// we're drawing shadows (no colors, for example)
	//void drawStuff(bool doingShadows = false);

	// setup the projection - assuming that the projection stack has been
	// cleared for you
	void setProjection();

	// Reset the Arc ball control
	void resetArcball();

	// pick a point (for when the mouse goes down)
	void doPick(int mx, int my);

	/*void drawTrain(float);
	void drawGround();*/

	//void loadTexture2D(QString str, GLuint &textureID);
	//void initTextures();

public:
	ArcBallCam		arcball;			// keep an ArcBall for the UI
	int				selectedCube;  // simple - just remember which cube is selected

	unsigned char* imagemap;
	int img_width;
	int img_height;

	int DIVIDE_LINE;
	int camera;
	int curve;
	int track;
	int cars = 1;
	float energy = 0;
	float kinetic = 0;
	float lowestpoint;
	bool isrun;
	bool terrain = true;
	bool doarc = true;
	bool music = true;
	bool showstruct = false;
	float time = 0;
	bool init = false;
	GLuint train_ID;


	/*vector<Pnt3f> interpos;
	vector<Pnt3f> interorient;
	*/
	GLuint now_id;
	std::map<std::string, GLuint> textures;
	std::string err;
};
