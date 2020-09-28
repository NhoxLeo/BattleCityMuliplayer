/**************************************************************************
*  @file	 : Textrue.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : ������
**************************************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "MyRender.h"

class Texture
{
	LPDIRECT3DTEXTURE9 m_D3DTexture;
	UINT m_Height;
	UINT m_width;
public:
	Texture(){};
	~Texture();
	/********************************************************
		*  @brief    :	����D3D����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool LoadTexture(LPCTSTR path, UINT width, UINT height, D3DCOLOR color);
	/********************************************************
		*  @brief    :	�õ���ǰD3D����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	LPDIRECT3DTEXTURE9 getTexture();
	void setHeight(UINT height);
	void setwidth(UINT width);
	UINT getHeight();
	UINT getwidth();
};

#endif