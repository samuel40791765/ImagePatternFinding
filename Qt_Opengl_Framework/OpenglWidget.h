#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtGui>
#include <QtWidgets/QtWidgets>

#include <stdio.h>

#include <fcntl.h>

#include <io.h>

#include <iostream>

#include <fstream>
#include "ClassDefine.h"

class OpenglWidget : public QWidget
{
	Q_OBJECT

public:
	OpenglWidget(QWidget *parent = 0);
	~OpenglWidget();
	bool mShutDown;
	QImage image;

public slots:
	// Override QWidget::paintEngine to return NULL
	QPaintEngine* paintEngine() const; // Turn off QTs paint engine for the Ogre widget.
protected:
	virtual void keyPressEvent(QKeyEvent *e);
	virtual void keyReleaseEvent(QKeyEvent *e);
	virtual void moveEvent(QMoveEvent *e);
	virtual void mouseDoubleClickEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);

	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void paintEvent(QPaintEvent *e);
	virtual void resizeEvent(QResizeEvent *e);
	virtual void showEvent(QShowEvent *e);
	virtual void wheelEvent(QWheelEvent *e);
	virtual void createScene(void)=0;

protected:
	void initOpenglSystem();
	void setupNLoadResources();
	void startApp();
	static const QPoint invalidMousePoint;

private:
	

protected:
	
	QPoint oldPos;
};

#endif OpenglWidget_H
