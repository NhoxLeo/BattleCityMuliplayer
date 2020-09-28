/**************************************************************************
*  @file	 : MyRender.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 渲染管理器
**************************************************************************/
#ifndef MYRENDER_H
#define MYRENDER_H

#include "common.h"

class MyRender
{
	MyRender();
public:
	/********************************************************
		*  @brief    :	初始化渲染设备
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool InitRender(HWND hwnd);
	/********************************************************
		*  @brief    :	得到渲染设备
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	LPDIRECT3DDEVICE9 getDevice();
	/********************************************************
		*  @brief    :	创建渲染单例
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	static MyRender* getInstance();
	~MyRender();
protected:
	LPDIRECT3D9 m_Direct3D9;
	LPDIRECT3DDEVICE9 m_D3DDEVICE9;
};

#endif