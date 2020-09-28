/**************************************************************************
*  @file	 : ReleaseManager.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : �Զ��ڴ��������
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
		*  @brief    :	�����ڴ��Զ�����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void addRelease(Ref* ref);
	/********************************************************
		*  @brief    :	������и���
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void checkRelease();
};

#endif