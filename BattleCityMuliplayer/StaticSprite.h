/**************************************************************************
*  @file	 : StaticSprite.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : ��̬������
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
		*  @brief    :	������̬�������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	static StaticSprite* Create(int Type);
	/********************************************************
	*  @brief    :	�Ѿ�̬����Ӹ��ڵ��Ƴ�
	*  @input	 :
	*  @output	 :
	*  @return   :
	*  @author   :	Alex
	********************************************************/
	void removeFromParent();
	/********************************************************
		*  @brief    :	��ʼ����̬�������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool init(int Type);
	/********************************************************
		*  @brief    :	�õ���̬���������
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
		*  @brief    :	�õ���̬��������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int getStaticSpriteNumber(){ return StaticSpriteNumber; }
	/********************************************************
		*  @brief    :	���뾲̬����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void pushSprite(StaticSprite* sp);
	/********************************************************
		*  @brief    :	�Ƴ���̬����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void removeStaticSprite(StaticSprite* sp);
	static StaticSpriteArray* getInstance();
	/********************************************************
		*  @brief    :	�õ����о�̬����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	vector<StaticSprite*> getArray(){ return StaticArray; }
};

#endif