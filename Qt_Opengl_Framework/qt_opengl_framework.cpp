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
// �s�ʥD�\�������Qt�POpengl�ʧ@
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
// * ��l�ƪ�� 
// �s�ʿ�ܪ�檺Qt�POpengl�ʧ@�A�B����About����
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
// * �]�wRendering System��OpenGL��
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
// * �]�wRendering System��DirectX��
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
// * ��ϥΪ̨ϥ�Open���ɥ\��ɡA�ҩI�s�����J�ɮר��
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
// * ��ϥΪ̨ϥ�Composing�\��ɡA�ݭn���J�ĤG�ӹ��ɮɩҩI�s���J���ɨ��
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
// * �q�w�Ф��}�ҫ��w����
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
// * �s�J���ɦܵw�Ы��w��m��
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
// * �����{��
// 
//============================================================================
void Qt_Opengl_Framework::OnQuit()
{
	exit(0);
}

//****************************************************************************
//
// * ����������
// 
//============================================================================
void Qt_Opengl_Framework::OnAbout()
{
	About_dialog->show();
}
//------------------------Color------------------------

//****************************************************************************
//
// * �s��GUI���s��Color to Grayscale�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnGray()
{
	application->Gray();
}

//****************************************************************************
//
// * �s��GUI���s��Uniform Quantization�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnQuant_Uniform()
{
	application->Quant_Uniform();
}

//****************************************************************************
//
// * �s��GUI���s��Populosity�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnQuant_Populosity()
{
	application->Quant_Populosity();
}



//------------------------Dithering------------------------

//****************************************************************************
//
// * �s��GUI���s��Naive Threshold Dithering�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_Threshold()
{
	application->Dither_Threshold();
}
//****************************************************************************
//
// * �s��GUI���s��Random Dithering�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_Random()
{
	application->Dither_Random();
}
//****************************************************************************
//
// * �s��GUI���s��Floyd-Steinberg Dithering�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_FS()
{
	application->Dither_FS();
}
//****************************************************************************
//
// * �s��GUI���s��Brightness Preserving Threshold Dithering�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_Bright()
{
	application->Dither_Bright();
}
//****************************************************************************
//
// * �s��GUI���s��Clustered Dithering�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_Cluster()
{
	application->Dither_Cluster();
}
//****************************************************************************
//
// * �s��GUI���s��Color Floyd-Steinberg Dithering�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnDither_Color()
{
	application->Dither_Color();
}

//------------------------Filter------------------------

//****************************************************************************
//
// * �s��GUI���s��Box Filter�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Box()
{
	application->Filter_Box();
}
//****************************************************************************
//
// * �s��GUI���s��Bartlett Filter�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Bartlett()
{
	application->Filter_Bartlett();
}
//****************************************************************************
//
// * �s��GUI���s��Gaussian Filter�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Gaussian()
{
	application->Filter_Gaussian();
}
//****************************************************************************
//
// * �s��GUI���s��Arbitrary-Size Gaussian Filter�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Gaussian_N()
{
	MBOX->hide();
	application->Filter_Gaussian_N(MBOX->getMyInt());
}
//****************************************************************************
//
// * �s��GUI���s��Edge�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Edge()
{
	
	application->Filter_Edge();
}
//****************************************************************************
//
// * �s��GUI���s��Enhance�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnFilter_Enhance()
{
	
	application->Filter_Enhance();
}

//------------------------Size------------------------

//****************************************************************************
//
// * �s��GUI���s��Half Size�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnHalf_Size()
{
	application->Half_Size();
}
//****************************************************************************
//
// * �s��GUI���s��Double Size�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnDouble_Size()
{
	application->Double_Size();
}
//****************************************************************************
//
// * �s��GUI���s��Arbitrary Uniform Scale scale�\��
// 
//============================================================================
void Qt_Opengl_Framework::OnResize()
{
	MBOX->hide();
	application->Resize(MBOX->getMyFloat());
}
//****************************************************************************
//
// * �s��GUI���s��Arbitrary Rotation�\��
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
// * �s��GUI���s��Over�\��
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
// * �s��GUI���s��In�\��
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
// * �s��GUI���s��Out�\��
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
// * �s��GUI���s��Atop�\��
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
// * �s��GUI���s��Xor�\��
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
// * �s��GUI���s��NPR Paint�\��
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
// * ��ܿ�JGaussian_N ��MessageBox
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
// * ��ܿ�JResize ��MessageBox
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
// * ��ܿ�JRotate ��MessageBox
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
// * ���UEnter�s�ɡA�e����l��
// 
//============================================================================
void Qt_Opengl_Framework::keyPressEvent(QKeyEvent *e)
{
	if (e->key()==Qt::Key_Enter || e->key()==Qt::Key_Return)
	{
		loadFile(Nowfilename);
	}
}









