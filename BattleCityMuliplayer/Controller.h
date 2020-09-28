/**************************************************************************
*  @file	 : Controller.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : ���ƹ�����
**************************************************************************/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "common.h"

/**************************************************************************
*  @file	 : Controller.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : ���������豸
**************************************************************************/
class ControlInput
{
private:
	static LPDIRECTINPUT8 m_DirectInput;
protected:
	ControlInput(){};
	/********************************************************
		*  @brief    :	������������豸
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool CreateControlInput(HINSTANCE hInstance);
	/********************************************************
		*  @brief    :	�õ���������豸
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
*  @brief    : ���������豸
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
		*  @brief    :	�������������豸
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool CreateKeyBoardDevice(HINSTANCE hInstance,HWND hwnd);
	/********************************************************
		*  @brief    :	��ʼ�����������豸
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool InitKeyBoard(HWND hwnd);
	/********************************************************
		*  @brief    :	�õ���ǰ����״̬
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
*  @brief    : ������
**************************************************************************/
class Controller :public ControllerKeyBoard
{
public:
	/********************************************************
		*  @brief    :	����������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool InitControl(HWND hwnd,HINSTANCE hInstance);
	/********************************************************
		*  @brief    :	�õ������������������ƣ��������м��ȡ���������,�˴��ݲ�����
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
		*  @brief    :	�õ���ǰ����״̬
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
		*  @brief    :	���¼���״̬������
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