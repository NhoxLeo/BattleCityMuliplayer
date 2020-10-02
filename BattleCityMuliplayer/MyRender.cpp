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

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
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

void MyRender::preUpdateGUI()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void MyRender::updateGUI()
{

}

void MyRender::lateUpdateGUI()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT MyRender::HandleWindowsEvents(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);

	return ERROR_SUCCESS;
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

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(m_D3DDEVICE9);

	return true;
}