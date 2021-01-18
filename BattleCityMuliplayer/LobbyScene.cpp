#include "LobbyScene.h"
#include "GameManager.h"
#include "Tank.h"
#include "Bullet.h"
#include "Map.h"
#include "StaticSprite.h"
#include "ActiveSprite.h"
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

	/*MyTank1 = Tank::create();
	MyTank1->setPlayer(1);
	MyTank1->setCamp(true);
	MyTank1->setPosition(D3DXVECTOR3(272, 444, 0));
	addActiveChild(MyTank1);

	MyTank2 = Tank::create();
	MyTank2->setPlayer(2);
	MyTank2->setCamp(true);
	MyTank2->setPosition(D3DXVECTOR3(126, 58, 0));
	addActiveChild(MyTank2);*/

	Scene* scene = this;
	GameManager::getInstance()->setScene(scene);
	LoadMap();
	return true;
}
void LobbyScene::Update()
{
	Scene::Update();
	static int localServerPort = 8888;
	if (GameManager::getInstance()->GetModNetServer() == nullptr && GameManager::getInstance()->GetModNetClient() == nullptr)
	{
		//GameManager::getInstance()->GetModLinkingContext()->clear();
		//TankArray::getInstance()->removeAllTank();

		ImGui::Begin("Main Menu");
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);
		ImGui::Spacing();
		ImGui::Text("Server");
		ImGui::InputInt("Server port", &localServerPort);
		if (ImGui::Button("Start server"))
		{
			GameManager::getInstance()->CreateServer();
			GameManager::getInstance()->GetModNetServer()->setEnabled(true);
			GameManager::getInstance()->GetModNetServer()->setListenPort(8888);
			Module* modNetServerptr = GameManager::getInstance()->GetModNetServer();
			modNetServerptr->start();
		}
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("Client");
		static char serverAddressStr[64] = "127.0.0.1";
		ImGui::InputText("Server address", serverAddressStr, sizeof(serverAddressStr));
		static int remoteServerPort = 8888;
		ImGui::InputInt("Server port", &remoteServerPort);
		static char playerNameStr[64] = "Player";
		ImGui::InputText("Player name", playerNameStr, sizeof(playerNameStr));
		static bool showInvalidUserName = false;
		if (ImGui::Button("Connect to server"))
		{
			GameManager::getInstance()->CreateClient();
			GameManager::getInstance()->GetModNetClient()->setEnabled(true);
			GameManager::getInstance()->GetModNetClient()->setServerAddress(serverAddressStr, remoteServerPort);
			GameManager::getInstance()->GetModNetClient()->setPlayerInfo(playerNameStr);
			Module* modNetClientptr = GameManager::getInstance()->GetModNetClient();
			if (modNetClientptr->needsStart())
			{
				modNetClientptr->updateEnabledState();
				if (modNetClientptr->start() == false);
			}
		}
		ImGui::PopItemWidth();
		ImGui::End();
	}
	if (GameManager::getInstance()->GetModNetServer())
	{
		Module* modNetServerptr = GameManager::getInstance()->GetModNetServer();
		modNetServerptr->preUpdate();
		modNetServerptr->update();
		modNetServerptr->gui();
	}
	if (GameManager::getInstance()->GetModNetClient())
	{
		Module* modNetClientptr = GameManager::getInstance()->GetModNetClient();
		modNetClientptr->preUpdate();
		modNetClientptr->update();
		modNetClientptr->gui();
		if (modNetClientptr->needsStop())
		{
			modNetClientptr->cleanUp();
			modNetClientptr->stop();
			delete[] modNetClientptr;
			GameManager::getInstance()->DeleteClient();
		}
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
