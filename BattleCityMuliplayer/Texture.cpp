#include "Texture.h"

UINT Texture::getHeight()
{
	return m_Height;
}

UINT Texture::getwidth()
{
	return m_width;
}

LPDIRECT3DTEXTURE9 Texture::getTexture()
{
	return m_D3DTexture;
}

bool Texture::LoadTexture(LPCTSTR path, UINT width, UINT height, D3DCOLOR color)
{
	m_Height = height;
	m_width = width;
	bool x = SUCCEEDED(D3DXCreateTextureFromFileEx(
		MyRender::getInstance()->getDevice(),
		path,
		m_width,
		m_Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		color,
		0,
		0,
		&m_D3DTexture
		));
 	return x;
}