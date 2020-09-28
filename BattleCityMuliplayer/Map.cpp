#include "Map.h"
#include "GameScene.h"

MapManager::MapManager()
{
	m_MapNumber = 0;
}

MapManager* MapManager::getInstance()
{
	static MapManager* Instance;
	if (Instance == NULL)
	{
		Instance = new MapManager();
		Instance->m_MapNumber = 0;
	}
	return Instance;
}

void MapManager::LoadMap(GameScene* scene, int Number)
{
	scene->setMap(getMapArray()[Number - 1]);
}

MapManager::~MapManager()
{
	for (int i = 0; i < m_MapNumber; i++)
	{
		delete MapArray[i];
	}
	MapArray.clear();
}

void Map::setMap(int wall[])
{
	for (int i = 0; i < 676; i++)
	{
		m_wall[i] = wall[i];
	}
}

bool Map::init()
{
	Ref::init();
	for(int i = 0; i < 676; i++)
	{
		m_wall[i] = 0;
	}
	return true;
}

void MapManager::addMap(Map* map)
{
	map->retain();
	MapArray.push_back(map);
	m_MapNumber++;
}

void MapManager::removeLast()
{
	MapArray[m_MapNumber - 1]->detain();
	MapArray.pop_back();
	m_MapNumber--;
}

void Map::copyTo(int wall[][26])
{
	int t = 0;
	for (int i = 0; i < 26;i++)
	{
		for (int k = 0; k < 26;k++)
		{
			wall[i][k] = m_wall[t];
			t++;
		}
	}
}

void Map::fixMap(int position, int type)
{
	m_wall[position] = type;
}