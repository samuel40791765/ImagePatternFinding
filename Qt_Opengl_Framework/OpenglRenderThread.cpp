#include "OpenglRenderThread.h"
#include <windows.h>
void OpenglRenderThread::run()
{

	while(1)
	{
		emit nextFrame();
		Sleep(1);
	}

}

void OpenglRenderThread::update()
{
	emit nextFrame();
}
