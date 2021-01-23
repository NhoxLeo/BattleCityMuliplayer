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
#include "NetworkScene.h"
#include "LobbyScene.h"
//#include "Sound.h"
#include "Networks.h"
#include "KeyboardInput.h"

static DWORD dwTime = GetTickCount();
DWORD m_Fps;
DWORD m_ElapsedTime;
DWORD m_PrevTime = GetTickCount();
DWORD m_NowTime;
DWORD m_DeltaTime;

static LONGLONG GlobalPerfCountFrequency;
static LARGE_INTEGER StartTime;
static LARGE_INTEGER EndTime;
static real32 GameUpdateHz;
static real32 TargetSecondsPerFrame;
inline LARGE_INTEGER Win32GetWallClock(void)
{
	LARGE_INTEGER Result;
	QueryPerformanceCounter(&Result);
	return(Result);
}
inline float Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
	float Result = ((float)(End.QuadPart - Start.QuadPart) /
		(float)GlobalPerfCountFrequency);
	return(Result);
}
static void Win32ProcessKeyboardButton(ButtonState* NewState, bool IsDown)
{
	if (IsDown)
	{
		*NewState = Press;
	}
	else
	{
		*NewState = Release;
	}
}

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
		WS_OVERLAPPEDWINDOW,
		0, 0,
		650, 500,
		NULL,
		NULL,
		m_hInstance,
		NULL);
	GameManager::getInstance()->Create(m_hwnd, m_hInstance);
	frame->Create();
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	// Is this query reliable?
	int MonitorRefreshHz = 60;
	HDC RefreshDC = GetDC(m_hwnd);
	int Win32RefreshRate = GetDeviceCaps(RefreshDC, VREFRESH);
	ReleaseDC(m_hwnd, RefreshDC);
	if (Win32RefreshRate > 1)
	{
		MonitorRefreshHz = Win32RefreshRate;
	}
	GameUpdateHz = (MonitorRefreshHz / 1.0f);
	TargetSecondsPerFrame = 1.0f / (real32)GameUpdateHz;
	StartTime = Win32GetWallClock();


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
	MyRender::getInstance()->HandleWindowsEvents(hwnd, msg, wParam, lParam);
	//KeyboardInput::GetInstance()->Create(hwnd);
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
	StartScene* scene = StartScene::create();
	GameManager::getInstance()->setLevel(1);
	//GameManager::getInstance()->setPlayer(1);
	//GameScene* scene = GameScene::create();
	//NetworkScene* scene = NetworkScene::create();
	//LobbyScene* scene = LobbyScene::create();
	GameManager::getInstance()->setScene(scene);
	Controller::getInstance1()->InitControl(m_hwnd, m_hInstance);
	Controller::getInstance2()->InitControl(m_hwnd, m_hInstance);
	Controller::getInstance3()->InitControl(m_hwnd, m_hInstance);
	Controller::getInstance4()->InitControl(m_hwnd, m_hInstance);
	//BASS_Init(-1, 44100, 0, 0, NULL);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/*DWORD dwCurrentTime = GetTickCount();
			DWORD dwElapsedTime = dwCurrentTime - dwTime;
			DWORD fElapsedTime = dwElapsedTime*0.001f;
			Update(fElapsedTime);
			dwTime = dwCurrentTime;*/
			Input.horizontalAxis = Input.verticalAxis = 0;
			Input.buttons[8] = ButtonState::Idle;
			Input.shoot = false;
			shootTime += Time.deltaTime;
			bool SpaceKeyWasDown = false;
			switch (msg.message)
			{
			case WM_QUIT:
				return false;
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
				if (ImGui::GetIO().WantCaptureKeyboard == false)
				{
					unsigned int VKCode = (unsigned int)msg.wParam;
					bool AltKeyWasDown = (msg.lParam & (1 << 29));
					bool ShiftKeyWasDown = (GetKeyState(VK_SHIFT) & (1 << 15));
					bool WasDown = ((msg.lParam & (1UL << 30)) != 0);
					bool IsDown = ((msg.lParam & (1UL << 31)) == 0);

					Input.horizontalAxis = (VKCode == VK_LEFT) ? -1 : ((VKCode == VK_RIGHT) ? 1 : 0);
					Input.verticalAxis = (VKCode == VK_DOWN) ? -1 : ((VKCode == VK_UP) ? 1 : 0);
				}
			case WM_KEYUP:
				if (ImGui::GetIO().WantCaptureKeyboard == false)
				{
					unsigned int VKCode = (unsigned int)msg.wParam;
					bool AltKeyWasDown = (msg.lParam & (1 << 29));
					bool ShiftKeyWasDown = (GetKeyState(VK_SHIFT) & (1 << 15));
					bool WasDown = ((msg.lParam & (1UL << 30)) != 0);
					bool IsDown = ((msg.lParam & (1UL << 31)) == 0);

					if (WasDown != IsDown)
					{
						if (VKCode == VK_UP)
						{
							Win32ProcessKeyboardButton(&Input.actionUp, IsDown);
						}
						else if (VKCode == VK_LEFT)
						{
							Win32ProcessKeyboardButton(&Input.actionLeft, IsDown);
						}
						else if (VKCode == VK_DOWN)
						{
							Win32ProcessKeyboardButton(&Input.actionDown, IsDown);
						}
						else if (VKCode == VK_RIGHT)
						{
							Win32ProcessKeyboardButton(&Input.actionRight, IsDown);
						}
						else if (VKCode == VK_ESCAPE)
						{
							Win32ProcessKeyboardButton(&Input.back, IsDown);
						}
						else if (VKCode == VK_SPACE)
						{
							Win32ProcessKeyboardButton(&Input.buttons[8], IsDown);
							SpaceKeyWasDown = true;
						}
					}
				}
				::TranslateMessage(&msg);
				break;

			default:;
			}
			static bool firstIteration = true;
			if (firstIteration)
			{
				StartTime = Win32GetWallClock();
				firstIteration = false;
			}
			// Time management
			EndTime = Win32GetWallClock();
			Time.frameTime = Win32GetSecondsElapsed(StartTime, EndTime);
			Time.deltaTime = TargetSecondsPerFrame;
			Time.time += (double)Time.deltaTime;
			StartTime = EndTime;

			if (shootTime > 0.5f)
			{
				if (SpaceKeyWasDown)
				{
					//Input.buttons[8] = ButtonState::Press;
					Input.shoot = true;
					shootTime = 0;
				}
			}

			m_NowTime = GetTickCount();
			m_DeltaTime = m_NowTime - m_PrevTime;
			m_ElapsedTime += m_DeltaTime;
			if (m_ElapsedTime > 1000) m_ElapsedTime = 0;
			float deltaTime = static_cast<float>(m_DeltaTime) / 1000.0f;
			deltaTime = deltaTime > 1 ? 0 : deltaTime;
			Update(deltaTime);
			m_PrevTime = m_NowTime;

		}
	}
	return msg.wParam;
}

std::string UnicodeToANSI(const std::wstring& str)
{
	int iTextlen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pEleMentText = new char[iTextlen + 1];
	memset((void*)pEleMentText, 0, sizeof(char) * (iTextlen + 1));
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
	memset(pUnicode, 0, (unicodelen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodelen);
	std::wstring rt(pUnicode);
	delete pUnicode;
	return rt;
};

void MyWindow::Update(float ElapsedTime)
{
	ZeroMemory(&szBuf, sizeof(char) * 10);
	wstring x;
	LPCTSTR Text;
	/*if (ElapsedTime < 1000 / FRAME) sprintf_s(szBuf, "%d", FRAME);
	else sprintf_s(szBuf, "%d", 1 / (ElapsedTime / 1000));*/
	x = ANSIToUnicode(szBuf);
	Text = (LPCWSTR)x.c_str();
	RECT rect = { 0,40, 100, 90 };

	
	/*KeyboardInput::GetInstance()->Update();
	Input.horizontalAxis = (KeyboardInput::GetInstance()->Keyboard_DownState(DIK_A)) ? -1 : ((KeyboardInput::GetInstance()->Keyboard_DownState(DIK_D)) ? 1 : 0);
	Input.verticalAxis = (KeyboardInput::GetInstance()->Keyboard_DownState(DIK_S)) ? -1 : ((KeyboardInput::GetInstance()->Keyboard_DownState(DIK_W)) ? 1 : 0);
	if (shootTime > 0.5f)
	{
		if (KeyboardInput::GetInstance()->Keyboard_DownState(DIK_SPACE))
		{
			Input.buttons[8] = ButtonState::Press;
			Input.shoot = true;
			shootTime = 0;
		}
	}*/

	GameManager::getInstance()->checkclick1();
	GameManager::getInstance()->checkclick2();
	GameManager::getInstance()->checkclick3();
	GameManager::getInstance()->checkclick4();
	MyRender::getInstance()->getDevice()->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 251, 240), 1.0f, 0);

	MyRender::getInstance()->getDevice()->BeginScene();
	MyRender::getInstance()->preUpdateGUI();
	GameManager::getInstance()->getScene()->Update();
	AutoRef::getInstance()->visitAll();
	ActionManager::getInstance()->Ac();
	GameManager::getInstance()->getScene()->DrawAllActive();
	GameManager::getInstance()->getScene()->DrawAllStatic();
	frame->GetFont()->DrawText(NULL, Text, -1, &rect, DT_TOP | DT_RIGHT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 255.0f));
	MyRender::getInstance()->updateGUI();
	MyRender::getInstance()->lateUpdateGUI();
	MyRender::getInstance()->getDevice()->EndScene();

	MyRender::getInstance()->getDevice()->Present(NULL, NULL, NULL, NULL);
	/*if (ElapsedTime < 1000 / FRAME)
	{
		Sleep(1000 / FRAME - ElapsedTime);
	}*/
}