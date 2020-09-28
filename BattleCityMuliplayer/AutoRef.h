/**************************************************************************
*  @file	 : AutoRef.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : �ڴ��Զ�������
**************************************************************************/
#ifndef AUTOREF_H
#define AUTOREF_H

#include <vector>
using namespace std;

class Ref;
/**************************************************************************
*  @file	 : AutoRef.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : �Զ�������
**************************************************************************/
class AutoRef
{
	AutoRef(){};
public:
	static AutoRef* getInstance();
	~AutoRef();
	void addRef(Ref* ref);
	/********************************************************
		*  @brief    :	���������Զ���������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	
		********************************************************/
	void visitAll();
	/********************************************************
		*  @brief    :	�Ƴ�ĳ������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void removeRef(Ref* ref);
protected:
	static int RefNumber;
	static vector<Ref*> AutoReleaseRef;
};

#endif