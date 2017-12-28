#pragma once
#include <OpenglWidget.h>
#include <ImageView.h>
#include <ImageWindow.h>

class Stroke { // Data structure for holding painterly strokes.
public:
	Stroke(void);
	Stroke(unsigned int radius, unsigned int x, unsigned int y,
		unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	// data
	unsigned int radius, x, y;	// Location for the stroke
	unsigned char r, g, b, a;	// Color
};

class Application :  public OpenglWidget
{
public:
	Application();
	~Application();
	unsigned char* Application::To_RGB(void);
	void Application::RGBA_To_RGB(unsigned char *rgba, unsigned char *rgb);

	// File
	void loadSecondaryImge(QString filePath);
	void openImage(QString filePath);
	void saveImage(QString filePath);
	void reload();

	// Color
	void Gray();
	void Quant_Uniform();
	void Quant_Populosity();

	// Dithering
	void Dither_Threshold();
	void Dither_Random();
	void Dither_FS();
	void Dither_Bright();
	void Dither_Cluster();
	void Dither_Color();

	// Filter
	void filtering(double filter[][5]);
	void filtering(double **filter, int n);
	void Filter_Box();
	void Filter_Bartlett();
	void Filter_Gaussian();
	void Filter_Gaussian_N(unsigned int N);
	unsigned char * Filter_Gaussian_N_array(unsigned int N, unsigned char * rgb);
	void Filter_Edge();
	void Filter_Enhance();

	// Size
	void Half_Size();
	void Double_Size();
	void Resize(float scale);
	void Rotate(float angleDegrees);
	void resample_src(int u, int v, float ww, unsigned char* rgba);

	// Composing
	enum _CompImage { _over, _in, _out, _atop, _xor };
	void Comp_image(int tMethod);
	void Comp_Over();
	void Comp_In();
	void Comp_Out();
	void Comp_Atop();
	void Comp_Xor();

	// Special
	void NPR_Paint();
	void Paint_Stroke(const Stroke& s);
	void NPR_Paint_Layer(unsigned char *tCanvas, unsigned char *tReferenceImage, int tBrushSize);

	//Pattern
	void FindPattern();
protected:
	QImage mImageSrc, mImageSrcSecond;
	QImage mImageDst;
	void createScene(void);

	void renew();

	bool bRenew;

	// Image data
	unsigned char* img_data,*img_data2;
	unsigned char *temp;
	int img_width,img_width2;
	int img_height,img_height2;
	

	QVector<QImage> patterns;
	

	// you should swap the rr and bb if you read img_data directly without using To_RGB(), cause OGRE is in BGR format
	enum {rr, gg, bb, aa}; 
	enum {BLACK = 0, WHITE = 255};

	
	Qt_Opengl_Framework* ui_instance;

};
