#include "ChoiceScene.h"
#include "Sprite.h"
#include "GameManager.h"
#include "GameScene.h"
#include "StartScene.h"
#include "ActiveSprite.h"

bool ChoiceScene::init()
{
	Scene::init();
	nowLevel = 1;
	decade = ActiveSprite::create();
	decade->setPosition(D3DXVECTOR3(310, 240, 0));
	decade->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
	decade->setRenderRet(14, 14, 0, 0);
	addActiveChild(decade);
	unit = ActiveSprite::create();
	unit->setPosition(D3DXVECTOR3(330, 240, 0));
	unit->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
	unit->setRenderRet(14, 14, 14, 0);
	addActiveChild(unit);
	return true;
}

void ChoiceScene::Update()
{
	if (GameManager::getInstance()->getClick3() == UPBUTTON_DOWN)
	{
		if (nowLevel!=MapNumber)
		{
			GameManager::getInstance()->setLevel(++nowLevel);
		}
	}
	if (GameManager::getInstance()->getClick3() == DOWNBUTTON_DOWN)
	{
		if (nowLevel!=1)
		{
			GameManager::getInstance()->setLevel(--nowLevel);
		}
	}
	if (GameManager::getInstance()->getClick2() == SPACEBUTTON_UP)
	{
		GameScene* scene = GameScene::create();
		GameManager::getInstance()->setScene(scene);
		release();
		return;
	}
	if (GameManager::getInstance()->getClick3() == ESCBUTTON_UP)
	{
		StartScene* scene = StartScene::create();
		GameManager::getInstance()->setScene(scene);
		release();
		return;
	}
	decade->setRenderRet(14, 14, 14 * (nowLevel / 10), 0);
	unit->setRenderRet(14, 14, 14 * (nowLevel % 10), 0);
}