/**************************************************************************
*  @file	 : Ret.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : 几何类
**************************************************************************/
#ifndef RET_H
#define RET_H

#include "common.h"

//#define INT int
/**************************************************************************
*  @file	 : Ret.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : 尺寸类
**************************************************************************/
class Size
{
	int height, width;
public:
	Size(){};
	Size(int h, int w);
	int getHeight(){ return height; }
	int getwidth(){ return width; }
	void setHeight(int h){ height = h; }
	void setWeight(int w){ width = w; }
};
/**************************************************************************
*  @file	 : Ret.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : 矩形类
**************************************************************************/
class Ret
{
	Size r_size;
	D3DXVECTOR3 r_position;
public:
	Ret(){};
	Ret(Size size, D3DXVECTOR3 position);
	void setHeight(int h);
	void setwidth(int w);
	void setPosition(D3DXVECTOR3 position);
	Size getSize();
	D3DXVECTOR3 getPosition();
	/********************************************************
		*  @brief    :	碰撞检测
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool Collision(Ret);
	bool Collision(int counter ,D3DXVECTOR3 direction, Ret);
};

#endif