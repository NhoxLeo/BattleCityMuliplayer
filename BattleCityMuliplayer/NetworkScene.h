#pragma once
#include "common.h"
#include "Scene.h"
#include "Debug.h"
class Tank;

class NetworkScene : public Scene
{
public:
	CREATE_FUNC(NetworkScene);
	virtual bool init();
	void Update();
	void clear();
	void LoadMap();
	void DebugUINT(UINT uint);
	void DebugBool(bool b);
private:
	ActiveSprite* background;
	Tank* MyTank1;
	Map* nowmap;
	Debug debug;
	ofstream ofs;
	bool isServer = false;
	bool isClient = false;
};

