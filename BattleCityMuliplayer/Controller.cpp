#include "Controller.h"

LPDIRECTINPUT8 ControlInput::m_DirectInput = NULL;

bool ControlInput::CreateControlInput(HINSTANCE hInstance)
{
	if (m_DirectInput)
	{
		return true;
	}
	return SUCCEEDED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL));
}

LPDIRECTINPUT8 ControlInput::getControlInput()
{
	if (m_DirectInput)
	{
		return m_DirectInput;
	}
	return NULL;
}

bool ControllerKeyBoard::InitKeyBoard(HWND hwnd)
{
	if (m_KeyDevice)
	{
		HRESULT hr = m_KeyDevice->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
		{
			MessageBox(hwnd, _T("设置数据格式失败"), _T("错误提示"), MB_OK);
		}
		hr = m_KeyDevice->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			MessageBox(hwnd, _T("设置协调等级失败"), _T("错误提示"), MB_OK);
		}
		hr = m_KeyDevice->Acquire();
		if (FAILED(hr))
		{
			MessageBox(hwnd, _T("获取键盘使用权失败"), _T("错误提示"), MB_OK);
		}
		return true;
	}
	return false;
}

bool ControllerKeyBoard::CreateKeyBoardDevice(HINSTANCE hInstance, HWND hwnd)
{
	if (CreateControlInput(hInstance))
	{
		if (m_KeyDevice)
		{
			return true;
		}
		return SUCCEEDED(getControlInput()->CreateDevice(GUID_SysKeyboard, &m_KeyDevice, NULL));
	}
	return false;
}

ControllerKeyBoard::ControllerKeyBoard()
{
	m_KeyDevice = NULL;
	ZeroMemory(m_KeyBuffer, sizeof(char) * 256);
	ZeroMemory(m_KeyStore, sizeof(char) * 256);
}

bool ControllerKeyBoard::getKeyState()
{
	if (m_KeyDevice)
	{
		if (DIERR_INPUTLOST == m_KeyDevice->GetDeviceState(sizeof(m_KeyBuffer), (LPVOID)m_KeyBuffer))
		{
			m_KeyDevice->Acquire();
			return SUCCEEDED(m_KeyDevice->GetDeviceState(sizeof(m_KeyBuffer), (LPVOID)m_KeyBuffer));
		}
	}
	return false;
}

bool Controller::InitControl(HWND hwnd, HINSTANCE hInstance)
{
	if (CreateKeyBoardDevice(hInstance, hwnd))
	{
		InitKeyBoard(hwnd);
		return true;
	}
	return false;
}

Controller* Controller::getInstance1()
{
	static Controller* Instance;
	if (Instance == NULL)
	{
		Instance = new Controller();
	}
	return Instance;
}

Controller* Controller::getInstance2()
{
	static Controller* Instance;
	if (Instance == NULL)
	{
		Instance = new Controller();
	}
	return Instance;
}

Controller* Controller::getInstance3()
{
	static Controller* Instance;
	if (Instance == NULL)
	{
		Instance = new Controller();
	}
	return Instance;
}

Controller* Controller::getInstance4()
{
	static Controller* Instance;
	if (Instance == NULL)
	{
		Instance = new Controller();
	}
	return Instance;
}

UINT Controller::getKey1()
{
	getKeyState();
	if (m_KeyBuffer[DIK_W] & 0x80)
	{
		if (!(m_KeyStore[DIK_W] & 0x80))
		{
			UpdateStore();
			return WBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return WBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_W] & 0x80)) && (m_KeyStore[DIK_W] & 0x80))
	{
		UpdateStore();
		return WBUTTON_UP;
	}
	if (m_KeyBuffer[DIK_S] & 0x80)
	{
		if (!(m_KeyStore[DIK_S] & 0x80))
		{
			UpdateStore();
			return SBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return SBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_S] & 0x80)) && (m_KeyStore[DIK_S] & 0x80))
	{
		UpdateStore();
		return SBUTTON_UP;
	}
	if (m_KeyBuffer[DIK_A] & 0x80)
	{
		if (!(m_KeyStore[DIK_A] & 0x80))
		{
			UpdateStore();
			return ABUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return ABUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_A] & 0x80)) && (m_KeyStore[DIK_A] & 0x80))
	{
		UpdateStore();
		return ABUTTON_UP;
	}
	if (m_KeyBuffer[DIK_D] & 0x80)
	{
		if (!(m_KeyStore[DIK_D] & 0x80))
		{
			UpdateStore();
			return DBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return DBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_D] & 0x80)) && (m_KeyStore[DIK_D] & 0x80))
	{
		UpdateStore();
		return DBUTTON_UP;
	}
	UpdateStore();
	return 0;
}

UINT Controller::getKey2()
{
	getKeyState();
	if (m_KeyBuffer[DIK_SPACE] & 0x80)
	{
		if (!(m_KeyStore[DIK_SPACE] & 0x80))
		{
			UpdateStore();
			return SPACEBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return SPACEBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_SPACE] & 0x80)) && (m_KeyStore[DIK_SPACE] & 0x80))
	{
		UpdateStore();
		return SPACEBUTTON_UP;
	}
	UpdateStore();
	return 0;
}

UINT Controller::getKey3()
{
	getKeyState();
	if (m_KeyBuffer[DIK_DOWN] & 0x80)
	{
		if (!(m_KeyStore[DIK_DOWN] & 0x80))
		{
			UpdateStore();
			return DOWNBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return DOWNBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_DOWN] & 0x80)) && (m_KeyStore[DIK_DOWN] & 0x80))
	{
		UpdateStore();
		return DOWNBUTTON_UP;
	}
	if (m_KeyBuffer[DIK_UP] & 0x80)
	{
		if (!(m_KeyStore[DIK_UP] & 0x80))
		{
			UpdateStore();
			return UPBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return UPBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_UP] & 0x80)) && (m_KeyStore[DIK_UP] & 0x80))
	{
		UpdateStore();
		return UPBUTTON_UP;
	}
	if (m_KeyBuffer[DIK_LEFT] & 0x80)
	{
		if (!(m_KeyStore[DIK_LEFT] & 0x80))
		{
			UpdateStore();
			return LEFTBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return LEFTBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_LEFT] & 0x80)) && (m_KeyStore[DIK_LEFT] & 0x80))
	{
		UpdateStore();
		return LEFTBUTTON_UP;
	}
	if (m_KeyBuffer[DIK_RIGHT] & 0x80)
	{
		if (!(m_KeyStore[DIK_RIGHT] & 0x80))
		{
			UpdateStore();
			return RIGHTBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return RIGHTBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_RIGHT] & 0x80)) && (m_KeyStore[DIK_RIGHT] & 0x80))
	{
		UpdateStore();
		return RIGHTBUTTON_UP;
	}
	if (m_KeyBuffer[DIK_NUMPAD9] & 0x80)
	{
		if (!(m_KeyStore[DIK_NUMPAD9] & 0x80))
		{
			UpdateStore();
			return NUMBERNINEBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return NUMBERNINEBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_NUMPAD9] & 0x80)) && (m_KeyStore[DIK_NUMPAD9] & 0x80))
	{
		UpdateStore();
		return NUMBERNINEBUTTON_UP;
	}
	if (m_KeyBuffer[DIK_ESCAPE] & 0x80)
	{
		if (!(m_KeyStore[DIK_ESCAPE] & 0x80))
		{
			UpdateStore();
			return ESCBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return ESCBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_ESCAPE] & 0x80)) && (m_KeyStore[DIK_ESCAPE] & 0x80))
	{
		UpdateStore();
		return ESCBUTTON_UP;
	}
	UpdateStore();
	return 0;
}

UINT Controller::getKey4()
{
	getKeyState();
	if (m_KeyBuffer[DIK_NUMPAD0] & 0x80)
	{
		if (!(m_KeyStore[DIK_NUMPAD0] & 0x80))
		{
			UpdateStore();
			return NUMBERZEROBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return NUMBERZEROBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_NUMPAD0] & 0x80)) && (m_KeyStore[DIK_NUMPAD0] & 0x80))
	{
		UpdateStore();
		return NUMBERZEROBUTTON_UP;
	}
	if (m_KeyBuffer[DIK_RETURN] & 0x80)
	{
		if (!(m_KeyStore[DIK_RETURN] & 0x80))
		{
			UpdateStore();
			return ENTERBUTTON_DOWN;
		}
		else
		{
			UpdateStore();
			return ENTERBUTTON_ON;
		}
	}
	if ((!(m_KeyBuffer[DIK_RETURN] & 0x80)) && (m_KeyStore[DIK_RETURN] & 0x80))
	{
		UpdateStore();
		return ENTERBUTTON_UP;
	}
	UpdateStore();
	return 0;
}

void Controller::UpdateStore()
{
	for (int i = 0; i < 256;i++)
	{
		m_KeyStore[i] = m_KeyBuffer[i];
	}
}