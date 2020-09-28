#include "MyWindow.h"
#include "GameManager.h"
#include "Scene.h"
#include "MyRender.h"
#include "StartScene.h"
#include "Controller.h"
#include "ActionManager.h"
#include "CFont.h"
#include "OverScene.h"
#include "GameScene.h"
//#include "Sound.h"

static DWORD dwTime = timeGetTime();

Message::Message(UINT m, void(*f)(HWND, WPARAM, LPARAM))
{
	msg = m;
	fp = f;
}

MyWindow::MyWindow()
{
	m_hwnd = 0;
	m_hInstance = GetModuleHandle(NULL);
	ZeroMemory(&m_wndClass, sizeof(WNDCLASSEX));
	//Height = GetSystemMetrics(SM_CYSCREEN);
	//width = GetSystemMetrics(SM_CXSCREEN);
	Height = 480;
	width = 640;
	frame = new CFont;
}

bool MyWindow::Create()
{
	if (!RegisterWindow())
	{
		return false;
	}
	m_hwnd = CreateWindowEx(NULL,
		WindowClassName,
		WindowClassName,
		WS_POPUP,
		0, 0,
		width, Height,
		NULL,
		NULL,
		m_hInstance,
		NULL);
	GameManager::getInstance()->Create(m_hwnd, m_hInstance);
	frame->Create();
	ShowWindow(m_hwnd,SW_SHOW);
	UpdateWindow(m_hwnd);

	return true;
}

bool MyWindow::RegisterWindow()
{
	m_wndClass.cbSize = sizeof(WNDCLASSEX);
	m_wndClass.cbWndExtra = 0;
	m_wndClass.cbClsExtra = 0;
	m_wndClass.style = 0;
	m_wndClass.lpfnWndProc = WinProc;
	m_wndClass.hInstance = m_hInstance;
	m_wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	m_wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_wndClass.lpszClassName = WindowClassName;
	m_wndClass.lpszMenuName = NULL;
	m_wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	return RegisterClassEx(&m_wndClass);
}

void MyWindow::setWindowSize(int h, int w)
{
	Height = h;
	width = w;
}

int MyWindow::getWindowHeight()
{
	return Height;
}

int MyWindow::getWindowwidth()
{
	return width;
}

LRESULT CALLBACK MyWindow::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int MyWindow::MessageProc()
{
	char FPS[256] = {};

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	//OverScene* scene = OverScene::create();
	//StartScene* scene = StartScene::create();
	GameManager::getInstance()->setLevel(1);
	GameScene* scene = GameScene::create();
	GameManager::getInstance()->setScene(scene);
	Controller::getInstance1()->InitControl(m_hwnd, m_hInstance);
	Controller::getInstance2()->InitControl(m_hwnd, m_hInstance);
	Controller::getInstance3()->InitControl(m_hwnd, m_hInstance);
	Controller::getInstance4()->InitControl(m_hwnd, m_hInstance);
	//BASS_Init(-1, 44100, 0, 0, NULL);

	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DWORD dwCurrentTime = timeGetTime();
			DWORD dwElapsedTime = dwCurrentTime - dwTime;
			DWORD fElapsedTime = dwElapsedTime*0.001f;
			Update(fElapsedTime);
			dwTime = dwCurrentTime;
		}
	}
	return msg.wParam;
}

std::string UnicodeToANSI(const std::wstring& str)
{
	int iTextlen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pEleMentText = new char[iTextlen + 1];
	memset((void*)pEleMentText, 0, sizeof(char)*(iTextlen + 1));
	WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pEleMentText, iTextlen, NULL, NULL);
	std::string strText(pEleMentText);
	delete[] pEleMentText;
	return strText;
};

std::wstring ANSIToUnicode(const std::string& str)
{
	int unicodelen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pUnicode;
	pUnicode = new wchar_t[unicodelen + 1];
	memset(pUnicode, 0, (unicodelen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodelen);
	std::wstring rt(pUnicode);
	delete pUnicode;
	return rt;
};

void MyWindow::Update(DWORD ElapsedTime)
{
	ZeroMemory(&szBuf, sizeof(char) * 10);
	wstring x;
	LPCTSTR Text;
	if (ElapsedTime < 1000 / FRAME)
	{
		sprintf_s(szBuf, "%d", FRAME);
	}
	else
	{
		sprintf_s(szBuf, "%d", 1 / (ElapsedTime / 1000));
	}
	x = ANSIToUnicode(szBuf);
	Text = (LPCWSTR)x.c_str();
	RECT rect = { 0,40, 100, 90 };
	GameManager::getInstance()->checkclick1();
	GameManager::getInstance()->checkclick2();
	GameManager::getInstance()->checkclick3();
	GameManager::getInstance()->checkclick4();
	MyRender::getInstance()->getDevice()->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 251, 240), 1.0f, 0);
	MyRender::getInstance()->getDevice()->BeginScene();
	GameManager::getInstance()->getScene()->Update();
	AutoRef::getInstance()->visitAll();
	ActionManager::getInstance()->Ac();
	GameManager::getInstance()->getScene()->DrawAllActive();
	GameManager::getInstance()->getScene()->DrawAllStatic();
	frame->GetFont()->DrawText(NULL, Text, -1, &rect, DT_TOP | DT_RIGHT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 255.0f));
	MyRender::getInstance()->getDevice()->EndScene();
	MyRender::getInstance()->getDevice()->Present(NULL, NULL, NULL, NULL);
	if (ElapsedTime < 1000 / FRAME)
	{
		Sleep(1000 / FRAME - ElapsedTime);
	}
}