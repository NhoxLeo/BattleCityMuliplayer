#pragma once
#include "common.h"
#include "Scene.h"
#include <iostream> 
#include <sstream> 

class Tank;
class Sprite;

class LobbyScene : public Scene
{
public:
	CREATE_FUNC(LobbyScene);
	virtual bool init();
	void Update();
	void clear();
	void LoadMap();
	void DebugUINT(UINT uint);
	void DebugBool(bool b);
	void DrawAllStatic();
private:
	ActiveSprite* background;
	Sprite* tank1;
	Sprite* tank2;
	Map* nowmap;
	float StartGameTimer = 0;
};

