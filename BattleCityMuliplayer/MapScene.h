#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "Scene.h"
#include "Sprite.h"

class Map;
class Tank;
//class Sprite;

class MapScene :public Scene
{
public:
	void Update();
	CREATE_FUNC(MapScene);
	virtual bool init();
	void DrawAllStatic();
	void changeMap();
	void changeSprite();
	void PositionToUp();
	void PositionToDown();
	void PositionToLeft();
	void PositionToRight();
	void addMap();
	~MapScene();
private:
	int nowPosition[2];
	Map* nowMap;
	Sprite* theSprite[676];
	Sprite* tank;
	Sprite* background;
};

#endif