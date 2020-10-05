#pragma once
#include "common.h"
#include "Scene.h"

class Tank;

class NetworkScene : public Scene
{
public:
	CREATE_FUNC(NetworkScene);
	virtual bool init();
	void Update();
	void Update(float deltaTime);
	void clear();
	void LoadMap();
	void DebugUINT(UINT uint);
	void DebugBool(bool b);
private:
	ActiveSprite* background;
	Tank* MyTank1;
	Map* nowmap;
	bool isServer = false;
	bool isClient = false;
};

