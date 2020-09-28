#include "MapScene.h"
#include "Sprite.h"
#include <tchar.h>
#include "Tank.h"
#include "Map.h"
#include "GameManager.h"
#include "StartScene.h"
#include "Texture.h"

MapScene::~MapScene()
{
	delete[]nowPosition;
	for (int i = 0; i < 676;i++)
	{
		theSprite[i]->release();
	}
	tank->release();
	background->release();
}

bool MapScene::init()
{
	Scene::init();
	nowPosition[0] = 0;
	nowPosition[1] = 0;
	nowMap = Map::create();
	nowMap->retain();
	background = Sprite::create();
	background->LoadTexture(_T("../Sprite/background2.jpg"), 416, 416, 1);
	background->setPosition(D3DXVECTOR3(320, 252, 0));
	background->setRet(Ret(Size(416, 416), background->getPosition()));
	background->retain();
	background->OnShow();
	tank = Sprite::create();
	tank->LoadTexture(_T("../Sprite/Tank/player1/1.png"), 56, 112, 1);
	tank->setRet(Ret(Size(28, 28), D3DXVECTOR3(0, 0, 0)));
	tank->setPosition(D3DXVECTOR3(128, 60, 0));
	tank->setRenderRet(28, 28, 0, 0);
	tank->retain();
	tank->OnShow();
	for (int i = 0; i < 26;i++)
	{
		for (int k = 0; k < 26;k++)
		{
			theSprite[i * 26 + k] = Sprite::create();
			theSprite[i * 26 + k]->LoadTexture(_T("../Sprite/tile.bmp"), 224, 32, 1);
			theSprite[i * 26 + k]->setRet(Ret(Size(16, 16), D3DXVECTOR3(0, 0, 0)));
			theSprite[i * 26 + k]->setPosition(D3DXVECTOR3(120 + k * 16, 52 + i * 16, 0));
			theSprite[i * 26 + k]->setRenderRet(32, 32, 0, 0);
			theSprite[i * 26 + k]->retain();
		}
	}
	return true;
}

void MapScene::Update()
{
	if (GameManager::getInstance()->getClick3() == UPBUTTON_UP)
	{
		PositionToUp();
		return;
	}
	if (GameManager::getInstance()->getClick3() == DOWNBUTTON_UP)
	{
		PositionToDown();
		return;
	}
	if (GameManager::getInstance()->getClick3() == LEFTBUTTON_UP)
	{
		PositionToLeft();
		return;
	}
	if (GameManager::getInstance()->getClick3() == RIGHTBUTTON_UP)
	{
		PositionToRight();
		return;
	}
	if (GameManager::getInstance()->getClick2() == SPACEBUTTON_UP)
	{
		changeMap();
		changeSprite();
		return;
	}
	if (GameManager::getInstance()->getClick4() == ENTERBUTTON_UP)
	{
		addMap();
		StartScene* scene = StartScene::create();
		GameManager::getInstance()->setScene(scene);
		release();
		return;
	}
}

void MapScene::DrawAllStatic()
{
	background->draw();
	for (int i = 0; i < 676;i++)
	{
		theSprite[i]->drawWithPartTexture(*theSprite[i]->getRenderRet()->RenderPoint, *theSprite[i]->getRenderRet()->RenderSize);
	}
	tank->drawWithPartTexture(*tank->getRenderRet()->RenderPoint, *tank->getRenderRet()->RenderSize);
}

void MapScene::changeMap()
{
	if (nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2] < 4)
	{
		nowMap->fixMap(nowPosition[0] * 52 + nowPosition[1] * 2, nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2] + 1);
		nowMap->fixMap(nowPosition[0] * 52 + nowPosition[1] * 2 + 1, nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2]);
		nowMap->fixMap(nowPosition[0] * 52 + nowPosition[1] * 2 + 26, nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2]);
		nowMap->fixMap(nowPosition[0] * 52 + nowPosition[1] * 2 + 27, nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2]);
		return;
	}
	if (nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2] == 4)
	{
		nowMap->fixMap(nowPosition[0] * 52 + nowPosition[1] * 2, 0);
		nowMap->fixMap(nowPosition[0] * 52 + nowPosition[1] * 2 + 1, 0);
		nowMap->fixMap(nowPosition[0] * 52 + nowPosition[1] * 2 + 26, 0);
		nowMap->fixMap(nowPosition[0] * 52 + nowPosition[1] * 2 + 27, 0);
		return;
	}
}

void MapScene::changeSprite()
{
	if (nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2] == 0)
	{
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2]->StopShow();
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 1]->StopShow();
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 26]->StopShow();
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 27]->StopShow();
		return;
	}
	if (nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2] == 1)
	{
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2]->OnShow();
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 1]->OnShow();
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 26]->OnShow();
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 27]->OnShow();
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2]->setRenderRet(16, 16, 0, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 1]->setRenderRet(16, 16, 0, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 26]->setRenderRet(16, 16, 0, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 27]->setRenderRet(16, 16, 0, 0);
		return;
	}
	if (nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2] == 2)
	{
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2]->setRenderRet(16, 16, 32, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 1]->setRenderRet(16, 16, 32, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 26]->setRenderRet(16, 16, 32, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 27]->setRenderRet(16, 16, 32, 0);
		return;
	}
	if (nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2] == 3)
	{
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2]->setRenderRet(16, 16, 64, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 1]->setRenderRet(16, 16, 64, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 26]->setRenderRet(16, 16, 64, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 27]->setRenderRet(16, 16, 64, 0);
		return;
	}
	if (nowMap->getMap()[nowPosition[0] * 52 + nowPosition[1] * 2] == 4)
	{
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2]->setRenderRet(16, 16, 96, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 1]->setRenderRet(16, 16, 96, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 26]->setRenderRet(16, 16, 96, 0);
		theSprite[nowPosition[0] * 52 + nowPosition[1] * 2 + 27]->setRenderRet(16, 16, 96, 0);
		return;
	}
}

void MapScene::PositionToUp()
{
	if (nowPosition[0] != 0)
	{
		nowPosition[0]--;
		tank->setPosition(tank->getPosition() + D3DXVECTOR3(0, -32, 0));
	}
}

void MapScene::PositionToDown()
{
	if (nowPosition[0] != 12)
	{
		nowPosition[0]++;
		tank->setPosition(tank->getPosition() + D3DXVECTOR3(0, 32, 0));
	}
}

void MapScene::PositionToLeft()
{
	if (nowPosition[1] != 0)
	{
		nowPosition[1]--;
		tank->setPosition(tank->getPosition() + D3DXVECTOR3(-32, 0, 0));
	}
}

void MapScene::PositionToRight()
{
	if (nowPosition[1] != 12)
	{
		nowPosition[1]++;
		tank->setPosition(tank->getPosition() + D3DXVECTOR3(32, 0, 0));
	}
}

void MapScene::addMap()
{
	nowMap->fixMap(0, 0);
	nowMap->fixMap(1, 0);
	nowMap->fixMap(26, 0);
	nowMap->fixMap(27, 0);
	nowMap->fixMap(12, 0);
	nowMap->fixMap(13, 0);
	nowMap->fixMap(24, 0);
	nowMap->fixMap(25, 0);
	nowMap->fixMap(38, 0);
	nowMap->fixMap(39, 0);
	nowMap->fixMap(50, 0);
	nowMap->fixMap(51, 0);
	nowMap->fixMap(312, 0);
	nowMap->fixMap(313, 0);
	nowMap->fixMap(338, 0);
	nowMap->fixMap(339, 0);
	nowMap->fixMap(336, 0);
	nowMap->fixMap(337, 0);
	nowMap->fixMap(362, 0);
	nowMap->fixMap(363, 0);
	nowMap->fixMap(609, 1);
	nowMap->fixMap(610, 1);
	nowMap->fixMap(611, 1);
	nowMap->fixMap(612, 1);
	nowMap->fixMap(633, 0);
	nowMap->fixMap(634, 0);
	nowMap->fixMap(635, 1);
	nowMap->fixMap(636, 6);
	nowMap->fixMap(637, 7);
	nowMap->fixMap(638, 1);
	nowMap->fixMap(639, 0);
	nowMap->fixMap(639, 0);
	nowMap->fixMap(659, 0);
	nowMap->fixMap(660, 0);
	nowMap->fixMap(661, 1);
	nowMap->fixMap(662, 8);
	nowMap->fixMap(663, 9);
	nowMap->fixMap(664, 1);
	nowMap->fixMap(665, 0);
	nowMap->fixMap(666, 0);
	MapManager::getInstance()->addMap(nowMap);
	GameManager::getInstance()->changeMapMake();
}