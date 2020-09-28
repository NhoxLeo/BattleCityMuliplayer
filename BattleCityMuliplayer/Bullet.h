
#ifndef BULLET_H
#define BULLET_H

#define BulletSpeed (Speed(0,0))

#include "Tank.h"

class Bullet :public ActiveSprite
{
public:
	~Bullet();
	Bullet(){};

	bool initWithParent(Tank* tank);

	static Bullet* createWithParent(Tank* tank);

	void setParent(Tank* tank);

	Tank* getParent();

	void Update();

	void BigBoom();
	void SmallBoom();

	bool getBoom(){ return boom; }

	void SpeedUp();
private:
	Tank* Parent;
	int counter;
	bool boom;
};

class BulletArray
{
private:
	BulletArray(){};
protected:
	int BulletNumber;
	vector<Bullet*> Bulletarray;
public:
	static BulletArray* getInstance();

	int getNumber(){ return BulletNumber; }

	vector<Bullet*> getBulletArray(){ return Bulletarray; }

	void VisitAll();

	void pushBullet(Bullet* m_Bullet)
	{
		Bulletarray.push_back(m_Bullet);
		BulletNumber++;
	}

	void removeBullet(Bullet* bullet);
};

#endif