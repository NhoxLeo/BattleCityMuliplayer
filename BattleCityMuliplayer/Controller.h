/**************************************************************************
*  @file	 : Controller.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 控制管理器
**************************************************************************/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "common.h"

/**************************************************************************
*  @file	 : Controller.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 控制输入设备
**************************************************************************/
class ControlInput
{
private:
	static LPDIRECTINPUT8 m_DirectInput;
protected:
	ControlInput(){};
	/********************************************************
		*  @brief    :	创建输入控制设备
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool CreateControlInput(HINSTANCE hInstance);
	/********************************************************
		*  @brief    :	得到输入控制设备
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	LPDIRECTINPUT8 getControlInput();
};

/**************************************************************************
*  @file	 : Controller.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 键盘输入设备
**************************************************************************/
class ControllerKeyBoard:public ControlInput
{
protected:
	LPDIRECTINPUTDEVICE8 m_KeyDevice;
	char m_KeyBuffer[256];
	char m_KeyStore[256];
protected:
	ControllerKeyBoard();
	/********************************************************
		*  @brief    :	创建键盘输入设备
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool CreateKeyBoardDevice(HINSTANCE hInstance,HWND hwnd);
	/********************************************************
		*  @brief    :	初始化键盘输入设备
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool InitKeyBoard(HWND hwnd);
	/********************************************************
		*  @brief    :	得到当前键盘状态
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool getKeyState();
};

/**************************************************************************
*  @file	 : Controller.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 控制器
**************************************************************************/
class Controller :public ControllerKeyBoard
{
public:
	/********************************************************
		*  @brief    :	创建控制器
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool InitControl(HWND hwnd,HINSTANCE hInstance);
	/********************************************************
		*  @brief    :	得到控制器单例（不完善，需设置中间层取消多个单例,此次暂不处理）
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	static Controller* getInstance1();
	static Controller* getInstance2();
	static Controller* getInstance3();
	static Controller* getInstance4();
	/********************************************************
		*  @brief    :	得到当前按键状态
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	UINT getKey1();
	UINT getKey2();
	UINT getKey3();
	UINT getKey4();
	/********************************************************
		*  @brief    :	更新键盘状态储存区
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void UpdateStore();
private:
	Controller(){};
};

#endif