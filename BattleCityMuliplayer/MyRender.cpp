#include "MyRender.h"

MyRender::MyRender()
{
	m_Direct3D9 = NULL;
	m_D3DDEVICE9 = NULL;
}

MyRender::~MyRender()
{
	if (m_D3DDEVICE9)
	{
		m_D3DDEVICE9->Release();
		m_D3DDEVICE9 = NULL;
	}
	if (m_Direct3D9)
	{
		m_Direct3D9->Release();
		m_Direct3D9 = NULL;
	}
}

MyRender* MyRender::getInstance()
{
	static MyRender* render;
	if (render == NULL)
	{
		render = new MyRender();
	}
	return render;
}

LPDIRECT3DDEVICE9 MyRender::getDevice()
{
	return m_D3DDEVICE9;
}

bool MyRender::InitRender(HWND hwnd)
{
	m_Direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	m_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_D3DDEVICE9);
	return true;
}