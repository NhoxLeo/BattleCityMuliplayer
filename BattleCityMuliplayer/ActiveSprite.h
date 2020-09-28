
#ifndef ACTIVESPRITE_H
#define ACTIVESPRITE_H

#include "common.h"
#include "Action.h"
#include "Sprite.h"

class ActiveSprite :public Sprite
{
protected:
	ActiveSprite(){};
	Speed m_speed;
	D3DXVECTOR3 Direction;
	bool camp;
	int Level;
	int Player;
public:
	~ActiveSprite();

	void setPlayer(int p){ Player = p; }

	int getPlayer(){ return Player; }

	void removeFromParent();

	void move();

	bool init();

	void setLevel(int level){ Level = level; }

	int getLevel(){ return Level; }

	void setCamp(bool m_camp);

	bool getCamp();

	void setSpeed(Speed sp);

	Speed getSpeed();

	void setDirection(D3DXVECTOR3 Direction);

	D3DXVECTOR3 getDirection();

	static ActiveSprite* create();

	virtual void Update(){};

	void returnPosition();
};

#endif