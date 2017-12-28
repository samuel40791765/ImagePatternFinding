#include "ImageView.h"

ImageView::ImageView(QWidget *parent)
	: QGLWidget(parent)
{
	resetArcball();
}

ImageView::~ImageView()
{
}

void ImageView::resetArcball()
//========================================================================
{
	// Set up the camera to look at the world
	// these parameters might seem magical, and they kindof are
	// a little trial and error goes a long way
	arcball.setup(this, 40, 500, .2f, .4f, 0);
}

void ImageView::paintGL()
{
	glViewport(0, 0, width(), height());
	glClearColor(0, 0, .3f, 0);		// background should be blue

									// we need to clear out the stencil buffer since we'll use
									// it for shadows
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH);

	// Blayne prefers GL_DIFFUSE
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// prepare for projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//GLUquadric *q;
	//drawStuff();
	setProjection();		// put the code to set up matrices here

							//######################################################################
							// TODO: 
							// you might want to set the lighting up differently. if you do, 
							// we need to set up the lights AFTER setting up the projection
							//######################################################################
							// enable the lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);



	// top view only needs one light
	if (this->camera == 1) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	}
	else {
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}

	//*********************************************************************
	//
	// * set the light parameters
	//
	//**********************************************************************
	GLfloat lightPosition1[] = { 0,1,1,0 }; // {50, 200.0, 50, 1.0};
	GLfloat lightPosition2[] = { 0, 1, 0, 0 };
	GLfloat lightPosition3[] = { 0, -1, 0, 0 };
	GLfloat yellowLight[] = { 0.5f, 0.5f, .1f, 1.0 };
	GLfloat whiteLight[] = { 1.0f, 1.0f, 1.0f, 1.0 };
	GLfloat blueLight[] = { .1f,.1f,.3f,1.0 };
	GLfloat grayLight[] = { .3f, .3f, .3f, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grayLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);

	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blueLight);
	setupFloor();
	glDisable(GL_LIGHTING);
	drawFloor(2000, 10);

	glEnable(GL_LIGHTING);
	setupObjects();
	glTranslatef(-150, 5, -150);
	
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < img_height - 1; i++) {
		for (int j = 0; j < img_width - 1; j++) {
			int offset_rgba = i*img_width + j;
			if (imagemap[offset_rgba] > 200)
				glColor3f(255, 255, 255);
			else if (imagemap[offset_rgba] > 150)
				glColor3f(0, 255, 0);
			else if (imagemap[offset_rgba] > 100)
				glColor3f(0, 0, 255);
			else if (imagemap[offset_rgba] > 50)
				glColor3f(255,0, 0);
			else
				glColor3f(0,0,0);
			glVertex3f(i , imagemap[offset_rgba] * 0.5f, j );
			glVertex3f(i , imagemap[offset_rgba + 1] * 0.5f, (j + 1));
			glVertex3f((i + 1) , imagemap[offset_rgba + img_width] * 0.5f, j );
		}
	}
	glEnd();

	glTranslatef(0, -135,0);
	glBegin(GL_LINES);
	for (int i = 0; i < img_height - 1; i++) {
		for (int j = 0; j < img_width - 1; j++) {
			int offset_rgba = i*img_width + j;
			if (imagemap[offset_rgba] > 200)
				glColor3f(255, 255, 255);
			else if (imagemap[offset_rgba] > 150)
				glColor3f(0, 255, 0);
			else if (imagemap[offset_rgba] > 100)
				glColor3f(0, 0, 255);
			else if (imagemap[offset_rgba] > 50)
				glColor3f(255, 0, 0);
			else
				glColor3f(0, 0, 0);
			glVertex3f(i, imagemap[offset_rgba] * 0.5f, j);
			glVertex3f(i, imagemap[offset_rgba + 1] * 0.5f, (j + 1));
			glVertex3f((i + 1), imagemap[offset_rgba + img_width] * 0.5f, j);
		}
	}
	glEnd();
}

void ImageView::
setProjection()
//========================================================================
{
	// Compute the aspect ratio (we'll need it)
	float aspect = static_cast<float>(width()) / static_cast<float>(height());

	// Check whether we use the world camp
	if (this->camera == 0) {
		arcball.setProjection(false);
		update();
		// Or we use the top cam
	}
	else if (this->camera == 1) {
		float wi, he;
		if (aspect >= 1) {
			wi = 110;
			he = wi / aspect;
		}
		else {
			he = 110;
			wi = he * aspect;
		}

		// Set up the top camera drop mode to be orthogonal and set
		// up proper projection matrix
		glMatrixMode(GL_PROJECTION);
		glOrtho(-wi, wi, he, -he, 200, -200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(-90, 1, 0, 0);
		update();
	}


	else {
#ifdef EXAMPLE_SOLUTION
		trainCamView(this, aspect);
#endif
		update();
	}
}

void ImageView::
doPick(int mx, int my)
//========================================================================
{
	// since we'll need to do some GL stuff so we make this window as 
	// active window
	makeCurrent();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPickMatrix((double)mx, (double)(viewport[3] - my),
		5, 5, viewport);

	// now set up the projection
	setProjection();
}
