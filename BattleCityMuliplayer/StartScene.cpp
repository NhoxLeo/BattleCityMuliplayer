#include "StartScene.h"
#include "Scene.h"
#include "GameManager.h"
#include "ChoiceScene.h"
#include "Map.h"
#include "ActiveSprite.h"
#include "MapScene.h"

bool StartScene::init()
{
	Scene::init();
	GameManager::getInstance()->setLevel(1);
	GameManager::getInstance()->setMyLife(InitLife, 1);
	GameManager::getInstance()->setMyLife(InitLife, 2);
	GameManager::getInstance()->setGrade(1, InitGrade);
	GameManager::getInstance()->setGrade(2, InitGrade);
	CursorPosition = 1;
	ActiveSprite* background = ActiveSprite::create();
	background->LoadTexture(_T("../Sprite/background.jpg"), 640, 480, 1);
	background->setPosition(D3DXVECTOR3(0, 0, 0));
	addActiveChild(background);
	ActiveSprite* startGame = ActiveSprite::create();
	startGame->LoadTexture(_T("../Sprite/splash.png"), 376, 260, 1);
	startGame->setPosition(D3DXVECTOR3(132, 129, 0));
	addActiveChild(startGame);
	Cursor = ActiveSprite::create();
	Cursor->LoadTexture(_T("../Sprite/Cursor.jpg"), 28, 28, 1);
	Cursor->setPosition(D3DXVECTOR3(222, 279, 0));
	addActiveChild(Cursor);
	return true;
}

void StartScene::Update()
{
	if (GameManager::getInstance()->getClick3() == ESCBUTTON_UP)
	{
		PostQuitMessage(0);
	}
	if (GameManager::getInstance()->getClick3() == UPBUTTON_UP)
	{
		if (CursorPosition != 1)
		{
			Cursor->setPosition(D3DXVECTOR3(222, Cursor->getPosition().y - 30, 0));
			CursorPosition--;
		}
	}
	if (GameManager::getInstance()->getClick3() == DOWNBUTTON_UP)
	{
		if (CursorPosition != 4)
		{
			Cursor->setPosition(D3DXVECTOR3(222, Cursor->getPosition().y + 30, 0));
			CursorPosition++;
		}
	}
	if (GameManager::getInstance()->getClick2() == SPACEBUTTON_UP)
	{
		if (CursorPosition == 1)
		{
			GameManager::getInstance()->setPlayer(1);
		}
		else if (CursorPosition == 2)
		{
			GameManager::getInstance()->setPlayer(2);
		}
		else if (CursorPosition == 4)
		{
			GameManager::getInstance()->setPlayer(3);
		}
		else if (CursorPosition == 3)
		{
			MapScene* scene = MapScene::create();
			GameManager::getInstance()->setScene(scene);
			release();
			return;
		}
		ChoiceScene* scene = ChoiceScene::create();
		GameManager::getInstance()->setScene(scene);
		release();
		return;
	}
}