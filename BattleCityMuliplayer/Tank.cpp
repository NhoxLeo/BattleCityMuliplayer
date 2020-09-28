#include "Bullet.h"
#include "GameManager.h"
#include "StaticSprite.h"
#include "Scene.h"
#include "Ret.h"
#include "Award.h"
#include "Sound.h"

void Tank::BulletLevelAdd()
{
	if (BulletLevel<4)
	{
		BulletLevel++;
	}
	if (BulletLevel == 3)
	{
		MaxBullet++;
	}
}

void Tank::removeBullet(Bullet* b)
{
	if (TheBullet[0] == b)
	{
		TheBullet.erase(TheBullet.begin());
		BulletNumber--;
		return;
	}
	if (TheBullet[1] == b)
	{
		TheBullet.erase(TheBullet.begin() + 1);
		BulletNumber--;
	}
}

Tank* Tank::create()
{
	Tank* tank = new Tank();
	if (tank&&tank->init())
	{
		tank->autorelease();
		return tank;
	}
	return 0;
}

bool Tank::init()
{
	ActiveSprite::init();
	TankArray::getInstance()->pushTank(this);
	LoadTexture(_T("../Sprite/Tank/bore.bmp"), 112, 28, 1);
	//LoadTexture(_T("../Sprite/Tank/player1/1.png"), 56, 112, 1);
	setRet(Ret(Size(28, 28), D3DXVECTOR3(0, 0, 0)));
	setRenderRet(28, 28, 0, 0);
	Invincible = false;
	counter = 0;
	Moveable = false;
	TouchAble = false;
	m_boom = false;
	Grade = 0;
	AwardAble = false;
	MaxBullet = 1;
	Player = 0;
	Timer = 0;
	Life = 1;
	InvincibleTimer = 0;
	BulletNumber = 0;
	return true;
}

void Tank::ChangeInvincible()
{
	Invincible = !Invincible;
	setTankTexture();
	if (Invincible)
	{
		InvincibleTimer++;
	}
	else
	{
		InvincibleTimer = 0;
	}
}

void Tank::ChangeInvincibleTo(bool Vincible)
{
	Invincible = Vincible;
	setTankTexture();
}

void Tank::checkInvincible()
{
	if (Invincible)
	{
		if (InvincibleTimer == FRAME*InvincibleTime)
		{
			ChangeInvincibleTo(false);
		}
		else
		{
			InvincibleTimer++;
		}
	}
}

bool Tank::getInvincible()
{
	return Invincible;
}

int Tank::getBulletLevel()
{
	return BulletLevel;
}

void Tank::fire()
{
	if (Moveable)
	{
		if (BulletNumber < MaxBullet)
		{
			Bullet* bullet = Bullet::createWithParent(this);
			BulletNumber++;
			TheBullet.push_back(bullet);
			GameManager::getInstance()->getScene()->addActiveChild(bullet);
		}
	}
}

void Tank::Update()
{
	counter--;
	if (counter == 0)
	{
		detain();
	}
	if (Timer>10000000)
	{
		Timer = 601;
	}
	Timer++;
	if (Timer == FRAME/2)
	{
		setRenderRet(28, 28, 28, 0);
	}
	if (Timer == FRAME)
	{
		setRenderRet(28, 28, 56,0);
	}
	if (Timer == 3*FRAME/2)
	{
		setRenderRet(28, 28, 28, 0);
	}
	if (Timer == 2*FRAME)
	{
		TouchAble = true;
		Moveable = true;
		if (getCamp())
		{
			ChangeInvincibleTo(false);
		}
		else
		{
			setTankTexture();
		}
	}
	if (Timer > 2*FRAME)
	{
		dirctionRenderRet();
	}
	if (InvincibleTimer%20 == 1)
	{	
		setRenderRet(28, 28, 0, 0);
	}
	if (InvincibleTimer%20 == 11)
	{
		setRenderRet(28, 28, 0, 28);
	}
	if (Life<=0)
	{
		detain();
	}
	checkInvincible();
}


void TankArray::VisitAll()
{
	for (int t = 0; t < TankNumber; t++)
	{
		Tankarray[t]->Update();
		if (Tankarray[t]->getSpeed().x != 0 || Tankarray[t]->getSpeed().y != 0)
		{
			int m_x = Tankarray[t]->getRenderRet()->RenderPoint->x / 56;
			int m_y = 28 - Tankarray[t]->getRenderRet()->RenderPoint->x;
			if (m_y < 0)
			{
				m_y = -m_y;
			}
			Tankarray[t]->setRenderRet(28, 28, m_x * 56 + m_y, Tankarray[t]->getRenderRet()->RenderPoint->y);
		}
			if (Tankarray[t]->getMoveState())
			{
				D3DXVECTOR3 m_Position = Tankarray[t]->getPosition() + D3DXVECTOR3(TankSpeed*Tankarray[t]->getSpeed().x, TankSpeed*Tankarray[t]->getSpeed().y, 0);
				Ret m_Ret = Ret(TankArray::getInstance()->Tankarray[t]->getSize(), m_Position);
				bool x = true; //x true -> movable

				//Tank vs tank collision
				for (int k = 0; k < TankArray::getInstance()->getNumber(); k++)
				{
					if (k != t)
					{
						if (m_Ret.Collision(TankSpeed, Tankarray[t]->getDirection(),getTankArray()[k]->getRet()))
						{
							x = false;
							break;
						}
					}
				}
				//Tank vs wall, brick, items.
				for (int k = 0; k < StaticSpriteArray::getInstance()->getStaticSpriteNumber(); k++)
				{
					if (m_Ret.Collision(TankSpeed, Tankarray[t]->getDirection(), StaticSpriteArray::getInstance()->getArray()[k]->getRet()))
					{
						if (StaticSpriteArray::getInstance()->getArray()[k]->getType() != 3
							&& StaticSpriteArray::getInstance()->getArray()[k]->getType() != 5)
						{
							x = false;
							break;
						}
					}
				}
				
				//Tank vs bound
				if (m_Position.x < 126 || m_Position.x>514 || m_Position.y < 58 || m_Position.y>446)
				{
					x = false;
				}

				//No collision -> x == true.
				if (x)
				{
					getTankArray()[t]->setPosition(m_Position);
				}
				else
				{
					getTankArray()[t]->setSpeed(Speed(0, 0));
				}
			}

			if (Tankarray[t]->getScene()->getAward()!=NULL)
			{
				if (Tankarray[t]->getRet().Collision(Tankarray[t]->getScene()->getAward()->getRet()))
				{
					GameManager::getInstance()->UpdateColl(Tankarray[t]->getScene()->getAward(), Tankarray[t]);
				}
			}
	}
}

void TankArray::removeTank(Tank* tank)
{
	for (int i = 0; i < TankNumber;i++)
	{
		if (Tankarray[i] == tank)
		{
			Tankarray.erase(Tankarray.begin() + i);
			TankNumber--;
			return;
		}
	}
}

TankArray* TankArray::getInstance()
{
	static TankArray* Instance;
	if (Instance == NULL)
	{
		Instance = new TankArray();
		Instance->TankNumber = 0;
	}
	return Instance;
}

Tank::~Tank()
{
	for (int i = 0; i<TheBullet.size();i++)
	{
		TheBullet[i]->setParent(NULL);
	}
	Sprite::~Sprite();
	TankArray::getInstance()->removeTank(this);
}

void Tank::setTankTexture()
{
	if (getInvincible())
	{
		LoadTexture(_T("../Sprite/Tank/shield.bmp"), 28, 56, 1);
		setRenderRet(28, 28, 0, 0);
	}
	else
	{
		if (getCamp())
		{
			if (getPlayer() == 1)
			{
				switch (Level)
				{
				case 1:
					LoadTexture(_T("../Sprite/Tank/player1/1.png"), 56, 112, 1);
					break;
				case 2:
					LoadTexture(_T("../Sprite/Tank/player1/2.png"), 56, 112, 1);
					break;
				case 3:
					LoadTexture(_T("../Sprite/Tank/player1/3.png"), 56, 112, 1);
					break;
				case 4:
					LoadTexture(_T("../Sprite/Tank/player1/4.png"), 56, 112, 1);
					break;
				default:
					break;
				}
			}
			else
			{
				switch (Level)
				{
				case 1:
					LoadTexture(_T("../Sprite/Tank/player2/1.png"), 56, 112, 1);
					break;
				case 2:
					LoadTexture(_T("../Sprite/Tank/player2/2.png"), 56, 112, 1);
					break;
				case 3:
					LoadTexture(_T("../Sprite/Tank/player2/3.png"), 56, 112, 1);
					break;
				case 4:
					LoadTexture(_T("../Sprite/Tank/player2/4.png"), 56, 112, 1);
					break;
				default:
					break;
				}
			}
		}
		else
		{
			switch (Level)
			{
			case 1:
				LoadTexture(_T("../Sprite/Tank/Enemy/1.png"), 112, 112, 1);
				break;
			case 2:
				LoadTexture(_T("../Sprite/Tank/Enemy/2.png"), 112, 112, 1);
				break;
			case 3:
				LoadTexture(_T("../Sprite/Tank/Enemy/3.png"), 224, 112, 1);
				break;
			default:
				break;
			}
		}
	}
}

void Tank::dirctionRenderRet()
{
	if (!m_boom)
	{
		int x = 56, y = 0;
		if (!Invincible)
		{
			if (AwardAble)
			{
				if (Level != 3)
				{
					x = 1 - (getRenderRet()->RenderPoint->x / 56);
					y = (int)getRenderRet()->RenderPoint->x % 56;
					setRenderRet(28, 28, x * 56 + y, getRenderRet()->RenderPoint->y);
				}
				else
				{
					x = 3 - (getRenderRet()->RenderPoint->x / 56);
					y = (int)getRenderRet()->RenderPoint->x % 56;
					setRenderRet(28, 28, x * 56 + y, getRenderRet()->RenderPoint->y);
				}
			}
			if (getDirection().x == 1)
			{
				setRenderRet(28, 28, getRenderRet()->RenderPoint->x, 28);
			}
			if (getDirection().x == -1)
			{
				setRenderRet(28, 28, getRenderRet()->RenderPoint->x, 84);
			}
			if (getDirection().y == 1)
			{
				setRenderRet(28, 28, getRenderRet()->RenderPoint->x, 56);
			}
			if (getDirection().y == -1)
			{
				setRenderRet(28, 28, getRenderRet()->RenderPoint->x, 0);
			}
		}
	}
}

void Tank::setLevel(int level)
{
	Level = level;
	if (level == 2)
	{
		setSpeed(Speed(getSpeed().x * 2, getSpeed().y * 2));
	}
	if (level == 3)
	{
		setLife(3);
	}
}

void Tank::boom()
{
	LoadTexture(_T("../Sprite/explode2.bmp"), 64, 64, 1);
	setCenter(D3DXVECTOR3(32, 32, 0));
	Moveable = false;
	setRenderRet(64, 64, 0, 0);
	counter = 10;
	AwardAble = false;
	TouchAble = false;
	m_boom = true;
}