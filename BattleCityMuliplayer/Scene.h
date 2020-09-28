
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Ref.h"

class ActiveSprite;
class StaticSprite;
class Map;
class Award;

class Scene:public Ref
{
public:

	bool init();
	CREATE_FUNC(Scene);

	void addActiveChild(ActiveSprite* child);
	
	void addStaticChild(StaticSprite* child);
	
	void DrawAllActive();
	
	virtual void DrawAllStatic();

	void removeActiveChild(ActiveSprite* sp);
	
	void removeStaticChild(StaticSprite* sp);
	~Scene();
	
	virtual void Update(){};
	
	void homeBoom();
	
	void setAward();

	void clearAward(){ nowAward = NULL; }

	Award* getAward(){ return nowAward; }
protected:
	int ActiveNumber;
	int StaticNumber;
	vector<ActiveSprite*> Active_children;
	vector<StaticSprite*> Static_children;
	vector<StaticSprite*> home;
	bool win;
	Award* nowAward;
};
#endif