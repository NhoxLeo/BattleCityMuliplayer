#include "Scene.h"
#include "Bullet.h"
#include "MyWindow.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	MyWindow wnd;
	if (!wnd.Create())
	{
		return -1;
	}
	
	return wnd.MessageProc();
}