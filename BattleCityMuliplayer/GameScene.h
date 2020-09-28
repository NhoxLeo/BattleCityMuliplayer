
#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "common.h"

class Tank;
class Sound;

class GameScene :public Scene
{
public:

	void setMap(Map* map);

	void LoadMap();

	void setEnemy(int number);

	void Update();
	CREATE_FUNC(GameScene);
	virtual bool init();

	bool checkTank(D3DXVECTOR3 po);

	int checkEnemy();

	void EnemyControl();
	float getWeight(D3DXVECTOR3 op,D3DXVECTOR3 dir);

	void setScene();

	void setPlayerTank();

	void clear();

	void resetScene();
	~GameScene();
private:
	ActiveSprite* flag;
	ActiveSprite* playerSign_T_1;
	ActiveSprite* playerSign_T_2;
	ActiveSprite* playerSign_R_1;
	ActiveSprite* playerSign_R_2;
	ActiveSprite* playerIco_1;
	ActiveSprite* playerIco_2;
	ActiveSprite* background2;
	Tank* MyTank1;
	Tank* MyTank2;
	Map* nowmap;
	int EnemyTimer;
	int nowEnemyNumber;
	int EnemyNumber;
	Map* nowMap;
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
	int counter;
	Sound* StartSound;
};

#endif