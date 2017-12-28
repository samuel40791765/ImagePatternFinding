#include <QThread>
#include <QTimer>
#include <vector>
#include <time.h>
class OpenglRenderThread : public QThread
{
	Q_OBJECT
public: 
protected:	
	virtual void run();
private:

signals:
	void nextFrame();
private slots:
	void update();
};