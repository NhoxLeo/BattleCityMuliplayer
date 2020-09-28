#include "CFont.h"
#include "MyRender.h"

CFont::CFont()
{
	m_pD3DXfont = NULL;
}

CFont::~CFont()
{
	if(m_pD3DXfont != NULL)
	{
		m_pD3DXfont->Release();
	}
}

HRESULT CFont::Create()
{
	D3DXFONT_DESC df;
	ZeroMemory(&df, sizeof(D3DXFONT_DESC));

	df.Height = 25;
	df.Width  = 12;
	df.Weight = 500;
	df.MipLevels = D3DX_DEFAULT;
	df.Italic    = false;
	df.CharSet   = DEFAULT_CHARSET;
	df.OutputPrecision = 0;
	df.Quality = 0;
	df.PitchAndFamily = 0;
	lstrcpy(df.FaceName, _T("Times New Roman"));

	if(FAILED(D3DXCreateFontIndirect(MyRender::getInstance()->getDevice(),
		                   &df,
						   &m_pD3DXfont)))
	{
		MessageBox(NULL, _T("创建字体失败！"), _T("错误信息"), MB_OK);
		return E_FAIL;
	};

	return SEC_E_OK;
}

LPD3DXFONT CFont::GetFont()
{
	return m_pD3DXfont;
}