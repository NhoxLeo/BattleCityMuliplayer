#include "LobbyScene.h"
#include "GameManager.h"
#include "Tank.h"
#include "Bullet.h"
#include "Map.h"
#include "StaticSprite.h"
#include "ActiveSprite.h"
#include "Sprite.h"
#include "Networks.h"
#include "KeyboardInput.h"

bool LobbyScene::init()
{
	Scene::init();

	background = ActiveSprite::create();
	background->LoadTexture(_T("../Sprite/background2.jpg"), 416, 416, 1);
	background->setPosition(D3DXVECTOR3(320, 252, 0));
	background->setRet(Ret(Size(416, 416), background->getPosition()));
	addActiveChild(background);
	StartGameTimer = 0;
	Scene* scene = this;
	GameManager::getInstance()->setScene(scene);
	//LoadMap();

	tank1 = Sprite::create();
	tank1->LoadTexture(_T("../Sprite/Tank/player1/1.png"), 56, 112, 1);
	tank1->setRet(Ret(Size(28, 28), D3DXVECTOR3(0, 0, 0)));
	tank1->setPosition(D3DXVECTOR3(256, 256, 0));
	tank1->setRenderRet(28, 28, 0, 0);
	tank1->retain();
	tank1->SetShow(false);


	tank2 = Sprite::create();
	tank2->LoadTexture(_T("../Sprite/Tank/player2/1.png"), 56, 112, 1);
	tank2->setRet(Ret(Size(28, 28), D3DXVECTOR3(0, 0, 0)));
	tank2->setPosition(D3DXVECTOR3(384, 256, 0));
	tank2->setRenderRet(28, 28, 0, 0);
	tank2->retain();
	tank2->SetShow(false);

	return true;
}
void LobbyScene::Update()
{
	Scene::Update();

	if (GameManager::getInstance()->GetModNetServer() != nullptr)
	{
		if (GameManager::getInstance()->GetModNetServer()->ClientSize() == 2)
		{
			tank1->SetShow(true);
			tank2->SetShow(true);
			if (StartGameTimer > 4.0f && StartGameTimer < 5.0f)
			{
				GameManager::getInstance()->LoadNetworkScene();
				StartGameTimer = 7.0f;
			}
			else if (StartGameTimer <= 4.0f) StartGameTimer += Time.deltaTime;
		}
		else tank1->SetShow(true);
	}
	else if (GameManager::getInstance()->GetModNetClient() != nullptr)
	{
		if (GameManager::getInstance()->GetModNetClient()->GetNUmberofPlayers() == 2)
		{
			tank1->SetShow(true);
			tank2->SetShow(true);
			if (StartGameTimer > 4.0f && StartGameTimer < 5.0f)
			{
				GameManager::getInstance()->LoadNetworkScene();
				StartGameTimer = 7.0f;
			}
			else if (StartGameTimer <= 4.0f) StartGameTimer += Time.deltaTime;
		}
		else tank1->SetShow(true);
	}
}
void LobbyScene::clear()
{
}
void LobbyScene::LoadMap()
{
	nowmap = Map::create();
	int mapArray1[676] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
		2, 2, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 2, 2,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 9, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	nowmap->setMap(mapArray1);

	D3DXVECTOR3 SpritePosition = D3DXVECTOR3(120, 52, 0);
	for (int i = 0; i < 26; i++)
	{
		SpritePosition = D3DXVECTOR3(120, 16 * i + 52, 0);
		for (int k = 0; k < 26; k++)
		{
			if (nowmap->getMap()[i * 26 + k])
			{
				StaticSprite* sp = StaticSprite::Create(nowmap->getMap()[i * 26 + k]);
				sp->setID(i * 26 + k);
				addStaticChild(sp);
				sp->setPosition(SpritePosition);
				sp->setRet(Ret(Size(16, 16), sp->getPosition()));
				if (nowmap->getMap()[i * 26 + k] == 6 || nowmap->getMap()[i * 26 + k] == 7 || nowmap->getMap()[i * 26 + k] == 8 || nowmap->getMap()[i * 26 + k] == 9) home.push_back(sp);
			}
			SpritePosition += D3DXVECTOR3(16, 0, 0);
		}
	}
}

void LobbyScene::DrawAllStatic()
{
	background->draw();
	tank1->drawWithPartTexture(*tank1->getRenderRet()->RenderPoint, *tank1->getRenderRet()->RenderSize);
	tank2->drawWithPartTexture(*tank2->getRenderRet()->RenderPoint, *tank2->getRenderRet()->RenderSize);
}
