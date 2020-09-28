/**************************************************************************
*  @file	 : ReleaseManager.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 自动内存管理器类
**************************************************************************/
#ifndef RELEASEMANAGER_H
#define RELEASEMANAGER_H

#include "AutoRef.h"

class ReleaseManager
{
	ReleaseManager(){};
public:
	static  ReleaseManager* getInstance();
	~ReleaseManager();
	/********************************************************
		*  @brief    :	加入内存自动管理
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void addRelease(Ref* ref);
	/********************************************************
		*  @brief    :	检测所有个体
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void checkRelease();
};

#endif