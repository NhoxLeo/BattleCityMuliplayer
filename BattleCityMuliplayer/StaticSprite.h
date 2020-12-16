
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

	static StaticSprite* Create(int Type);

	void removeFromParent();

	bool init(int Type);

	int getType(){ return m_type; }
};

class StaticSpriteArray
{
private:
	int StaticSpriteNumber;
	vector<StaticSprite*> StaticArray;
	StaticSpriteArray(){};
public:

	int getStaticSpriteNumber(){ return StaticSpriteNumber; }

	void pushSprite(StaticSprite* sp);

	void removeStaticSprite(StaticSprite* sp);
	static StaticSpriteArray* getInstance();

	vector<StaticSprite*> getArray(){ return StaticArray; }
};

#endif