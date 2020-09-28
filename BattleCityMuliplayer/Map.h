
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
	/********************************************************
		*  @brief    :	得到所有地图
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	vector<Map*> getMapArray(){ return MapArray; }
	/********************************************************
		*  @brief    :	删掉玩家新建地图
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void removeLast();
};


class Map:public Ref
{
	int m_wall[676];
public:
	CREATE_FUNC(Map);
	/********************************************************
		*  @brief    :	得到当前地图的数据
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int* getMap(){ return m_wall; }
	/********************************************************
		*  @brief    :	初始化该地图
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool init();
	/********************************************************
		*  @brief    :	设置地图数据
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setMap(int wall[]);
	/********************************************************
		*  @brief    :	把地图加载到场景上
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void LoadToScene(GameScene* scene);
	/********************************************************
		*  @brief    :	修改地图
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void fixMap(int position, int type);
	void copyTo(int wall[][26]);//未使用
};

#endif