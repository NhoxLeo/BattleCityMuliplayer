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
	void setType(int _type)
	{
		m_type = _type;
		setRenderRet(30, 28, 30 * m_type, 0);
	}
	bool init();
	CREATE_FUNC(Award);
	~Award();
	void setScene(Scene* scene){ thescene = scene; }
	Scene* getScene(){ return thescene; }
};

#endif