#include "ImageWindow.h"

ImageWindow::ImageWindow(QWidget *parent)
{
	ui.setupUi(this);
	trainview = new ImageView();
	setGeometry(100, 25, 1000, 768);
	ui.mainLayout->layout()->addWidget(trainview);
	trainview->installEventFilter(this);
	this->canpan = false;
	this->isHover = false;
	this->trainview->camera = 0;
	this->trainview->track = 0;
	this->trainview->curve = 0;
	this->trainview->isrun = false;
	this->yplane = false;
}

ImageWindow::~ImageWindow()
{
}

void ImageWindow::setImageMap(unsigned char *image, int width, int height)
{
	imagemap = image;
	trainview->imagemap = imagemap;
	trainview->img_height = height;
	trainview->img_width = width;
}

bool ImageWindow::eventFilter(QObject *watched, QEvent *e) {
	if (e->type() == QEvent::MouseButtonPress) {
		QMouseEvent *event = static_cast<QMouseEvent*> (e);
		// Get the mouse position
		float x, y;
		trainview->arcball.getMouseNDC((float)event->localPos().x(), (float)event->localPos().y(), x, y);

		// Compute the mouse position
		trainview->arcball.down(x, y);
		if (event->button() == Qt::LeftButton) {
			trainview->doPick(event->localPos().x(), event->localPos().y());
			this->isHover = true;
			//if (this->canpan)
				trainview->arcball.mode = trainview->arcball.Pan;
		}
		if (event->button() == Qt::RightButton) {
			trainview->arcball.mode = trainview->arcball.Rotate;
		}
	}

	if (e->type() == QEvent::MouseButtonRelease) {
		this->canpan = false;
		this->isHover = false;
		trainview->arcball.mode = trainview->arcball.None;
	}

	if (e->type() == QEvent::Wheel) {
		QWheelEvent *event = static_cast<QWheelEvent*> (e);
		float zamt = (event->delta() < 0) ? 1.1f : 1 / 1.1f;
		trainview->arcball.eyeZ *= zamt;
	}

	if (e->type() == QEvent::MouseMove) {
		QMouseEvent *event = static_cast<QMouseEvent*> (e);
		if (trainview->arcball.mode != trainview->arcball.None) { // we're taking the drags
			float x, y;
			trainview->arcball.getMouseNDC((float)event->localPos().x(), (float)event->localPos().y(), x, y);
			trainview->arcball.computeNow(x, y);
		};
	}

	if (e->type() == QEvent::KeyPress) {
		QKeyEvent *event = static_cast< QKeyEvent*> (e);
		// Set up the mode
		if (event->key() == Qt::Key_0)
			this->canpan = true;
	}

	return QWidget::eventFilter(watched, e);
}