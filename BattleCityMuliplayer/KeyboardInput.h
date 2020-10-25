#pragma once
#include "common.h"

enum class KeyState
{
	None = 0,
	Down,
	Pressed,
	Up
};

enum MouseButton
{
	Left = VK_LBUTTON,
	Middle = VK_MBUTTON,
	Right = VK_RBUTTON
};

class KeyboardInput 
{
private:
	bool m_PrevKeyState[256];
	bool m_NowKeyState[256];

private:
	D3DXVECTOR3 m_MousePosition;
	bool m_MouseHandled;

public:
	static KeyboardInput* getInstance();

	KeyboardInput();
	~KeyboardInput();

	void Update();

	void Clear();

public:
	KeyState GetKeyState(int key);

public:
	KeyState GetMouseButtonState(MouseButton button);
	D3DXVECTOR3 GetMousePosition()
	{
		return m_MousePosition;
	}

	bool GetMouseHandled() {
		return m_MouseHandled;
	}
	void SetMouseHandled(bool handled) {
		m_MouseHandled = handled;
	}

private:
	void UpdateKeyState();
	void UpdateMouseState();
};

