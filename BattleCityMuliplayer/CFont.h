/**************************************************************************
*  @Copyright (c) 2014, NIng, All rights reserved.
*  @file	 : CFont.h
*  @version  : ver 1.0
*  @author   : NIng
*  @date     : 2014/11/27 10:35
*  @brief    : ������
**************************************************************************/
#ifndef _CFONT_H_
#define _CFONT_H_
#include "common.h"
class CFont
{
public:
	CFont();
	~CFont();
	/********************************************************
	*  @brief    :	��������
	*  @input	 :
	*  @output	 :
	*  @return   :	HRESULT
	*  @author   :	NIng	2014/11/27 10:33
	********************************************************/
	HRESULT Create();
	/********************************************************
	*  @brief    :	�õ�����
	*  @input	 :
	*  @output	 :
	*  @return   :	LPD3DXFONT
	*  @author   :	NIng	2014/11/27 10:33
	********************************************************/
	LPD3DXFONT GetFont();
private:
	CFont(const CFont&);
	CFont& operator=(const CFont&);
private:
	LPD3DXFONT m_pD3DXfont;
};



#endif
