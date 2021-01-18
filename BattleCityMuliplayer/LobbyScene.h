#pragma once
#include "common.h"
#include "Scene.h"
#include <iostream> 
#include <sstream> 

class Tank;

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
private:
	ActiveSprite* background;
	Tank* MyTank1;
	Tank* MyTank2;

	Map* nowmap;
};

