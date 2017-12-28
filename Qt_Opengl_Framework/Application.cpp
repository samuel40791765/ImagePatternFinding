#include "Application.h"
#include "qt_opengl_framework.h"
#include <vector>
#include <Map>
#include <chrono>
#include <cuda_runtime.h>

#include <helper_cuda.h>
#include <helper_functions.h>
#include <patternvalidate.h>
using namespace std;

Application::Application()
{

}
Application::~Application()
{

}
//****************************************************************************
//
// * ªì©lµe­±¡A¨ÃÅã¥ÜNtust.png¹ÏÀÉ
// 
//============================================================================
void Application::createScene(void)
{

	ui_instance = Qt_Opengl_Framework::getInstance();

}

//****************************************************************************
//
// * ¥´¶}«ü©w¹ÏÀÉ
// 
//============================================================================
void Application::openImage(QString filePath)
{
	mImageSrc.load(filePath);
	mImageDst.load(filePath);

	renew();

	img_data = mImageSrc.bits();
	img_width = mImageSrc.width();
	img_height = mImageSrc.height();

	ui_instance->ui.label->setFixedHeight(img_height);
	ui_instance->ui.label->setFixedWidth(img_width);
}
//****************************************************************************
//
// * ¨ê·sµe­±
// 
//============================================================================
void Application::renew()
{
	ui_instance = Qt_Opengl_Framework::getInstance();

	ui_instance->ui.label->clear();
	ui_instance->ui.label->setPixmap(QPixmap::fromImage(mImageDst));
	ui_instance->ui.label->setFixedHeight(img_height);
	ui_instance->ui.label->setFixedWidth(img_width);

	std::cout << "Renew" << std::endl;
}

//****************************************************************************
//
// * µe­±ªì©l¤Æ
// 
//============================================================================
void Application::reload()
{
	ui_instance = Qt_Opengl_Framework::getInstance();

	ui_instance->ui.label->clear();
	ui_instance->ui.label->setPixmap(QPixmap::fromImage(mImageSrc));
}

//****************************************************************************
//
// * Àx¦s¹ÏÀÉ
// 
//============================================================================
void Application::saveImage(QString filePath)
{
	mImageDst.save(filePath);
}

//****************************************************************************
//
// * ±N¹ÏÀÉ¸ê®ÆÂà´«¬°RGB¦â±m¸ê®Æ
// 
//============================================================================
unsigned char* Application::To_RGB(void)
{
	unsigned char *rgb = new unsigned char[img_width * img_height * 3];
	int i, j;

	if (!img_data)
		return NULL;

	// Divide out the alpha
	for (i = 0; i < img_height; i++)
	{
		int in_offset = i * img_width * 4;
		int out_offset = i * img_width * 3;

		for (j = 0; j < img_width; j++)
		{
			RGBA_To_RGB(img_data + (in_offset + j * 4), rgb + (out_offset + j * 3));
		}
	}

	return rgb;
}

void Application::RGBA_To_RGB(unsigned char *rgba, unsigned char *rgb)
{
	const unsigned char	BACKGROUND[3] = { 0, 0, 0 };

	unsigned char  alpha = rgba[3];

	if (alpha == 0)
	{
		rgb[0] = BACKGROUND[0];
		rgb[1] = BACKGROUND[1];
		rgb[2] = BACKGROUND[2];
	}
	else
	{
		float	alpha_scale = (float)255 / (float)alpha;
		int	val;
		int	i;

		for (i = 0; i < 3; i++)
		{
			val = (int)floor(rgba[i] * alpha_scale);
			if (val < 0)
				rgb[i] = 0;
			else if (val > 255)
				rgb[i] = 255;
			else
				rgb[i] = val;
		}
	}
}
//------------------------Color------------------------

///////////////////////////////////////////////////////////////////////////////
//
//  Convert image to grayscale.  Red, green, and blue channels should all 
//  contain grayscale value.  Alpha channel shoould be left unchanged.  Return
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Gray()
{
	unsigned char *rgb = this->To_RGB();

	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			unsigned char gray = 0.299 * rgb[offset_rgb + rr] + 0.587 * rgb[offset_rgb + gg] + 0.114 * rgb[offset_rgb + bb];

			for (int k = 0; k < 3; k++)
				img_data[offset_rgba + k] = gray;
			img_data[offset_rgba + aa] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Convert the image to an 8 bit image using uniform quantization.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Quant_Uniform()
{
	unsigned char *rgb = this->To_RGB();
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;

			img_data[offset_rgba + rr] = ((int)rgb[offset_rgb + rr] / 32) * 32;
			img_data[offset_rgba + gg] = ((int)rgb[offset_rgb + gg] / 32) * 32;
			img_data[offset_rgba + bb] = ((int)rgb[offset_rgb + bb] / 64) * 64;
			img_data[offset_rgba + aa] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Convert the image to an 8 bit image using populosity quantization.  
//  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
struct pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	int pixelcolor;

	bool const operator== (const pixel &o) const {
		return r == o.r && g == o.g && b == o.b;
	}
	bool const operator<(const pixel &o) const {
		if (r < o.r)
		{
			return true;
		}
		else if (r == o.r)
		{
			if (g < o.g)
			{
				return true;
			}
			else if (g == o.g)
			{
				if (b < o.b)
					return true;
				else if (b == o.b)
					return false;
			}
		}

		return false;
	}

};
void Application::Quant_Populosity()
{
	map<pixel, int> frequency;
	vector<pixel> mostfrequent;
	unsigned char *rgb = this->To_RGB();
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;

			img_data[offset_rgba + rr] = ((int)rgb[offset_rgb + rr] / 8) * 8;
			img_data[offset_rgba + gg] = ((int)rgb[offset_rgb + gg] / 8) * 8;
			img_data[offset_rgba + bb] = ((int)rgb[offset_rgb + bb] / 8) * 8;
			img_data[offset_rgba + aa] = WHITE;
		}
	}
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgba = i*img_width * 4 + j * 4;
			pixel temp;
			temp.r = img_data[(offset_rgba + rr)];
			temp.g = img_data[(offset_rgba + gg)];
			temp.b = img_data[(offset_rgba + bb)];
			temp.pixelcolor = temp.r*temp.g*temp.b;
			if (frequency.find(temp) != frequency.end()) {
				frequency[temp] = frequency[temp] + 1;
			}
			else {
				frequency[temp] = 1;
			}

		}
	}


	map<pixel, int>::iterator it;
	for (int i = 0; i < 256; i++) {
		pixel temp;
		int max = 0;
		for (it = frequency.begin(); it != frequency.end(); ++it) {
			if (max < it->second) {
				temp = it->first;
				max = it->second;
			}
		}
		mostfrequent.push_back(temp);
		it = frequency.find(temp);
		frequency.erase(it);
	}

	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgba = i*img_width * 4 + j * 4;
			int leastdiff = 999999999;
			int leastdiffiterator = 0;
			for (int i = 0; i < 256; i++) {
				int temp = pow(img_data[offset_rgba + rr] - mostfrequent[i].r, 2) + pow(img_data[offset_rgba + gg] - mostfrequent[i].g, 2) + pow(img_data[offset_rgba + bb] - mostfrequent[i].b, 2);
				if (temp < leastdiff) {
					leastdiff = temp;
					leastdiffiterator = i;
				}
			}
			img_data[offset_rgba + rr] = mostfrequent[leastdiffiterator].r;
			img_data[offset_rgba + gg] = mostfrequent[leastdiffiterator].g;
			img_data[offset_rgba + bb] = mostfrequent[leastdiffiterator].b;
			img_data[offset_rgba + aa] = WHITE;
		}
	}
	/**AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	for (int i = 0; i < 256; i++) {
		printf("%d %d %d %d\n", mostfrequent[i].r, mostfrequent[i].g, mostfrequent[i].b, mostfrequent[i].pixelcolor);
	}**/
	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}

//------------------------Dithering------------------------

///////////////////////////////////////////////////////////////////////////////
//
//  Dither the image using a threshold of 1/2.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Dither_Threshold()
{
	Gray();
	int threshold = 255 / 2;
	unsigned char *rgb = this->To_RGB();
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			for (int k = 0; k < 3; k++) {
				img_data[offset_rgba + k] = rgb[offset_rgb + k] > threshold ? 255 : 0;
			}

			img_data[offset_rgba + aa] = WHITE;
		}
	}


	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Dither image using random dithering.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Dither_Random()
{
	Gray();
	unsigned char *rgb = this->To_RGB();
	srand(time(NULL));

	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			float randnum = ((rand() % 41) - 20) * 0.01;
			int threshold = 256 * (0.5 + randnum);
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			for (int k = 0; k < 3; k++) {
				img_data[offset_rgba + k] = rgb[offset_rgb + k] > threshold ? 255 : 0;
			}

			img_data[offset_rgba + aa] = WHITE;

		}
	}

	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Perform Floyd-Steinberg dithering on the image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Dither_FS()
{
	Gray();
	unsigned char *rgb = this->To_RGB();
	for (int i = 0; i < img_height; i = i + 2)
	{
		for (int j = 0; j < img_width - 1; j++)
		{
			if (i > img_height - 2)
				break;
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			int threshold = 255 * 0.5;
			float e = 0;

			for (int k = 0; k < 3; k++) {
				img_data[offset_rgba + k] = rgb[offset_rgb + k] >= threshold ? 255 : 0;
				e = rgb[offset_rgb + k] - img_data[offset_rgba + k];
				rgb[offset_rgb + k + 3] += e * 7 / 16.0;
				rgb[offset_rgb + k + img_width * 3 * 1] += e * 5 / 16.0;
				rgb[offset_rgb + k + img_width * 3 * 1 - 3] += e * 3 / 16.0;
				rgb[offset_rgb + k + img_width * 3 * 1 + 3] += e * 1 / 16.0;
			}


			img_data[offset_rgba + aa] = WHITE;
		}

		for (int j = img_width - 1; j > 0; j--)
		{
			if (i > img_height - 4)
				break;
			int offset_rgb = (i + 1)*img_width * 3 + j * 3;
			int offset_rgba = (i + 1)*img_width * 4 + j * 4;
			int threshold = 255 * 0.5;
			float e = 0;

			/**AllocConsole();
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			freopen("CONOUT$", "w", stderr);**/
			for (int k = 0; k < 3; k++) {
				img_data[offset_rgba + k] = rgb[offset_rgb + k] >= threshold ? 255 : 0;
				e = rgb[offset_rgb + k] - img_data[offset_rgba + k];
				rgb[offset_rgb + k - 3] += e * 7 / 16.0;
				rgb[offset_rgb + k + img_width * 3 * 1] += e * 5 / 16.0;
				rgb[offset_rgb + k + img_width * 3 * 1 + 3] += e * 3 / 16.0;
				rgb[offset_rgb + k + img_width * 3 * 1 - 3] += e * 1 / 16.0;
			}

			//printf("%d %d %d %d\n", rgb[offset_rgb + rr - 3],rgb[offset_rgb + rr + img_width * 3 * 1], rgb[offset_rgb + rr + img_width * 3 * 1 + 3], rgb[offset_rgb + rr + img_width * 3 * 1 - 3]);

			img_data[offset_rgba + aa] = WHITE;
		}

	}




	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Dither the image while conserving the average brightness.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Dither_Bright()
{
	Gray();
	unsigned char *rgb = this->To_RGB();

	vector<int> allvalues;
	int sum = 0;
	//AllocConsole();
	//freopen("CONIN$", "r", stdin);
	//freopen("CONOUT$", "w", stdout);
	//freopen("CONOUT$", "w", stderr);
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			bool exist = false;
			for (int check = 0; check < allvalues.size(); check++) {
				if (allvalues[check] == rgb[offset_rgb + rr]) {
					exist = true;
					break;
				}
			}
			if (!exist)
				allvalues.push_back(rgb[offset_rgb + rr]);
		}
	}
	for (int i = 0; i < allvalues.size(); i++)
		sum = sum + allvalues[i];


	int threshold = sum / (allvalues.size());


	//printf("%d ", threshold); 
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			for (int k = 0; k < 3; k++) {
				img_data[offset_rgba + k] = rgb[offset_rgb + k] > threshold ? 255 : 0;
			}
			img_data[offset_rgba + aa] = WHITE;
		}
	}


	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Perform clustered differing of the image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Dither_Cluster()
{
	Gray();
	unsigned char *rgb = this->To_RGB();
	for (int i = 0; i < img_height; i = i + 4)
	{
		for (int j = 0; j < img_width; j = j + 4)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			int threshold = 255;
			float matrix[4][4] = { { 0.7059, 0.3529, 0.5882, 0.2353 }, { 0.0588, 0.9412, 0.8235, 0.4118 } ,{ 0.4706, 0.7647, 0.8824, 0.1176 }, { 0.1765, 0.5294, 0.2941 ,0.6471 } };
			for (int matrixcolumn = 0; matrixcolumn < 4; matrixcolumn++) {
				for (int matrixrow = 0; matrixrow < 4; matrixrow++) {
					for (int k = 0; k < 3; k++) {
						if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 < img_height*img_width * 3) {
							img_data[offset_rgba + matrixcolumn*img_width * 4 + matrixrow * 4 + k] = rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr] >= threshold*matrix[matrixcolumn][matrixrow] ? 255 : 0;
						}
					}

				}

			}
			img_data[offset_rgba + aa] = WHITE;

		}
	}


	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Convert the image to an 8 bit image using Floyd-Steinberg dithering over
//  a uniform quantization - the same quantization as in Quant_Uniform.
//  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Dither_Color()
{
	unsigned char *rgb = this->To_RGB();
	//int rchannel[9] = { 0,32,64,96,128,160,192,224,255 };
	//int gchannel[9] = { 0,32,64,96,128,160,192,224,255 };
	//int bchannel[5] = { 0,64,128,192,255 };

	//double rchannel[8] = { 0,36,73,109,146,182,219,255 };
	//double gchannel[8] = { 0,36,73,109,146,182,219,255 };
	//double bchannel[4] = { 0,85,170,255 };

	double rchannel[8] = { 0,64,128,192,255 };
	double gchannel[8] = { 0,36,73,109,146,182,219,255 };
	double bchannel[8] = { 0,36,73,109,146,182,219,255 };

	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			int thresholditerator = 0, tempiterator = 0;
			double e = 0;
			/**AllocConsole();
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			freopen("CONOUT$", "w", stderr);**/
			if (rgb[offset_rgb + rr] < 0) {
				rgb[offset_rgb + rr] = 0;
				e = rgb[offset_rgb + rr];
			}
			else if (rgb[offset_rgb + rr] > 255) {
				rgb[offset_rgb + rr] = 255;
				e = rgb[offset_rgb + rr] - 255;
			}
			else {
				while (rgb[offset_rgb + rr] >= rchannel[tempiterator]) {
					if (rgb[offset_rgb + rr] == rchannel[tempiterator])
						break;
					thresholditerator = tempiterator;
					tempiterator++;
				}
				//printf("%d \n",thresholditerator);
				if (rgb[offset_rgb + rr] - rchannel[thresholditerator] <= -1 * (rgb[offset_rgb + rr] - rchannel[thresholditerator + 1])) {
					img_data[offset_rgba + rr] = rchannel[thresholditerator];
					e = rgb[offset_rgb + rr] - rchannel[thresholditerator];
				}
				else {
					if (rchannel[thresholditerator + 1] == 255) {
						img_data[offset_rgba + rr] = rchannel[thresholditerator];
						e = rgb[offset_rgb + rr] - rchannel[thresholditerator];
					}
					img_data[offset_rgba + rr] = rchannel[thresholditerator + 1];
					e = rgb[offset_rgb + rr] - rchannel[thresholditerator + 1];
				}
			}
			if (j + 1 < img_width && j + 1 >= 0)
				rgb[offset_rgb + rr + 3] += e * 7 / 16.0;
			if (i != img_height - 1) {
				rgb[offset_rgb + rr + img_width * 3 * 1] += e * 5 / 16.0;
				if (j - 1 < img_width && j - 1 >= 0)
					rgb[offset_rgb + rr + img_width * 3 * 1 - 3] += e * 3 / 16.0;
				if (j + 1 < img_width && j + 1 >= 0)
					rgb[offset_rgb + rr + img_width * 3 * 1 + 3] += e * 1 / 16.0;
			}

			thresholditerator = 0; tempiterator = 0;
			if (rgb[offset_rgb + gg] < 0) {
				rgb[offset_rgb + gg] = 0;
				e = rgb[offset_rgb + gg];
			}
			else if (rgb[offset_rgb + gg] > 255) {
				rgb[offset_rgb + gg] = 255;
				e = rgb[offset_rgb + gg] - 255;
			}
			else {
				while (rgb[offset_rgb + gg] >= gchannel[tempiterator]) {
					if (rgb[offset_rgb + gg] == gchannel[tempiterator])
						break;
					thresholditerator = tempiterator;
					tempiterator++;
				}
				if (rgb[offset_rgb + gg] - gchannel[thresholditerator] <= -1 * (rgb[offset_rgb + gg] - gchannel[thresholditerator + 1])) {
					img_data[offset_rgba + gg] = gchannel[thresholditerator];
					e = rgb[offset_rgb + gg] - gchannel[thresholditerator];
				}
				else {
					if (gchannel[thresholditerator + 1] == 255) {
						img_data[offset_rgba + gg] = gchannel[thresholditerator];
						e = rgb[offset_rgb + gg] - gchannel[thresholditerator];
					}
					img_data[offset_rgba + gg] = gchannel[thresholditerator + 1];
					e = rgb[offset_rgb + gg] - gchannel[thresholditerator + 1];
				}
			}
			if (j + 1 < img_width && j + 1 >= 0)
				rgb[offset_rgb + gg + 3] += e * 7 / 16.0;
			if (i != img_height - 1) {
				rgb[offset_rgb + gg + img_width * 3 * 1] += e * 5 / 16.0;
				if (j - 1 < img_width && j - 1 >= 0)
					rgb[offset_rgb + gg + img_width * 3 * 1 - 3] += e * 3 / 16.0;
				if (j + 1 < img_width && j + 1 >= 0)
					rgb[offset_rgb + gg + img_width * 3 * 1 + 3] += e * 1 / 16.0;
			}

			thresholditerator = 0; tempiterator = 0;
			if (rgb[offset_rgb + bb] < 0) {
				rgb[offset_rgb + bb] = 0;
				e = rgb[offset_rgb + bb];
			}
			else if (rgb[offset_rgb + rr] > 255) {
				rgb[offset_rgb + bb] = 255;
				e = rgb[offset_rgb + bb] - 255;
			}
			else {
				while (rgb[offset_rgb + bb] >= bchannel[tempiterator]) {
					if (rgb[offset_rgb + bb] == bchannel[tempiterator])
						break;
					thresholditerator = tempiterator;
					tempiterator++;
				}
				if (rgb[offset_rgb + bb] - bchannel[thresholditerator] <= -1 * (rgb[offset_rgb + bb] - bchannel[thresholditerator + 1])) {
					img_data[offset_rgba + bb] = bchannel[thresholditerator];
					e = rgb[offset_rgb + bb] - bchannel[thresholditerator];
				}
				else {
					if (bchannel[thresholditerator + 1] == 255) {
						img_data[offset_rgba + bb] = bchannel[thresholditerator];
						e = rgb[offset_rgb + bb] - bchannel[thresholditerator];
					}
					img_data[offset_rgba + bb] = bchannel[thresholditerator + 1];
					e = rgb[offset_rgb + bb] - bchannel[thresholditerator + 1];
				}
			}
			if (j + 1 < img_width && j + 1 >= 0)
				rgb[offset_rgb + bb + 3] += e * 7 / 16.0;
			if (i != img_height - 1) {
				rgb[offset_rgb + bb + img_width * 3 * 1] += e * 5 / 16.0;
				if (j - 1 < img_width && j - 1 >= 0)
					rgb[offset_rgb + bb + img_width * 3 * 1 - 3] += e * 3 / 16.0;
				if (j + 1 < img_width && j + 1 >= 0)
					rgb[offset_rgb + bb + img_width * 3 * 1 + 3] += e * 1 / 16.0;
			}
			img_data[offset_rgba + aa] = WHITE;
		}
		i++;
		if (i == img_height - 2)
			break;
		for (int j = (img_width - 1); j >= 0; j--)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			int thresholditerator = 0, tempiterator = 0;
			double e = 0;
			if (rgb[offset_rgb + rr] < 0) {
				rgb[offset_rgb + rr] = 0;
				e = rgb[offset_rgb + rr];
			}
			else if (rgb[offset_rgb + rr] > 255) {
				rgb[offset_rgb + rr] = 255;
				e = rgb[offset_rgb + rr] - 255;
			}
			else {
				while (rgb[offset_rgb + rr] >= rchannel[tempiterator]) {
					if (rgb[offset_rgb + rr] == rchannel[tempiterator])
						break;
					thresholditerator = tempiterator;
					tempiterator++;
				}
				if (rgb[offset_rgb + rr] - rchannel[thresholditerator] <= -1 * (rgb[offset_rgb + rr] - rchannel[thresholditerator + 1])) {
					img_data[offset_rgba + rr] = rchannel[thresholditerator];
					e = rgb[offset_rgb + rr] - rchannel[thresholditerator];
				}
				else {
					if (rchannel[thresholditerator + 1] == 255) {
						img_data[offset_rgba + rr] = rchannel[thresholditerator];
						e = rgb[offset_rgb + rr] - rchannel[thresholditerator];
					}
					img_data[offset_rgba + rr] = rchannel[thresholditerator + 1];
					e = rgb[offset_rgb + rr] - rchannel[thresholditerator + 1];
				}
			}
			if (j - 1 < img_width && j - 1 >= 0)
				rgb[offset_rgb + rr - 3] += e * 7 / 16.0;
			if (i != img_height - 1) {
				rgb[offset_rgb + rr + img_width * 3 * 1] += e * 5 / 16.0;
				if (j + 1 < img_width && j + 1 >= 0)
					rgb[offset_rgb + rr + img_width * 3 * 1 + 3] += e * 3 / 16.0;
				if (j - 1 < img_width && j - 1 >= 0)
					rgb[offset_rgb + rr + img_width * 3 * 1 - 3] += e * 1 / 16.0;
			}

			thresholditerator = 0, tempiterator = 0;
			if (rgb[offset_rgb + gg] < 0) {
				rgb[offset_rgb + gg] = 0;
				e = rgb[offset_rgb + gg];
			}
			else if (rgb[offset_rgb + gg] > 255) {
				rgb[offset_rgb + gg] = 255;
				e = rgb[offset_rgb + gg] - 255;
			}
			else {
				while (rgb[offset_rgb + gg] >= gchannel[tempiterator]) {
					if (rgb[offset_rgb + gg] == gchannel[tempiterator])
						break;
					thresholditerator = tempiterator;
					tempiterator++;
				}
				if (rgb[offset_rgb + gg] - gchannel[thresholditerator] <= -1 * (rgb[offset_rgb + gg] - gchannel[thresholditerator + 1])) {
					img_data[offset_rgba + gg] = gchannel[thresholditerator];
					e = rgb[offset_rgb + gg] - gchannel[thresholditerator];
				}
				else {
					if (gchannel[thresholditerator + 1] == 255) {
						img_data[offset_rgba + gg] = gchannel[thresholditerator];
						e = rgb[offset_rgb + gg] - gchannel[thresholditerator];
					}
					img_data[offset_rgba + gg] = gchannel[thresholditerator + 1];
					e = rgb[offset_rgb + gg] - gchannel[thresholditerator + 1];
				}
			}
			if (j - 1 < img_width && j - 1 >= 0)
				rgb[offset_rgb + gg - 3] += e * 7 / 16.0;
			if (i != img_height - 1) {
				rgb[offset_rgb + gg + img_width * 3 * 1] += e * 5 / 16.0;
				if (j + 1 < img_width && j + 1 >= 0)
					rgb[offset_rgb + gg + img_width * 3 * 1 + 3] += e * 3 / 16.0;
				if (j - 1 < img_width && j - 1 >= 0)
					rgb[offset_rgb + gg + img_width * 3 * 1 - 3] += e * 1 / 16.0;
			}

			thresholditerator = 0, tempiterator = 0;
			if (rgb[offset_rgb + rr] < 0) {
				rgb[offset_rgb + rr] = 0;
				e = rgb[offset_rgb + rr];
			}
			else if (rgb[offset_rgb + rr] > 255) {
				rgb[offset_rgb + gg + rr] = 255;
				e = rgb[offset_rgb + rr] - 255;
			}
			else {
				while (rgb[offset_rgb + bb] >= bchannel[tempiterator]) {
					if (rgb[offset_rgb + bb] == bchannel[tempiterator])
						break;
					thresholditerator = tempiterator;
					tempiterator++;
				}
				if (rgb[offset_rgb + bb] - bchannel[thresholditerator] <= -1 * (rgb[offset_rgb + bb] - bchannel[thresholditerator + 1])) {
					img_data[offset_rgba + bb] = bchannel[thresholditerator];
					e = rgb[offset_rgb + bb] - bchannel[thresholditerator];
				}
				else {
					if (bchannel[thresholditerator + 1] == 255) {
						img_data[offset_rgba + bb] = bchannel[thresholditerator];
						e = rgb[offset_rgb + bb] - bchannel[thresholditerator];
					}
					img_data[offset_rgba + bb] = bchannel[thresholditerator + 1];
					e = rgb[offset_rgb + bb] - bchannel[thresholditerator + 1];
				}
			}
			if (j - 1 < img_width && j - 1 >= 0)
				rgb[offset_rgb + bb - 3] += e * 7 / 16.0;
			if (i != img_height - 1) {
				rgb[offset_rgb + bb + img_width * 3 * 1] += e * 5 / 16.0;
				if (j + 1 < img_width && j + 1 >= 0)
					rgb[offset_rgb + bb + img_width * 3 * 1 + 3] += e * 3 / 16.0;
				if (j - 1 < img_width && j - 1 >= 0)
					rgb[offset_rgb + bb + img_width * 3 * 1 - 3] += e * 1 / 16.0;
			}
			//printf("%d\n", e);
			img_data[offset_rgba + aa] = WHITE;
		}

	}
	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}

//------------------------Filter------------------------

///////////////////////////////////////////////////////////////////////////////
//
//     Filtering the img_data array by the filter from the parameters
//
///////////////////////////////////////////////////////////////////////////////
void Application::filtering(double filter[][5])
{
	unsigned char *rgb = this->To_RGB();



	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}

void Application::filtering(double **filter, int n)
{
	unsigned char *rgb = this->To_RGB();



	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Perform 5x5 box filter on this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Filter_Box()
{

	unsigned char *rgb = this->To_RGB();
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			int threshold = 255;
			double matrix[5][5] = { { 0.04,0.04,0.04,0.04,0.04 },{ 0.04,0.04,0.04,0.04,0.04 } ,{ 0.04,0.04,0.04,0.04,0.04 },{ 0.04,0.04,0.04,0.04,0.04 },{ 0.04,0.04,0.04,0.04,0.04 } };
			int filtersumrr = 0, filtersumgg = 0, filtersumbb = 0;
			for (int matrixcolumn = -2; matrixcolumn < 3; matrixcolumn++) {
				for (int matrixrow = -2; matrixrow < 3; matrixrow++) {
					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr > img_height*img_width * 3 - 1) {
						filtersumrr = filtersumrr + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + rr] * matrix[matrixcolumn + 2][matrixrow + 2];
					}
					else {
						filtersumrr = filtersumrr + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr] * matrix[matrixcolumn + 2][matrixrow + 2];

					}

					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr >img_height*img_width * 3 - 1) {
						filtersumgg = filtersumgg + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + gg] * matrix[matrixcolumn + 2][matrixrow + 2];
					}
					else {
						filtersumgg = filtersumgg + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg] * matrix[matrixcolumn + 2][matrixrow + 2];

					}

					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr >img_height*img_width * 3 - 1) {
						filtersumbb = filtersumbb + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + bb] * matrix[matrixcolumn + 2][matrixrow + 2];
					}
					else {
						filtersumbb = filtersumbb + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb] * matrix[matrixcolumn + 2][matrixrow + 2];

					}


				}

			}
			img_data[offset_rgba + rr] = filtersumrr;
			img_data[offset_rgba + gg] = filtersumgg;
			img_data[offset_rgba + bb] = filtersumbb;
			img_data[offset_rgba + aa] = WHITE;

		}
	}


	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Perform 5x5 Bartlett filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Filter_Bartlett()
{
	unsigned char *rgb = this->To_RGB();
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			int threshold = 255;
			double matrix[5][5] = { { 1,2,3,2,1 },{ 2,4,6,4,2 } ,{ 3,6,9,6,3 },{ 2,4,6,4,2 },{ 1,2,3,2,1 } };
			int filtersumrr = 0, filtersumgg = 0, filtersumbb = 0;
			for (int matrixcolumn = -2; matrixcolumn < 3; matrixcolumn++) {
				for (int matrixrow = -2; matrixrow < 3; matrixrow++) {
					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr >img_height*img_width * 3 - 1) {
						filtersumrr = filtersumrr + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + rr] * matrix[matrixcolumn + 2][matrixrow + 2] / 81.0;
					}
					else {
						filtersumrr = filtersumrr + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr] * matrix[matrixcolumn + 2][matrixrow + 2] / 81.0;

					}

					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg >img_height*img_width * 3 - 1) {
						filtersumgg = filtersumgg + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + gg] * matrix[matrixcolumn + 2][matrixrow + 2] / 81.0;
					}
					else {
						filtersumgg = filtersumgg + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg] * matrix[matrixcolumn + 2][matrixrow + 2] / 81.0;

					}

					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb >img_height*img_width * 3 - 1) {
						filtersumbb = filtersumbb + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + bb] * matrix[matrixcolumn + 2][matrixrow + 2] / 81.0;
					}
					else {
						filtersumbb = filtersumbb + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb] * matrix[matrixcolumn + 2][matrixrow + 2] / 81.0;

					}


				}

			}
			img_data[offset_rgba + rr] = filtersumrr;
			img_data[offset_rgba + gg] = filtersumgg;
			img_data[offset_rgba + bb] = filtersumbb;
			img_data[offset_rgba + aa] = WHITE;

		}
	}


	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Perform 5x5 Gaussian filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Filter_Gaussian()
{
	unsigned char *rgb = this->To_RGB();
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			int threshold = 255;
			double matrix[5][5] = { { 1,4,6,4,1 },{ 4,16,24,16,4 } ,{ 6,24,36,24,6 },{ 4,16,24,16,4 },{ 1,4,6,4,1 } };
			int filtersumrr = 0, filtersumgg = 0, filtersumbb = 0;
			for (int matrixcolumn = -2; matrixcolumn < 3; matrixcolumn++) {
				for (int matrixrow = -2; matrixrow < 3; matrixrow++) {
					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr >img_height*img_width * 3 - 1) {
						filtersumrr = filtersumrr + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + rr] * matrix[matrixcolumn + 2][matrixrow + 2] / 256.0;
					}
					else {
						filtersumrr = filtersumrr + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr] * matrix[matrixcolumn + 2][matrixrow + 2] / 256.0;

					}

					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg >img_height*img_width * 3 - 1) {
						filtersumgg = filtersumgg + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + gg] * matrix[matrixcolumn + 2][matrixrow + 2] / 256.0;
					}
					else {
						filtersumgg = filtersumgg + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg] * matrix[matrixcolumn + 2][matrixrow + 2] / 256.0;

					}

					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb >img_height*img_width * 3 - 1) {
						filtersumbb = filtersumbb + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + bb] * matrix[matrixcolumn + 2][matrixrow + 2] / 256.0;
					}
					else {
						filtersumbb = filtersumbb + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb] * matrix[matrixcolumn + 2][matrixrow + 2] / 256.0;

					}


				}

			}
			img_data[offset_rgba + rr] = filtersumrr;
			img_data[offset_rgba + gg] = filtersumgg;
			img_data[offset_rgba + bb] = filtersumbb;
			img_data[offset_rgba + aa] = WHITE;

		}
	}
	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Perform NxN Gaussian filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Filter_Gaussian_N(unsigned int N)
{
	if (N <= 1)
		return;
	float dividend = 0;
	int **matrix = new int*[N];
	for (int i = 0; i < N; i++)
		matrix[i] = new int[N];
	for (int i = 1; i <= N; i++) {
		int topfactorial = 1, bottomleftfact = 1, bottomrightfact = 1;
		for (int j = 1; j < N; j++)
			topfactorial = topfactorial*j;

		for (int j = 1; j < i; j++)
			bottomleftfact = bottomleftfact*j;

		for (int j = 1; j < N - i + 1; j++)
			bottomrightfact = bottomrightfact*j;

		matrix[0][i - 1] = topfactorial / (bottomleftfact*bottomrightfact);
	}
	for (int i = 0; i < N; i++)
		dividend = dividend + matrix[0][i];
	dividend = pow(dividend, 2);
	for (int i = 0; i < N; i++) {
		matrix[i][0] = matrix[0][i];
	}
	for (int i = 1; i < N; i++) {
		for (int j = 1; j < N; j++) {
			matrix[i][j] = matrix[i][0] * matrix[0][j];
		}
	}
	/**AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	for (int i = 0; i < N; i++) {
		for (int j = 0;  j < N; j++) {
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}
	printf("%f", dividend);**/
	unsigned char *rgb = this->To_RGB();
	int start = -1 * (N / 2);
	int stop = 1 + N / 2;
	if (N % 2 == 0)
		stop = N / 2;
	/**printf("%d\n", N);
	printf("%d\n", start);
	printf("%d\n", stop);**/
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			int filtersumrr = 0, filtersumgg = 0, filtersumbb = 0;
			for (int matrixcolumn = start; matrixcolumn < stop; matrixcolumn++) {
				for (int matrixrow = start; matrixrow < stop; matrixrow++) {
					//printf("%d\n",matrixrow);
					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr >img_height*img_width * 3 - 1) {
						filtersumrr = filtersumrr + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + rr] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)] / dividend;
					}
					else {
						filtersumrr = filtersumrr + (rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)]) / dividend;

					}

					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg >img_height *img_width * 3 - 1) {
						filtersumgg = filtersumgg + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + gg] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)] / dividend;
					}
					else {
						filtersumgg = filtersumgg + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)] / dividend;

					}

					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb >img_height*img_width * 3 - 1) {
						filtersumbb = filtersumbb + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + bb] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)] / dividend;
					}
					else {
						filtersumbb = filtersumbb + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)] / dividend;

					}


				}


			}
			//printf("%d %d %d\n", filtersumrr, filtersumgg, filtersumbb);
			img_data[offset_rgba + rr] = filtersumrr;
			img_data[offset_rgba + gg] = filtersumgg;
			img_data[offset_rgba + bb] = filtersumbb;
			img_data[offset_rgba + aa] = WHITE;

		}
	}
	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Perform 5x5 edge detect (high pass) filter on this image.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
unsigned char *Application::Filter_Gaussian_N_array(unsigned int N, unsigned char *rgb) {
	temp = new unsigned char[img_width*img_height * 4];
	if (N <= 1)
		return rgb;
	float dividend = 0;
	int **matrix = new int*[N];
	for (int i = 0; i < N; i++)
		matrix[i] = new int[N];
	for (int i = 1; i <= N; i++) {
		int topfactorial = 1, bottomleftfact = 1, bottomrightfact = 1;
		for (int j = 1; j < N; j++)
			topfactorial = topfactorial*j;

		for (int j = 1; j < i; j++)
			bottomleftfact = bottomleftfact*j;

		for (int j = 1; j < N - i + 1; j++)
			bottomrightfact = bottomrightfact*j;

		matrix[0][i - 1] = topfactorial / (bottomleftfact*bottomrightfact);
	}
	for (int i = 0; i < N; i++)
		dividend = dividend + matrix[0][i];
	dividend = pow(dividend, 2);
	for (int i = 0; i < N; i++) {
		matrix[i][0] = matrix[0][i];
	}
	for (int i = 1; i < N; i++) {
		for (int j = 1; j < N; j++) {
			matrix[i][j] = matrix[i][0] * matrix[0][j];
		}
	}


	int start = -1 * (N / 2);
	int stop = 1 + N / 2;
	if (N % 2 == 0)
		stop = N / 2;
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width * 4 + j * 4;
			int filtersumrr = 0, filtersumgg = 0, filtersumbb = 0;
			for (int matrixcolumn = start; matrixcolumn < stop; matrixcolumn++) {
				for (int matrixrow = start; matrixrow < stop; matrixrow++) {
					//printf("%d\n",matrixrow);
					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr >img_height*img_width * 3 - 1) {
						filtersumrr = filtersumrr + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + rr] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)] / dividend;
					}
					else {
						filtersumrr = filtersumrr + (rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + rr] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)]) / dividend;

					}

					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg >img_height *img_width * 3 - 1) {
						filtersumgg = filtersumgg + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + gg] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)] / dividend;
					}
					else {
						filtersumgg = filtersumgg + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + gg] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)] / dividend;

					}

					if (offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb < 0 || offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb >img_height*img_width * 3 - 1) {
						filtersumbb = filtersumbb + rgb[offset_rgb + -1 * matrixcolumn*img_width * 3 + -1 * matrixrow * 3 + bb] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)] / dividend;
					}
					else {
						filtersumbb = filtersumbb + rgb[offset_rgb + matrixcolumn*img_width * 3 + matrixrow * 3 + bb] * matrix[matrixcolumn + (-1 * start)][matrixrow + (-1 * start)] / dividend;

					}


				}


			}
			//printf("%d %d %d\n", filtersumrr, filtersumgg, filtersumbb);
			temp[offset_rgba + rr] = filtersumrr;
			temp[offset_rgba + gg] = filtersumgg;
			temp[offset_rgba + bb] = filtersumbb;
			temp[offset_rgba + aa] = WHITE;

		}
	}
	//delete[] rgb;
	return temp;
}

void Application::Filter_Edge()
{

}
///////////////////////////////////////////////////////////////////////////////
//
//  Perform a 5x5 enhancement filter to this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Filter_Enhance()
{
	unsigned char *rgb = this->To_RGB();



	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}

//------------------------Size------------------------

///////////////////////////////////////////////////////////////////////////////
//
//  Halve the dimensions of this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Half_Size()
{

	unsigned char *rgb = this->To_RGB();

	for (int i = 0; i < img_height / 2; i++)
	{
		for (int j = 0; j < img_width / 2; j++)
		{
			int offset_rgb = 2 * i*img_width * 3 + 2 * j * 3;
			int offset_rgba = i*(img_width / 2) * 4 + j * 4;
			for (int k = 0; k < 3; k++) {
				img_data[offset_rgba + k] = (rgb[offset_rgb + k] + rgb[offset_rgb + 3 + k] + rgb[offset_rgb + img_width * 3 + k] + rgb[offset_rgb + img_width * 3 + k + 3]) / 4.0;


				img_data[offset_rgba + aa] = 255;

			}



		}
	}
	img_width = img_width / 2;
	img_height = img_height / 2;
	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}
///////////////////////////////////////////////////////////////////////////////
//
//  Double the dimensions of this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Double_Size()
{
	unsigned char *rgb = this->To_RGB();
	img_data = new unsigned char[img_width*img_height * 16];
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = 2 * i*(img_width * 2) * 4 + 2 * j * 4;
			for (int k = 0; k < 3; k++) {
				img_data[offset_rgba + k] = rgb[offset_rgb + k];
				img_data[offset_rgba + 4 + k] = rgb[offset_rgb + k];
				img_data[offset_rgba + k + img_width * 2 * 4] = rgb[offset_rgb + k];
				img_data[offset_rgba + 4 + k + img_width * 2 * 4] = rgb[offset_rgb + k];


			}

			img_data[offset_rgba + aa] = WHITE;
			img_data[offset_rgba + aa + 4] = WHITE;
			img_data[offset_rgba + aa + img_width * 2 * 4] = WHITE;
			img_data[offset_rgba + aa + 4 + img_width * 2 * 4] = WHITE;

		}
	}
	img_width = img_width * 2;
	img_height = img_height * 2;
	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	Filter_Gaussian_N((unsigned int)3);
	//renew();

}
///////////////////////////////////////////////////////////////////////////////
//
//  resample_src for resize and rotate
//
///////////////////////////////////////////////////////////////////////////////
void Application::resample_src(int u, int v, float ww, unsigned char* rgba)
{

}

///////////////////////////////////////////////////////////////////////////////
//
//  Scale the image dimensions by the given factor.  The given factor is 
//	assumed to be greater than one.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Resize(float scale)
{
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}

//////////////////////////////////////////////////////////////////////////////
//
//  Rotate the image clockwise by the given angle.  Do not resize the 
//  image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

//credits for interpolation http://stackoverflow.com/questions/695080/how-do-i-rotate-an-image
void Application::Rotate(float angleDegrees)
{
	//float x = x*cos(Q) - y*sin(Q);
	//float y = x*sin(Q) + y*cos(Q);
	//angleDegrees = angleDegrees - 90;
	const double PI = 3.14159265;
	unsigned char *rgb = this->To_RGB();
	double cornersx[4] = { 0, cos(angleDegrees*PI / 180 * -1)*img_width - sin(angleDegrees*PI / 180 * -1)*img_height ,cos(angleDegrees*PI / 180 * -1)*img_width ,-1 * sin(angleDegrees*PI / 180 * -1)*img_height };
	double cornersy[4] = { 0, sin(angleDegrees*PI / 180 * -1)*img_width + cos(angleDegrees*PI / 180 * -1)*img_height ,sin(angleDegrees*PI / 180 * -1)*img_width ,cos(angleDegrees*PI / 180 * -1)*img_height };
	double largestx = 0, smallestx = 0;
	for (int i = 0; i < 4; i++) {
		if (cornersx[i] > largestx)
			largestx = cornersx[i];
		if (cornersx[i] < smallestx)
			smallestx = cornersx[i];
	}

	int new_width = largestx - smallestx;
	int largesty = 0, smallesty = 0;
	for (int i = 0; i < 4; i++) {
		if (cornersy[i] > largesty)
			largesty = cornersy[i];
		if (cornersy[i] < smallesty)
			smallesty = cornersy[i];
	}
	int new_height = largesty - smallesty;
	img_data = new unsigned char[new_width*new_height * 16];
	for (int i = 0; i < new_width*new_height * 4; i++) {
		img_data[i] = 0;
	}

	/*AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	printf("%f\n", cos(angleDegrees*PI / 180));
	printf("%f\n", sin(angleDegrees*PI / 180));
	for (int i = 0; i < 4; i++)
		printf("%f ", cornersx[i]);
	printf("\n");
	for (int i = 0; i < 4; i++)
		printf("%f ", cornersy[i]);*/
	for (int i = 0; i < new_height; i++) {
		for (int j = 0; j < new_width; j++) {
			double offsetx = smallestx > 0 ? smallestx : -1 * smallestx;
			double offsety = smallesty > 0 ? smallesty : -1 * smallesty;
			int tempi = i - offsety;
			int tempj = j - offsetx;
			int offset_rgba = i*new_width * 4 + j * 4;
			double u = ((double)tempj*cos(angleDegrees*PI / 180.0) - (double)tempi*sin(angleDegrees*PI / 180.0));
			double v = ((double)sin(angleDegrees*PI / 180.0)*tempj + (double)cos(angleDegrees*PI / 180.0)*tempi);
			if ((u >= 0 && v >= 0) && (u < img_width && v < img_height)) {
				int offset_rgb = (int)v*img_width * 3 + (int)u * 3;
				for (int k = 0; k < 3; k++) {
					img_data[offset_rgba + k] = rgb[offset_rgb + k];
				}
				img_data[offset_rgba + aa] = WHITE;
			}
			else {
				for (int k = 0; k < 3; k++) {
					img_data[offset_rgba + k] = 0;
				}
				img_data[offset_rgba + aa] = WHITE;
			}
		}
	}
	/*for (int i = 0; i<img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{

			int offsetx = smallestx > 0 ? smallestx : -1 * smallestx;
			int offsety = smallesty > 0 ? smallesty : -1 * smallesty;
			int offset_rgb = i*img_width * 3 + j * 3;
			int newx = j*cos(angleDegrees*PI / 180 * -1) - i*sin(angleDegrees*PI / 180 * -1);
			int newy = sin(angleDegrees*PI / 180 * -1)*j + cos(angleDegrees*PI / 180 * -1)*i;
			int offset_rgba = (newy + offsety)*new_width * 4 + (newx + offsetx) * 4;

			for (int k = 0; k < 3; k++) {
				img_data[offset_rgba + k] = rgb[offset_rgb + k];
				if(img_data[offset_rgba + 4 + k]==0)
					img_data[offset_rgba + 4 + k] = rgb[offset_rgb + k];
				if (img_data[offset_rgba + k + new_width * 4] == 0)
					img_data[offset_rgba + k + new_width * 4] = rgb[offset_rgb + k];
				if (img_data[offset_rgba + 4 + k + new_width * 4] == 0)
					img_data[offset_rgba + 4 + k + new_width * 4] = rgb[offset_rgb + k];
			}

			img_data[offset_rgba + aa] = WHITE;


		}
	}
	for (int i = 0; i < new_height; i++) {
		for (int j = 0; j < new_width; j++) {
			int offset_rgb = i*new_width * 4 + j * 4;
			for (int k = 0; k < 3; k++) {
				if (img_data[offset_rgb + k] == 0)
					img_data[offset_rgb + k] = img_data[offset_rgb + k + 4];
			}
		}
	}*/
	img_width = new_width;
	img_height = new_height;
	delete[] rgb;
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
	//Filter_Gaussian_N(3);
}

//------------------------Composing------------------------


void Application::loadSecondaryImge(QString filePath)
{
	mImageSrcSecond.load(filePath);

	renew();

	img_data2 = mImageSrcSecond.bits();
	img_width2 = mImageSrcSecond.width();
	img_height2 = mImageSrcSecond.height();
}

//////////////////////////////////////////////////////////////////////////
//
//	Composite the image A and image B by Over, In, Out, Xor and Atom. 
//
//////////////////////////////////////////////////////////////////////////
void Application::Comp_image(int tMethod)
{
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//      Composite the current image over the given image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Comp_Over()
{
	if (img_height == img_height2 && img_width == img_width2)
	{

	}
	else
	{
		std::cout << "Images not the same size" << std::endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image "in" the given image.  See lecture notes for 
//  details.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Comp_In()
{
	if (img_height == img_height2 && img_width == img_width2)
	{

	}
	else
	{
		std::cout << "Images not the same size" << std::endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image "out" the given image.  See lecture notes for 
//  details.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Comp_Out()
{
	if (img_height == img_height2 && img_width == img_width2)
	{

	}
	else
	{
		std::cout << "Images not the same size" << std::endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//      Composite current image "atop" given image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Comp_Atop()
{
	if (img_height == img_height2 && img_width == img_width2)
	{

	}
	else
	{
		std::cout << "Images not the same size" << std::endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image with given image using exclusive or (XOR).  Return
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Comp_Xor()
{
	if (img_height == img_height2 && img_width == img_width2)
	{

	}
	else
	{
		std::cout << "Images not the same size" << std::endl;
	}
}

//------------------------NPR------------------------

///////////////////////////////////////////////////////////////////////////////
//
//      Run simplified version of Hertzmann's painterly image filter.
//      You probably will want to use the Draw_Stroke funciton and the
//      Stroke class to help.
// Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::NPR_Paint()
{
	unsigned char *rgb = this->To_RGB();
	//unsigned char *img_data = new unsigned char[img_width*img_height * 4];
	for (int j = 0; j < img_width*img_height * 4; j++) {
		//img_data[j] = 255;
	}
	int sizes[10] = { 9,7,5,4,3,2,1 };
	for (int i = 0; i < 7; i++) {
		int radius = sizes[i];
		NPR_Paint_Layer(img_data, Filter_Gaussian_N_array(2 * radius + 1, rgb), radius);

	}
	mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);
	renew();
}

void Application::NPR_Paint_Layer(unsigned char *tCanvas, unsigned char *tReferenceImage, int tBrushSize)
{
	vector<Stroke> strokeset;
	unsigned char *difference = new unsigned char[img_width*img_height];
	for (int i = 0; i < img_height; i++) {
		for (int j = 0; j < img_width; j++) {
			int offset_rgba = i*img_width * 4 + j * 4;
			int sum = 0;
			for (int k = 0; k < 3; k++)
				sum = sum + pow(tCanvas[offset_rgba + k] - tReferenceImage[offset_rgba + k], 2);

			difference[i*img_width + j] = sqrt(sum);
		}
	}
	for (int i = 0; i < img_height; i += tBrushSize) {
		for (int j = 0; j < img_width; j += tBrushSize) {
			//implement random window center
			int centerx = j + rand() % (tBrushSize - 1 == 0 ? 1 : tBrushSize - 1);
			if (tBrushSize > 3)
				if (centerx<j + 1 || centerx>j + tBrushSize - 1)
					centerx = j + rand() % (tBrushSize - 1 == 0 ? 1 : tBrushSize - 1);
			int centery = i + rand() % (tBrushSize - 1 == 0 ? 1 : tBrushSize - 1);
			if (tBrushSize > 3)
				if (centery<i + 1 || centery>i + tBrushSize - 1)
					centery = i + rand() % (tBrushSize - 1 == 0 ? 1 : tBrushSize - 1);
			int errorsum = 0;
			for (int m = -1 * tBrushSize / 2 + 1; m < tBrushSize / 2; m++) {
				for (int n = -1 * tBrushSize / 2 + 1; n < tBrushSize / 2; n++) {
					if (centery + m > img_height - 1 || centery + m < 0)
						break;
					if (centerx + n > img_width - 1 || centerx + n < 0)
						break;
					errorsum += difference[(centery + m)*img_width + (centerx + n)];
				}
			}
			errorsum = errorsum / pow(tBrushSize, 2);

			if (errorsum > 25) {
				int largestdiff = 0;
				int largestx = 0; int largesty = 0;
				for (int m = -1 * tBrushSize / 2 + 1; m < tBrushSize / 2; m++) {
					for (int n = -1 * tBrushSize / 2 + 1; n < tBrushSize / 2; n++) {
						if (centery + m > img_height - 1 || centery + m < 0)
							break;
						if (centerx + n > img_width - 1 || centerx + n < 0)
							break;
						if (largestdiff < difference[(centery + m)*img_width + (centerx + n)]) {
							largestdiff = difference[(centery + m)*img_width + (centerx + n)];
							largesty = centery + m;
							largestx = centerx + n;
						}
					}
				}
				int referenceoffset = largesty*img_width * 4 + largestx * 4;
				Stroke s(tBrushSize, largestx, largesty, tReferenceImage[referenceoffset + rr], tReferenceImage[referenceoffset + gg],
					tReferenceImage[referenceoffset + bb], tReferenceImage[referenceoffset + aa]);
				strokeset.push_back(s);
			}
		}
		map<int, Stroke> psuedorandom;
		for (int i = 0; i < strokeset.size(); i++) {
			psuedorandom[i] = strokeset[i];
		}
		for (int k = 0; k < 3; k++) {
			for (int i = 0; i < strokeset.size(); i++) {
				int change = rand() % strokeset.size();
				Stroke temp = psuedorandom[i];
				psuedorandom[i] = psuedorandom[change];
				psuedorandom[change] = temp;
			}
		}

		for (int i = 0; i < strokeset.size(); i++) {
			Paint_Stroke(psuedorandom[i]);
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
//
//      Helper function for the painterly filter; paint a stroke at
// the given location
//
///////////////////////////////////////////////////////////////////////////////
void Application::Paint_Stroke(const Stroke& s)
{
	int radius_squared = (int)s.radius * (int)s.radius;
	for (int x_off = -((int)s.radius); x_off <= (int)s.radius; x_off++)
	{
		for (int y_off = -((int)s.radius); y_off <= (int)s.radius; y_off++)
		{
			int x_loc = (int)s.x + x_off;
			int y_loc = (int)s.y + y_off;

			// are we inside the circle, and inside the image?
			if ((x_loc >= 0 && x_loc < img_width && y_loc >= 0 && y_loc < img_height))
			{
				int dist_squared = x_off * x_off + y_off * y_off;
				int offset_rgba = (y_loc * img_width + x_loc) * 4;

				if (dist_squared <= radius_squared)
				{
					img_data[offset_rgba + rr] = s.r;
					img_data[offset_rgba + gg] = s.g;
					img_data[offset_rgba + bb] = s.b;
					img_data[offset_rgba + aa] = s.a;
				}
				else if (dist_squared == radius_squared + 1)
				{
					img_data[offset_rgba + rr] = (img_data[offset_rgba + rr] + s.r) / 2;
					img_data[offset_rgba + gg] = (img_data[offset_rgba + gg] + s.g) / 2;
					img_data[offset_rgba + bb] = (img_data[offset_rgba + bb] + s.b) / 2;
					img_data[offset_rgba + aa] = (img_data[offset_rgba + aa] + s.a) / 2;
				}
			}
		}
	}
}



void Application::FindPattern() {

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	int findsize = 50;
	unsigned char *rgb = this->To_RGB();
	unsigned char *img_data = new unsigned char[img_width*img_height];
	int histogram[256];
	vector<unsigned char> pixelthreshold;

	for (int i = 0; i < 256; i++)
		histogram[i] = 0;

	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int offset_rgb = i*img_width * 3 + j * 3;
			int offset_rgba = i*img_width + j;
			unsigned char gray = 0.299 * rgb[offset_rgb + rr] + 0.587 * rgb[offset_rgb + gg] + 0.114 * rgb[offset_rgb + bb];

			img_data[offset_rgba] = gray;
			histogram[gray]++;
		}
	}

	vector<vector<unsigned char>> frequency;
	//vector<int> most;
	for (int i = 0; i < 256; i++)
		printf("%d %d\n", i, histogram[i]);

	for (int i = 0; i <= 246; i++) {
		bool swapped = false;
		bool similar = false;
		int sum = 0;
		int most = i;
		vector<unsigned char> set;
		set.push_back(i);
		for (int k = 0; k < 10; k++) {
			sum += histogram[i + k];
			if (histogram[i + k] > histogram[most])
				most = i + k;
		}
		set.push_back(most);
		if (frequency.size() < 3) {
			frequency.push_back(set);
			continue;
		}

		for (int j = 0; j < frequency.size(); j++) {
			int comparesum = 0;
			if (most - frequency[j][1] <= 15 && most - frequency[j][1] >= -15) {
				swapped = true;
				for (int k = 0; k < 10; k++)
					comparesum += histogram[frequency[j][0] + k];
				if (sum > comparesum) {
					frequency.erase(frequency.begin() + j);
					frequency.push_back(set);
				}
				break;
			}
		}
		if (!swapped) {
			int least = 9999999999;
			int place = -1;
			for (int j = 0; j < frequency.size(); j++) {
				int comparesum = 0;
				for (int k = 0; k < 10; k++)
					comparesum += histogram[frequency[j][0] + k];
				if (comparesum < least) {
					least = comparesum;
					place = j;
				}
			}
			if (sum > least) {
				frequency.erase(frequency.begin() + place);
				frequency.push_back(set);
			}

		}
	}
	int save = -1;
	while (true) {
		bool same = false;
		for (int i = 0; i < frequency.size(); i++) {
			for (int j = i + 1; j < frequency.size(); j++) {
				if (frequency[i][1] == frequency[j][1]) {
					frequency.erase(frequency.begin() + j);
					same = true;
					break;
				}
			}
		}
		if (!same)
			break;
	}


	for (int i = 0; i < frequency.size(); i++)
		printf("frequency: %d \n", frequency[i][1]);
	for (int i = 0; i < frequency.size(); i++)
		pixelthreshold.push_back(frequency[i][1]);

	time_t start = time(0);
	time_t now;
	printf("time:0");
	cudaDeviceProp deviceProp;
	deviceProp.major = 0;
	deviceProp.minor = 0;
	int dev = findCudaDevice(0, NULL);

	checkCudaErrors(cudaGetDeviceProperties(&deviceProp, dev));

	printf("CUDA device [%s] has %d Multi-Processors, Compute %d.%d\n",
		deviceProp.name, deviceProp.multiProcessorCount, deviceProp.major, deviceProp.minor);
	vector<Match> output;
	vector<PossibleMatches> input;


	int *outputwidth, *outputheight, *outputtimes;
	size_t heap_size;
	cudaDeviceGetLimit(&heap_size, cudaLimitMallocHeapSize);
	printf("space: %d\n", heap_size);
	cudaDeviceSetLimit(cudaLimitMallocHeapSize, 1000000000);
	cudaDeviceGetLimit(&heap_size, cudaLimitMallocHeapSize);
	printf("space: %d\n", heap_size);

	cudaSetDevice(0);
	//checkpatterns(img_width, img_height, img_data, pixelthreshold, input);
	printf("done\n ");
	//externalfunction(img_width, img_height, rgb, input, output);
	ImageWindow *iw = new ImageWindow();
	iw->setImageMap(img_data, img_width, img_height);
	iw->show();
	QWidget *w = new QWidget();
	w->resize(1000, 1000);
	QVBoxLayout layout{ w };
	QTableWidget *table = new QTableWidget();
	table->setShowGrid(true);

	for (int i = 0; i < output.size(); i++) {
		//printf("\nnumber of patterns: %d\n", output[i].height);
	}
	table->setRowCount(output.size());
	table->setColumnCount(3);
	printf("\nnumber of patterns: %d\n\n", output.size());
	for (int i = 0; i < output.size(); i++) {
		QLabel *pattern = new QLabel();
		unsigned char *patterntemp = new unsigned char[output[i].height * output[i].width * 4];
		for (int ii = 0; ii < output[i].height; ii++) {
			for (int jj = 0; jj < output[i].width; jj++) {
				int offset_rgb = output[i].startpos + ii*img_width * 3 + jj * 3;
				int offset_rgba = ii*output[i].width * 4 + jj * 4;
				patterntemp[offset_rgba + rr] = rgb[offset_rgb + rr];
				patterntemp[offset_rgba + gg] = rgb[offset_rgb + gg];
				patterntemp[offset_rgba + bb] = rgb[offset_rgb + bb];
				patterntemp[offset_rgba + aa] = WHITE;
			}
		}
		QImage patternimage = QImage(patterntemp, output[i].width,
			output[i].height, QImage::Format_ARGB32);
		pattern->setPixmap(QPixmap::fromImage(patternimage));
		QTableWidgetItem *thumbnail = new QTableWidgetItem;
		thumbnail->setData(Qt::DecorationRole, QPixmap::fromImage(patternimage));
		table->setItem(i, 0, thumbnail);
		table->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(output[i].times)));
		table->setItem(i, 2, new QTableWidgetItem(tr("%1x%2").arg(output[i].width).arg(output[i].height)));
		delete[] patterntemp;
	}
	table->resizeRowsToContents();
	table->resizeColumnsToContents();
	//table->setItem(0, 1, new QTableView("Hello"));
	layout.addWidget(table);
	//w->show();
	delete[] rgb;
	//mImageDst = QImage(img_data, img_width, img_height, QImage::Format_ARGB32);


	//renew();
}







///////////////////////////////////////////////////////////////////////////////
//
//      Build a Stroke
//
///////////////////////////////////////////////////////////////////////////////
Stroke::Stroke() {}

///////////////////////////////////////////////////////////////////////////////
//
//      Build a Stroke
//
///////////////////////////////////////////////////////////////////////////////
Stroke::Stroke(unsigned int iradius, unsigned int ix, unsigned int iy,
	unsigned char ir, unsigned char ig, unsigned char ib, unsigned char ia) :
	radius(iradius), x(ix), y(iy), r(ir), g(ig), b(ib), a(ia)
{
}



