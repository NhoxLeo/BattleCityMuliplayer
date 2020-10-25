#include "KeyboardInput.h"

KeyboardInput* KeyboardInput::getInstance()
{
	static KeyboardInput* Instance;
	if (Instance == NULL)
	{
		Instance = new KeyboardInput();
	}
	return Instance;
}

KeyboardInput::KeyboardInput()
	: m_MousePosition(0.f,0.f, 0.f)
	, m_MouseHandled(false)
{
	ZeroMemory(m_PrevKeyState, sizeof(m_PrevKeyState));
	ZeroMemory(m_NowKeyState, sizeof(m_NowKeyState));
}
KeyboardInput::~KeyboardInput()
{
}

void KeyboardInput::Update()
{
	UpdateKeyState();
	//UpdateMouseState();
}

void KeyboardInput::Clear()
{
	// Ű �Է� ���� �ʱ�ȭ
	for (int i = 0; i < 256; i++)
	{
		m_NowKeyState[i] = false;
	}
}

KeyState KeyboardInput::GetKeyState(int key)
{
	bool prevState = m_PrevKeyState[key];
	bool nowState = m_NowKeyState[key];

	if (prevState == false && nowState == true)
	{
		return KeyState::Down;
	}
	else if (prevState == true && nowState == true)
	{
		return KeyState::Pressed;
	}
	else if (prevState == true && nowState == false)
	{
		return KeyState::Up;
	}

	return KeyState::None;
}

KeyState KeyboardInput::GetMouseButtonState(MouseButton button)
{
	return GetKeyState(button);
}

void KeyboardInput::UpdateKeyState()
{
	for (int i = 0; i < 256; i++)
	{
		m_PrevKeyState[i] = m_NowKeyState[i];
		m_NowKeyState[i] = static_cast<bool>(::GetKeyState(i) & 0x8000);
	}
}
void KeyboardInput::UpdateMouseState()
{
	m_MouseHandled = false;

	// ���콺�� ��ġ�� ������
	POINT position;
	GetCursorPos(&position);

	// ������ ��ġ�� ��ȯ
	//ScreenToClient(Application::GetInstance()->GetHWND(), &position);

	m_MousePosition.x = static_cast<float>(position.x);
	m_MousePosition.y = static_cast<float>(position.y);
}
