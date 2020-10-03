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
	void clear();
	void LoadMap();
	const char* UINTtoChar(UINT uint);
private:
	ActiveSprite* background;
	Tank* MyTank1;
	Map* nowmap;
};

