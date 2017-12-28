#include "OpenglWidget.h"
#include "qt_opengl_framework.h"

const QPoint     OpenglWidget::invalidMousePoint(-1,-1);

OpenglWidget::OpenglWidget(QWidget *parent)
:QWidget(parent),
 oldPos(invalidMousePoint)
{
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_PaintOnScreen);
	setMinimumSize(240,240);
	setFocusPolicy(Qt::ClickFocus);
	initOpenglSystem();
}

OpenglWidget::~OpenglWidget()
{

}


void OpenglWidget::keyPressEvent(QKeyEvent *e)
{

}
void OpenglWidget::keyReleaseEvent( QKeyEvent *e )
{

}

void OpenglWidget::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void OpenglWidget::mouseMoveEvent(QMouseEvent *e)
{

}

void OpenglWidget::mousePressEvent(QMouseEvent *e)
{

}

void OpenglWidget::mouseReleaseEvent(QMouseEvent *e)
{

}

void OpenglWidget::moveEvent(QMoveEvent *e)
{
	QWidget::moveEvent(e);

	if(e->isAccepted() /*&& mWindow*/)
	{
		update();
	}
}

QPaintEngine* OpenglWidget::paintEngine() const
{
	// We don't want another paint engine to get in the way for our Ogre based paint engine.
	// So we return nothing.
	return NULL;
}

void OpenglWidget::paintEvent(QPaintEvent *e)
{	
	QPainter painter(this);
	painter.drawImage(QPoint(0, 0), image);
}

void OpenglWidget::resizeEvent(QResizeEvent *e)
{
	QWidget::resizeEvent(e);

	if(e->isAccepted())
	{
		const QSize &newSize = e->size();
	}
}

void OpenglWidget::showEvent(QShowEvent *e)
{
	QWidget::showEvent(e);
}

void OpenglWidget::wheelEvent(QWheelEvent *e)
{

}

void OpenglWidget::initOpenglSystem()
{
	bool ok = false;
	Qt_Opengl_Framework* w = Qt_Opengl_Framework::getInstance();
	std::string  desiredRenderer = w->plugin;
	
}
void OpenglWidget::startApp()
{

}

void OpenglWidget::setupNLoadResources()
{
	
}