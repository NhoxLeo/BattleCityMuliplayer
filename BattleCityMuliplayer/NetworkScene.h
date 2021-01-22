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
	void LoadUI();
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

	ActiveSprite* flag;
	ActiveSprite* playerSign_T_1;
	ActiveSprite* playerSign_T_2;
	ActiveSprite* playerSign_R_1;
	ActiveSprite* playerSign_R_2;
	ActiveSprite* playerIco_1;
	ActiveSprite* playerIco_2;
	ActiveSprite* background2;
	int Grade_1;
	int Grade_2;
	int n;
	int j_1;
	int j_2;
	vector<ActiveSprite*> levelArray;
	vector<ActiveSprite* >player_1_grade;
	vector<ActiveSprite* >player_2_grade;
	ActiveSprite* player_1_blood;
	ActiveSprite* player_2_blood;
	vector<ActiveSprite*>NumberOfEnemy;
};

