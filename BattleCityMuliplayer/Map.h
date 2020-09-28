
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
		*  @brief    :	�õ����е�ͼ
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	vector<Map*> getMapArray(){ return MapArray; }
	/********************************************************
		*  @brief    :	ɾ������½���ͼ
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
		*  @brief    :	�õ���ǰ��ͼ������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int* getMap(){ return m_wall; }
	/********************************************************
		*  @brief    :	��ʼ���õ�ͼ
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool init();
	/********************************************************
		*  @brief    :	���õ�ͼ����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setMap(int wall[]);
	/********************************************************
		*  @brief    :	�ѵ�ͼ���ص�������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void LoadToScene(GameScene* scene);
	/********************************************************
		*  @brief    :	�޸ĵ�ͼ
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void fixMap(int position, int type);
	void copyTo(int wall[][26]);//δʹ��
};

#endif