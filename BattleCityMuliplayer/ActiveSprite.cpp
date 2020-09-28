#include "ActiveSprite.h"
#include "GameManager.h"
#include "Scene.h"
#include "Tank.h"
#include "StaticSprite.h"
#include "Bullet.h"

ActiveSprite* ActiveSprite::create()
{
	ActiveSprite *activesprite = new ActiveSprite();
	if (activesprite&&activesprite->init())
	{
		activesprite->autorelease();
		return activesprite;
	}
	return 0;
}

bool ActiveSprite::init()
{
	Sprite::init();
	camp = false;
	m_speed = Speed(0,0);
	Level = 1;
	Direction = D3DXVECTOR3(0, 1, 0);
	Moveable = true;
	return true;
}

D3DXVECTOR3 ActiveSprite::getDirection()
{
	return Direction;
}

Speed ActiveSprite::getSpeed()
{
	return m_speed;
}

void ActiveSprite::setDirection(D3DXVECTOR3 Direction)
{
	bool x = true;
	if (getDirection() == Direction)
	{
		x = false;
	}
	this->Direction = Direction;
	if (x)
	{
		returnPosition();
	}
}

void ActiveSprite::setSpeed(Speed Sp)
{
	m_speed = Sp;
}

void ActiveSprite::move()
{
	MoveBySpeed* m_move = MoveBySpeed::create(m_speed);
	runAction(m_move);
}

void ActiveSprite::setCamp(bool m_camp)
{
	camp = m_camp;
}

bool ActiveSprite::getCamp()
{
	return camp;
}

void ActiveSprite::removeFromParent()
{
	theScene->removeActiveChild(this);
}

ActiveSprite::~ActiveSprite()
{
 	if (theScene != NULL)
 	{
 		removeFromParent();
 	}
}

void ActiveSprite::returnPosition()
{
	/*int x, y;
	D3DXVECTOR3 position1,position2;
	if (getDirection().x == 0)
	{
		x = (int)(getPosition().x - 112) / 16;
		y = (int)(getPosition().x - 112) % 16;
		if (y >= 8)
		{
			position1 = D3DXVECTOR3(112 + x * 16 + 16, getPosition().y, 0);
			position2 = D3DXVECTOR3(112 + x * 16, getPosition().y, 0);
		}
		else
		{
			position1 = D3DXVECTOR3(112 + x * 16, getPosition().y, 0);
			position2 = D3DXVECTOR3(112 + x * 16 + 16, getPosition().y, 0);
		}
		setPosition(position1);
		for (int i = 0; i < TankArray::getInstance()->getNumber();i++)
		{
			if (ret.Collision(TankArray::getInstance()->getTankArray()[i]->getRet()))
			{
				setPosition(position2);
				return;
			}
		}
		for (int i = 0; i < StaticSpriteArray::getInstance()->getStaticSpriteNumber(); i++)
		{
			if (ret.Collision(StaticSpriteArray::getInstance()->getArray()[i]->getRet()))
			{
				setPosition(position2);
				return;
			}
		}
		return;
	}
	else
	{
		x = (int)(getPosition().y - 44) / 16;
		y = (int)(getPosition().y - 44) % 16;
		if (y >= 8)
		{
			position1 = D3DXVECTOR3(getPosition().x, 44 + x * 16 + 16, 0);
			position2 = D3DXVECTOR3(getPosition().x, 44 + x * 16, 0);
		}
		else
		{
			position1 = D3DXVECTOR3(getPosition().x, 44 + x * 16, 0);
			position2 = D3DXVECTOR3(getPosition().x, 44 + x * 16 + 16, 0);
		}
		setPosition(position1);
		for (int i = 0; i < TankArray::getInstance()->getNumber(); i++)
		{
			if (ret.Collision(TankArray::getInstance()->getTankArray()[i]->getRet()))
			{
				setPosition(position2);
				return;
			}
		}
		for (int i = 0; i < StaticSpriteArray::getInstance()->getStaticSpriteNumber(); i++)
		{
			if (ret.Collision(StaticSpriteArray::getInstance()->getArray()[i]->getRet()))
			{
				setPosition(position2);
				return;
			}
		}
		return;
	}*/
}