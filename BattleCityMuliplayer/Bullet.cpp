#include "bullet.h"
#include "GameManager.h"
#include "Tank.h"
#include "StaticSprite.h"

Bullet::~Bullet()
{
	if (Parent!=NULL)
	{
		Parent->removeBullet(this);
	}
	BulletArray::getInstance()->removeBullet(this);
}

bool Bullet::initWithParent(Tank* tank)
{
	ActiveSprite::init();
	LoadTexture(_T("../Sprite/bullet.bmp"), 32, 8, 1);
	BulletArray::getInstance()->pushBullet(this);
	Player = tank->getPlayer();
	counter = 0;
	setParent(tank);
	boom = false;
	camp = tank->getCamp();
	setRet(Ret(Size(8, 8), D3DXVECTOR3(0, 0, 0)));
	m_position = tank->getPosition() + tank->getDirection() * 14;
	m_speed = Speed(tank->getDirection().x*BulletSpeed, tank->getDirection().y*BulletSpeed);
	Level = tank->getBulletLevel();
	if (Level>1)
	{
		SpeedUp();
	}
	Direction = tank->getDirection();
	if (Direction.y == -1)
	{
		setRenderRet(8, 8, 0, 0);
	}
	if (Direction.x == 1)
	{
		setRenderRet(8, 8, 8, 0);
	}
	if (Direction.y == 1)
	{
		setRenderRet(8, 8, 16, 0);
	}
	if (Direction.x == -1)
	{
		setRenderRet(8, 8, 24, 0);
	}
	return true;
}

Bullet* Bullet::createWithParent(Tank* tank)
{
	Bullet* bullet = new Bullet;
	if (bullet->initWithParent(tank))
	{
		bullet->autorelease();
		return bullet;
	}
	return 0;
}

Tank* Bullet::getParent()
{
	return Parent;
}

void Bullet::setParent(Tank* tank)
{
	Parent = tank;
}

void Bullet::Update()
{
	if (!boom)
	{
		setPosition(getPosition() + D3DXVECTOR3(getSpeed().x, getSpeed().y, 0));
	}
	counter--;
	if (counter == 0)
	{
		release();
	}
}

void BulletArray::VisitAll()
{
	for (int t = 0; t < BulletNumber; t++)
	{
		if (!Bulletarray[t]->getBoom())
		{
			D3DXVECTOR3 m_Position = Bulletarray[t]->getPosition();
			for (int k = 0; k < BulletNumber; k++)
			{
				if (Bulletarray[t]->getRet().Collision(BulletSpeed, Bulletarray[t]->getDirection(), Bulletarray[k]->getRet()))
				{
 					GameManager::getInstance()->UpdateColl(Bulletarray[t], Bulletarray[k]);
				}
			}
			for (int k = 0; k < TankArray::getInstance()->getNumber(); k++)
			{
				if (Bulletarray[t]->getRet().Collision(BulletSpeed, Bulletarray[t]->getDirection(), TankArray::getInstance()->getTankArray()[k]->getRet()))
				{
					GameManager::getInstance()->UpdateColl(TankArray::getInstance()->getTankArray()[k], Bulletarray[t]);
				}
			}
			for (int k = 0; k < StaticSpriteArray::getInstance()->getStaticSpriteNumber(); k++)
			{
				if (Bulletarray[t]->getRet().Collision(BulletSpeed, Bulletarray[t]->getDirection(),StaticSpriteArray::getInstance()->getArray()[k]->getRet()))
				{
					GameManager::getInstance()->UpdateColl(StaticSpriteArray::getInstance()->getArray()[k], Bulletarray[t]);
				}
			}
			if (m_Position.x < 112 || m_Position.x>530 || m_Position.y < 44 || m_Position.y>460)
			{
				Bulletarray[t]->detain();
			}
		}
		Bulletarray[t]->Update();
	}
}

void BulletArray::removeBullet(Bullet* bullet)
{
	for (int i = 0; i < BulletNumber; i++)
	{
		if (Bulletarray[i] == bullet)
		{
			Bulletarray.erase(Bulletarray.begin() + i);
			BulletNumber--;
			return;
		}
	}
}

BulletArray* BulletArray::getInstance()
{
	static BulletArray* Instance;
	if (Instance == NULL)
	{
		Instance = new BulletArray();
		Instance->BulletNumber = 0;
	}
	return Instance;
}

void Bullet::BigBoom()
{
	if (Cross)
	{
		LoadTexture(_T("../Sprite/explode2.bmp"), 64, 64, 1);
		setSpeed(Speed(0, 0));
		setCenter(D3DXVECTOR3(32, 32, 0));
		boom = true;
		setRenderRet(64, 64, 0, 0);
		counter = 10;
	}
}

void Bullet::SmallBoom()
{
	if (Cross)
	{
		LoadTexture(_T("../Sprite/explode1.bmp"), 28, 28, 1);
		setSpeed(Speed(0, 0));
		setCenter(D3DXVECTOR3(14, 14, 0));
		setRenderRet(28,28,0,0);
		boom = true;
		counter = 10;
	}
}

void Bullet::SpeedUp()
{
	if (getSpeed().x>0)
	{
		setSpeed(Speed(getSpeed().x + 2, 0));
	}
	else if (getSpeed().x<0)
	{
		setSpeed(Speed(getSpeed().x - 2, 0));
	}
	else if (getSpeed().y<0)
	{
		setSpeed(Speed(0, getSpeed().y - 2));
	}
	else if (getSpeed().y>0)
	{
		setSpeed(Speed(0, getSpeed().y + 2));
	}
}