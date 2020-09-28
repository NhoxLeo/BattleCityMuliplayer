#ifndef AWARD_H
#define AWARD_H

#include "Sprite.h"

class Scene;

enum AwardType{ AddLife, TimeStop, StrengBase, Bommb, StrengBullet, UnTouch };
class Award :public Sprite
{
private:
	int m_type;
	int Timer;
	Scene* thescene;
public:
	void Update();
	int getType(){ return m_type; }
	bool init();
	CREATE_FUNC(Award);
	~Award();
	void setScene(Scene* scene){ thescene = scene; }
	Scene* getScene(){ return thescene; }
};

#endif