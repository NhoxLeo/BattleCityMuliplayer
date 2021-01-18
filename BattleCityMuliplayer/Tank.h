
#ifndef TANK_H
#define TANK_H

#include "ActiveSprite.h"
#include "common.h"

class Bullet;

class Tank :public ActiveSprite
{
public:
	~Tank();
	Tank() {};

	void setTankTexture();

	void BulletLevelAdd();

	void ChangeInvincibleTo(bool Vincible);

	void ChangeInvincible();

	bool getInvincible();

	void fire();

	bool init();

	int getBulletLevel();

	static Tank* create();

	void Update();

	vector<Bullet*> getBullet() { return TheBullet; }

	void BulletReduce() { BulletNumber--; }

	void MaxBulletAdd() { MaxBullet++; }

	void removeBullet(Bullet* b);

	void directionRenderRet();

	int getTimer() { return Timer; }

	void checkInvincible();

	void setLife(int life) { Life = life; }

	int getLife() { return Life; }

	bool getTouchAbleState() { return TouchAble; }
	void setTouchAbleState(bool touchable) { TouchAble = touchable; }

	void setLife();

	void setLevel(int level);

	void changeAwardAble() { AwardAble = !AwardAble; }

	bool getAwardAble() { return AwardAble; }

	void boom();

	void SetIsPlayer(bool _isplayer) { isPlayer = _isplayer; }
	bool IsPlayer() { return isPlayer; }
	void SetShootTrigger(bool _shootTrigger) { shootTrigger = _shootTrigger; }
	bool GetShootTrigger() { return shootTrigger; }

	void SetPreviousPosition(D3DXVECTOR3 _prevPos) { previousPosition = _prevPos; }
	void SetCurrentPosition(D3DXVECTOR3 _curPos) { currentPosition = _curPos; }
	D3DXVECTOR3 GetPreviousPosition() { return previousPosition; }
	D3DXVECTOR3 GetCurrentPosition() { return currentPosition; }
private:
	int Life;
	int Timer;
	int BulletNumber;
	int MaxBullet;
	int BulletLevel = 1;
	bool Invincible = false;
	vector<Bullet*> TheBullet;
	int InvincibleTimer;
	int Grade;
	bool TouchAble;
	bool AwardAble;
	int counter;
	bool m_boom;
	bool isPlayer = true;
	bool shootTrigger = false;
	D3DXVECTOR3 previousPosition, currentPosition;
};

class TankArray
{
private:
	TankArray() {};
protected:
	int TankNumber;
	vector<Tank*> Tankarray;
public:
	static TankArray* getInstance();

	int getNumber() { return TankNumber; }

	vector<Tank*> getTankArray() { return Tankarray; }

	void VisitAll();
	void VisitAll(int _networkID,CollisionCheckMethod method);
	void AllAITankVisitAll();
	void UpdateAITanks();
	void UpdateAllTanks();
	Tank* GetTank(int _networkID);

	void pushTank(Tank* m_Tank)
	{
		Tankarray.push_back(m_Tank);
		TankNumber++;
	}

	void removeTank(Tank* tank);
	void removeAllTank();
};
#endif