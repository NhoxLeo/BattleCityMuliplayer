/**************************************************************************
*  @file	 : StaticSprite.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 静态精灵类
**************************************************************************/
#ifndef STATICSPRITE_H
#define STATICSPRITE_H

#include "common.h"
#include "Sprite.h"

enum type{ brick, algam, grass, water, ice, base1,base2,base3,base4 };

class Texture;

class StaticSprite :public Sprite
{
private:
	int m_type;
public:
	~StaticSprite();
	/********************************************************
		*  @brief    :	创建静态精灵对象
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	static StaticSprite* Create(int Type);
	/********************************************************
	*  @brief    :	把静态精灵从父节点移除
	*  @input	 :
	*  @output	 :
	*  @return   :
	*  @author   :	Alex
	********************************************************/
	void removeFromParent();
	/********************************************************
		*  @brief    :	初始化静态精灵对象
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool init(int Type);
	/********************************************************
		*  @brief    :	得到静态精灵的种类
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int getType(){ return m_type; }
};

class StaticSpriteArray
{
private:
	int StaticSpriteNumber;
	vector<StaticSprite*> StaticArray;
	StaticSpriteArray(){};
public:
	/********************************************************
		*  @brief    :	得到静态精灵数量
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int getStaticSpriteNumber(){ return StaticSpriteNumber; }
	/********************************************************
		*  @brief    :	加入静态精灵
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void pushSprite(StaticSprite* sp);
	/********************************************************
		*  @brief    :	移除静态精灵
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void removeStaticSprite(StaticSprite* sp);
	static StaticSpriteArray* getInstance();
	/********************************************************
		*  @brief    :	得到所有静态精灵
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	vector<StaticSprite*> getArray(){ return StaticArray; }
};

#endif