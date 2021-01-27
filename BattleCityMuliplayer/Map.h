
#ifndef MAP_H
#define MAP_H

#include "Ref.h"
#include <vector>
using namespace std;

class Map;
class GameScene;
class StaticSprite;

class MapManager
{
private:
	vector<Map*> MapArray;
	int m_MapNumber;
	MapManager();
public:
	static MapManager* getInstance();

	void addMap(Map*);
	
	void LoadMap(GameScene* scene,int Number);
	~MapManager();
	vector<Map*> getMapArray(){ return MapArray; }
	void removeLast();
};


class Map:public Ref
{
	int m_wall[676];
	vector<StaticSprite*>* walls;
public:
	CREATE_FUNC(Map);
	int* getMap(){ return m_wall; }
	bool init();
	void setMap(int wall[]);
	void LoadToScene(GameScene* scene);
	void fixMap(int position, int type);
	void copyTo(int wall[][26]);//Î´Ê¹ÓÃ
	void PushWallArray(StaticSprite* sp) { walls->push_back(sp); };
	vector<StaticSprite*>* GetWallArray() { return walls; }
};

#endif