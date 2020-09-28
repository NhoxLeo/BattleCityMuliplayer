/**************************************************************************
*  @file	 : ActionManager.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : ������������
**************************************************************************/
#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "Action.h"

class ActionManager
{
	ActionManager(){};
public:
	/********************************************************
		*  @brief    :	�õ������������ĵ���
		*  @input	 :
		*  @output	 :
		*  @return   :  ActionManager*
		*  @author   :	Alex
		********************************************************/
	static ActionManager* getInstance();
	~ActionManager();
	/********************************************************
		*  @brief    :	���ö�������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	
		********************************************************/
	void setTarget(Sprite* target, Action* action);
	/********************************************************
		*  @brief    :	ִ����������״̬�Ķ���
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	
		********************************************************/
	void Ac();
};

#endif