/**************************************************************************
*  @file	 : MyRender.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : ��Ⱦ������
**************************************************************************/
#ifndef MYRENDER_H
#define MYRENDER_H

#include "common.h"

class MyRender
{
	MyRender();
public:
	/********************************************************
		*  @brief    :	��ʼ����Ⱦ�豸
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool InitRender(HWND hwnd);
	/********************************************************
		*  @brief    :	�õ���Ⱦ�豸
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	LPDIRECT3DDEVICE9 getDevice();
	/********************************************************
		*  @brief    :	������Ⱦ����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	static MyRender* getInstance();
	void updateGUI();
	LRESULT HandleWindowsEvents(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	~MyRender();
protected:
	LPDIRECT3D9 m_Direct3D9;
	LPDIRECT3DDEVICE9 m_D3DDEVICE9;
};

#endif