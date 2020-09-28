#include "OverScene.h"
#include "ActiveSprite.h"
#include "common.h"
#include "StartScene.h"
#include "GameManager.h"
#include "Sound.h"

bool OverScene::init()
{
	Scene::init();
	oversound = Sound::create(_T("../sound/Lose.mp3"), false);
	oversound->Play();
	counter = 0;
	ActiveSprite* background = ActiveSprite::create();
	background->LoadTexture(_T("../Sprite/background.jpg"), 640, 480, 1);
	background->setPosition(D3DXVECTOR3(0, 0, 0));
	addActiveChild(background);
	ActiveSprite* sp = ActiveSprite::create();
	sp->LoadTexture(_T("../Sprite/gameover.bmp"), 248, 160, 1);
	sp->setPosition(D3DXVECTOR3(196, 160, 0));
	addActiveChild(sp);
	return true;
}

void OverScene::Update()
{
	counter++;
	if (counter == 5*FRAME)
	{
		StartScene* scene = StartScene::create();
		GameManager::getInstance()->setScene(scene);
		release();
		return;
	}
}