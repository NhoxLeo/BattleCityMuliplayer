/**************************************************************************
*  @file	 : AutoRef.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : 内存自动管理类
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
*  @brief    : 自动个体类
**************************************************************************/
class AutoRef
{
	AutoRef(){};
public:
	static AutoRef* getInstance();
	~AutoRef();
	void addRef(Ref* ref);
	/********************************************************
		*  @brief    :	遍历所有自动个体数组
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	
		********************************************************/
	void visitAll();
	/********************************************************
		*  @brief    :	移除某个个体
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