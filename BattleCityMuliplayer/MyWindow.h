
#ifndef WINDOWS_H
#define WINDOWS_H

#include <windows.h>
#include <vector>
#include <tchar.h>

using namespace std;


class GameManager;
class CFont;

struct Message
{
	Message(UINT m, void(*f)(HWND, WPARAM, LPARAM));
	UINT     msg;
	void(*fp)(HWND, WPARAM, LPARAM);
};


class MyMessage
{
public:

	void addMessage();

	static void OnDestroy(HWND hwnd, WPARAM wparam, LPARAM lparam);
protected:
	static vector<Message> m_Message;
};

class MyWindow :public MyMessage
{
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	WNDCLASSEX m_wndClass;
	int Height;
	int width;
	CFont* frame;
	char szBuf[10];
private:

	bool RegisterWindow();
public:
	MyWindow();
	~MyWindow(){};

	bool Create();

	HWND getWnd();

	static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int MessageProc();

	void setWindowSize(int h, int w);
	int getWindowHeight();
	int getWindowwidth();

	void Update(DWORD dwElapsedTime);
};

#endif