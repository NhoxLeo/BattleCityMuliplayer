/**************************************************************************
*  @file	 : ActionManager.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : 动作管理器类
**************************************************************************/
#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "Action.h"

class ActionManager
{
	ActionManager(){};
public:
	/********************************************************
		*  @brief    :	得到动作管理器的单例
		*  @input	 :
		*  @output	 :
		*  @return   :  ActionManager*
		*  @author   :	Alex
		********************************************************/
	static ActionManager* getInstance();
	~ActionManager();
	/********************************************************
		*  @brief    :	设置动作对象
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	
		********************************************************/
	void setTarget(Sprite* target, Action* action);
	/********************************************************
		*  @brief    :	执行所有运行状态的动作
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	
		********************************************************/
	void Ac();
};

#endif