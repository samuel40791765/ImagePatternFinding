#ifndef QT_OPENGL_FRAMEWORK_H
#define QT_OPENGL_FRAMEWORK_H

#include <QtWidgets/QMainWindow>
#include "ui_qt_opengl_framework.h"
#include "ui_prompt.h"
#include "ui_About.h"
#include "Application.h"
#include "MB.h"
#include "OpenglRenderThread.h"

class Qt_Opengl_Framework : public QMainWindow
{
	Q_OBJECT

public:
	Qt_Opengl_Framework(QWidget *parent = 0);
	~Qt_Opengl_Framework();
	static Qt_Opengl_Framework* getInstance();
	std::string plugin;
	QDialog* rendere_dialog;
	QDialog* About_dialog;
	Ui_Dialog rendere_prompt;
	Ui_About mAbout;
	Ui_MyMessageBox *MBOX;
	QLabel* status_text;
	QLabel* fps_text;
	Ui::Qt_Opengl_FrameworkClass ui;
	QString Nowfilename;
	

private:
	void Initialize();
	void loadFile(const QString &fileName);
	void loadSecondFile(const QString &fileName);
	Application *application;
	static Qt_Opengl_Framework *single;
	OpenglRenderThread rt;
	

private slots:
	void a_GL();
	void a_DirectX();
	void mMyBox();
	void mMyBox1();
	void mMyBox2();
	//File
	void OnOpen();
	void OnSave();
	void OnQuit();
	void OnAbout();
	//Color
	void OnGray();
	void OnQuant_Uniform();
	void OnQuant_Populosity();
	//Dithering
	void OnDither_Threshold();
	void OnDither_Random();
	void OnDither_FS();
	void OnDither_Bright();
	void OnDither_Cluster();
	void OnDither_Color();
	// Filter
	void OnFilter_Box();
	void OnFilter_Bartlett();
	void OnFilter_Gaussian();
	void OnFilter_Gaussian_N();
	void OnFilter_Edge();
	void OnFilter_Enhance();
	// Size
	void OnHalf_Size();
	void OnDouble_Size();
	void OnResize();
	void OnRotate();
	// Composing
	void OnComp_Over();
	void OnComp_In();
	void OnComp_Out();
	void OnComp_Atop();
	void OnComp_Xor();
	// Bouns
	void OnNPR();

	//Pattern
	void mPatternSize();
	void onFindPattern();
protected:
	virtual void keyPressEvent(QKeyEvent *e);
	
};

#endif // Qt_Opengl_Framework_H
