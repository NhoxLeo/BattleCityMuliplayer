#pragma once
#include "common.h"
#include "Scene.h"
#include "Debug.h"
#include <iostream> 
#include <sstream> 
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
	Tank* MyTank2;

	Map* nowmap;
	Debug debug;
	vector<Debug> debugs;
	vector<ofstream> files;
	int debugIndex;
	bool isDebug;
	bool isServer = false;
	bool isClient = false;
};

