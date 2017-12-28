#include "qt_opengl_framework.h"
Qt_Opengl_Framework* Qt_Opengl_Framework::single = NULL;
Qt_Opengl_Framework* Qt_Opengl_Framework::getInstance()
{
	if(! single)
	{
		single = new Qt_Opengl_Framework();
		single->Initialize();
		//instanceFlag = true;
		return single;
	}
	else
	{
		return single;
	}
}
//****************************************************************************
//
// * Constructor 
// 連動主功能視窗的Qt與Opengl動作
//============================================================================

Qt_Opengl_Framework::Qt_Opengl_Framework(QWidget *parent)
	: QMainWindow(parent)
{
	application = NULL;
	ui.setupUi(this);
	//File
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(OnOpen()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(OnSave()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(OnQuit()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(OnAbout()));
	//Color
	connect(ui.actionGray, SIGNAL(triggered()), this, SLOT(OnGray()));
	connect(ui.actionUniform_Quantization, SIGNAL(triggered()), this, SLOT(OnQuant_Uniform()));
	connect(ui.actionPopulosity, SIGNAL(triggered()), this, SLOT(OnQuant_Populosity()));
	//Dithering
	connect(ui.actionNaive_Threshold_Dithering, SIGNAL(triggered()), this, SLOT(OnDither_Threshold()));
	connect(ui.actionBrightness_Preserving_Threshold_Dithering, SIGNAL(triggered()), this, SLOT(OnDither_Bright()));
	connect(ui.actionRandom, SIGNAL(triggered()), this, SLOT(OnDither_Random()));
	connect(ui.actionClustered, SIGNAL(triggered()), this, SLOT(OnDither_Cluster()));
	connect(ui.actionFloyd_Steinberg, SIGNAL(triggered()), this, SLOT(OnDither_FS()));
	connect(ui.actionColor_Floyd_Steinberg, SIGNAL(triggered()), this, SLOT(OnDither_Color()));
	// Filter
	connect(ui.actionBox, SIGNAL(triggered()), this, SLOT(OnFilter_Box()));
	connect(ui.actionBartlett, SIGNAL(triggered()), this, SLOT(OnFilter_Bartlett()));
	connect(ui.actionGaussian, SIGNAL(triggered()), this, SLOT(OnFilter_Gaussian()));
	connect(ui.actionArbitrary_Size_Gaussian, SIGNAL(triggered()), this, SLOT(mMyBox()));
	connect(ui.actionEdge, SIGNAL(triggered()), this, SLOT(OnFilter_Edge()));
	connect(ui.actionEnhance, SIGNAL(triggered()), this, SLOT(OnFilter_Enhance()));
	// Size
	connect(ui.actionHalf, SIGNAL(triggered()), this, SLOT(OnHalf_Size()));
	connect(ui.actionDouble, SIGNAL(triggered()), this, SLOT(OnDouble_Size()));
	connect(ui.actionArbitrary_Uniform_Scale, SIGNAL(triggered()), this, SLOT(mMyBox1()));
	connect(ui.actionRotate, SIGNAL(triggered()), this, SLOT(mMyBox2()));
	// Composing
	connect(ui.actionOver, SIGNAL(triggered()), this, SLOT(OnComp_Over()));
	connect(ui.actionIn, SIGNAL(triggered()), this, SLOT(OnComp_In()));
	connect(ui.actionOut, SIGNAL(triggered()), this, SLOT(OnComp_Out()));
	connect(ui.actionAtop, SIGNAL(triggered()), this, SLOT(OnComp_Atop()));
	connect(ui.actionXor, SIGNAL(triggered()), this, SLOT(OnComp_Xor()));
	// Bouns
	connect(ui.actionNPR, SIGNAL(triggered()), this, SLOT(OnNPR()));

	connect(ui.actionFind_Pattern, SIGNAL(triggered()), this, SLOT(onFindPattern()));
}

Qt_Opengl_Framework::~Qt_Opengl_Framework()
{

}

//****************************************************************************
//
// * 初始化表單 
// 連動選擇表單的Qt與Opengl動作，且產生About視窗
//============================================================================
void Qt_Opengl_Framework::Initialize()
{
	rendere_dialog = new QDialog();
	About_dialog= new QDialog();
	rendere_dialog->setModal(true);
	About_dialog->setModal(true);
	rendere_prompt.setupUi(rendere_dialog);
	mAbout.setupUi(About_dialog);
	rendere_dialog->show();
	connect(rendere_prompt.GLButton, SIGNAL(clicked()), this, SLOT(a_GL()));
	connect(rendere_prompt.DirectXButton, SIGNAL(clicked()), this, SLOT(a_DirectX()));
}

//****************************************************************************
//
// * 設定Rendering System為OpenGL時
// 
//============================================================================
void Qt_Opengl_Framework::a_GL()
{
	plugin =("OpenGL Rendering Subsystem");
	application = new Application;
	application->setMouseTracking(true);
	rendere_dialog->hide();
}

//****************************************************************************
//
// * 設定Rendering System為DirectX時
// 
//============================================================================
void Qt_Opengl_Framework::a_DirectX()
{
	plugin =("Direct3D9 Rendering Subsystem");
	application = new Application;
	application->setMouseTracking(true);
	rendere_dialog->hide();
}

//****************************************************************************
//
// * 當使用者使用Open圖檔功能時，所呼叫的載入檔案函數
// 
//============================================================================
void Qt_Opengl_Framework::loadFile(const QString &fileName)
{
	QFile file(fileName);

	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}

	QTextStream in(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	application->openImage(fileName);
}

//****************************************************************************
//
// * 當使用者使用Composing功能時，需要載入第二個圖檔時所呼叫載入圖檔函數
// 
//============================================================================
void Qt_Opengl_Framework::loadSecondFile( const QString &fileName )
{
	QFile file(fileName);

	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}

	QTextStream in(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	application->loadSecondaryImge(fileName);
}

//------------------------File------------------------

//****************************************************************************
//
// * 從硬碟中開啟指定圖檔
// 
//============================================================================
void Qt_Opengl_Framework::OnOpen()
{

	QString fileName = QFileDialog::getOpenFileName(this,"OpenImage","../../media/materials/textures", tr("PNG (*.png);;JPEG (*.jpg)" ));
	if (!fileName.isEmpty()){
		loadFile(fileName);
		Nowfilename = fileName;
	}
}

//****************************************************************************
//
// * 存入圖檔至硬碟指定位置中
// 
//============================================================================
void Qt_Opengl_Framework::OnSave()
{
	QString fileName = QFileDialog::getSaveFileName(this,"SaveImage",".",tr("PNG (*.png);;JPEG (*.jpg)" ));
	if (!fileName.isEmpty())
		application->saveImage(fileName);
}

//****************************************************************************
//
// * 結束程式
// 
//============================================================================
void Qt_Opengl_Framework::OnQuit()
{
	exit(0);
}

//****************************************************************************
//
// * 顯示關於視窗
// 
//============================================================================
void Qt_Opengl_Framework::OnAbout()
{
	About_dialog->show();
}
//------------------------Color------------------------

//****************************************************************************
//
// * 連動GUI按鈕至Color to Grayscale功能
// 
//============================================================================
void Qt_Opengl_Framework::OnGray()
{
	application->Gray();
}

//****************************************************************************
//
// * 連動GUI按鈕至Uniform Quantization功能
// 
//============================================================================
void Qt_Opengl_Framework::OnQuant_Uniform()
{
	application->Quant_Uniform();
}

//****************************************************************************
//
// * 連動GUI按鈕至Populosity功能
// 
//============================================================================
void Qt_Opengl_Framework::OnQuant_Populosity()
{
	application->Quant_Populosity();
}



//------------------------Dithering------------------------

//****************************************************************************
//
// * 連動GUI按鈕至Naive Threshold Dithering功能
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_Threshold()
{
	application->Dither_Threshold();
}
//****************************************************************************
//
// * 連動GUI按鈕至Random Dithering功能
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_Random()
{
	application->Dither_Random();
}
//****************************************************************************
//
// * 連動GUI按鈕至Floyd-Steinberg Dithering功能
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_FS()
{
	application->Dither_FS();
}
//****************************************************************************
//
// * 連動GUI按鈕至Brightness Preserving Threshold Dithering功能
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_Bright()
{
	application->Dither_Bright();
}
//****************************************************************************
//
// * 連動GUI按鈕至Clustered Dithering功能
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_Cluster()
{
	application->Dither_Cluster();
}
//****************************************************************************
//
// * 連動GUI按鈕至Color Floyd-Steinberg Dithering功能
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_Color()
{
	application->Dither_Color();
}

//------------------------Filter------------------------

//****************************************************************************
//
// * 連動GUI按鈕至Box Filter功能
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Box()
{
	application->Filter_Box();
}
//****************************************************************************
//
// * 連動GUI按鈕至Bartlett Filter功能
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Bartlett()
{
	application->Filter_Bartlett();
}
//****************************************************************************
//
// * 連動GUI按鈕至Gaussian Filter功能
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Gaussian()
{
	application->Filter_Gaussian();
}
//****************************************************************************
//
// * 連動GUI按鈕至Arbitrary-Size Gaussian Filter功能
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Gaussian_N()
{
	MBOX->hide();
	application->Filter_Gaussian_N(MBOX->getMyInt());
}
//****************************************************************************
//
// * 連動GUI按鈕至Edge功能
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Edge()
{
	
	application->Filter_Edge();
}
//****************************************************************************
//
// * 連動GUI按鈕至Enhance功能
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Enhance()
{
	
	application->Filter_Enhance();
}

//------------------------Size------------------------

//****************************************************************************
//
// * 連動GUI按鈕至Half Size功能
// 
//============================================================================
void Qt_Opengl_Framework::OnHalf_Size()
{
	application->Half_Size();
}
//****************************************************************************
//
// * 連動GUI按鈕至Double Size功能
// 
//============================================================================
void Qt_Opengl_Framework::OnDouble_Size()
{
	application->Double_Size();
}
//****************************************************************************
//
// * 連動GUI按鈕至Arbitrary Uniform Scale scale功能
// 
//============================================================================
void Qt_Opengl_Framework::OnResize()
{
	MBOX->hide();
	application->Resize(MBOX->getMyFloat());
}
//****************************************************************************
//
// * 連動GUI按鈕至Arbitrary Rotation功能
// 
//============================================================================
void Qt_Opengl_Framework::OnRotate()
{
	MBOX->hide();
	application->Rotate(MBOX->getMyFloat());
}

//------------------------Composing------------------------

//****************************************************************************
//
// * 連動GUI按鈕至Over功能
// 
//============================================================================
void Qt_Opengl_Framework::OnComp_Over()
{
	QString fileName = QFileDialog::getOpenFileName(this,"OpenImage","/Qt_Opengl_Framework/media/materials/textures", tr("PNG (*.png);;JPEG (*.jpg)" ));
	if (!fileName.isEmpty()){
		loadSecondFile(fileName);
		application->Comp_Over();
	}
	
}
//****************************************************************************
//
// * 連動GUI按鈕至In功能
// 
//============================================================================
void Qt_Opengl_Framework::OnComp_In()
{

	QString fileName = QFileDialog::getOpenFileName(this,"OpenImage","/Qt_Opengl_Framework/media/materials/textures", tr("PNG (*.png);;JPEG (*.jpg)" ));
	if (!fileName.isEmpty()){
		loadSecondFile(fileName);
		application->Comp_In();
	}

	
}
//****************************************************************************
//
// * 連動GUI按鈕至Out功能
// 
//============================================================================
void Qt_Opengl_Framework::OnComp_Out()
{
	QString fileName = QFileDialog::getOpenFileName(this,"OpenImage","/Qt_Opengl_Framework/media/materials/textures", tr("PNG (*.png);;JPEG (*.jpg)" ));
	if (!fileName.isEmpty()){
		loadSecondFile(fileName);
		application->Comp_Out();
	}

	
}
//****************************************************************************
//
// * 連動GUI按鈕至Atop功能
// 
//============================================================================
void Qt_Opengl_Framework::OnComp_Atop()
{
	QString fileName = QFileDialog::getOpenFileName(this,"OpenImage","/Qt_Opengl_Framework/media/materials/textures", tr("PNG (*.png);;JPEG (*.jpg)" ));
	if (!fileName.isEmpty()){
		loadSecondFile(fileName);
		application->Comp_Atop();
	}

	
}
//****************************************************************************
//
// * 連動GUI按鈕至Xor功能
// 
//============================================================================
void Qt_Opengl_Framework::OnComp_Xor()
{
	QString fileName = QFileDialog::getOpenFileName(this,"OpenImage","/Qt_Opengl_Framework/media/materials/textures", tr("PNG (*.png);;JPEG (*.jpg)" ));
	if (!fileName.isEmpty()){
		loadSecondFile(fileName);
		application->Comp_Xor();
	}

	
}


//------------------------NPR------------------------

//****************************************************************************
//
// * 連動GUI按鈕至NPR Paint功能
// 
//============================================================================
void Qt_Opengl_Framework::OnNPR()
{
	application->NPR_Paint();
}

void Qt_Opengl_Framework::onFindPattern()
{
	application->FindPattern();
}

//------------------------MessageBox------------------------

//****************************************************************************
//
// * 顯示輸入Gaussian_N 的MessageBox
// 
//============================================================================
void Qt_Opengl_Framework::mPatternSize()
{
	MBOX = new Ui_MyMessageBox();
	MBOX->show();
	MBOX->setWindowTitle("Pattern Pixel Size");
	MBOX->label->setText("Value N");
	connect(MBOX->pushButton, SIGNAL(clicked()), this, SLOT(onFindPattern()));
}

void Qt_Opengl_Framework::mMyBox()
{
	MBOX=new Ui_MyMessageBox();
	MBOX->show();
	MBOX->setWindowTitle("Gaussian_N");
	MBOX->label->setText("Value N");
	connect(MBOX->pushButton, SIGNAL(clicked()), this, SLOT(OnFilter_Gaussian_N()));
}

//****************************************************************************
//
// * 顯示輸入Resize 的MessageBox
// 
//============================================================================
void Qt_Opengl_Framework::mMyBox1()
{
	MBOX=new Ui_MyMessageBox();
	MBOX->show();
	MBOX->setWindowTitle("Resize");
	MBOX->label->setText("Scale");
	connect(MBOX->pushButton, SIGNAL(clicked()), this, SLOT(OnResize()));
}

//****************************************************************************
//
// * 顯示輸入Rotate 的MessageBox
// 
//============================================================================
void Qt_Opengl_Framework::mMyBox2()
{
	MBOX=new Ui_MyMessageBox();
	MBOX->show();
	MBOX->setWindowTitle("Rotate");
	MBOX->label->setText("Angles");
	connect(MBOX->pushButton, SIGNAL(clicked()), this, SLOT(OnRotate()));
}

//****************************************************************************
//
// * 按下Enter鈕時，畫面初始化
// 
//============================================================================
void Qt_Opengl_Framework::keyPressEvent(QKeyEvent *e)
{
	if (e->key()==Qt::Key_Enter || e->key()==Qt::Key_Return)
	{
		loadFile(Nowfilename);
	}
}









