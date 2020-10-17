#include "NetworkScene.h"
#include "GameManager.h"
#include "Tank.h"
#include "Bullet.h"
#include "Map.h"
#include "StaticSprite.h"
#include "ActiveSprite.h"
#include "Networks.h"


bool NetworkScene::init()
{
	Scene::init();

	background = ActiveSprite::create();
	background->LoadTexture(_T("../Sprite/background2.jpg"), 416, 416, 1);
	background->setPosition(D3DXVECTOR3(320, 252, 0));
	background->setRet(Ret(Size(416, 416), background->getPosition()));
	addActiveChild(background);

	MyTank1 = Tank::create();
	MyTank1->setPlayer(1);

	MyTank1->setCamp(true);
	MyTank1->setPosition(D3DXVECTOR3(272, 444, 0));

	addActiveChild(MyTank1);


	Scene* scene = this;
	GameManager::getInstance()->setScene(scene);
	LoadMap();

	return true;
}

void NetworkScene::DebugUINT(UINT uint)
{
	UINT newint = uint;
	char newchar[10] = "";
	sprintf(newchar, "%d", newint);
	ImGui::Text("Debug: ");
	ImGui::Text((const char*)newchar);
}
void NetworkScene::DebugBool(bool b)
{
	if (b)
	{
		ImGui::Text("Debug: ");
		ImGui::Text("True");
	}
	else
	{
		ImGui::Text("Debug: ");
		ImGui::Text("False");
	}
}

void NetworkScene::Update()
{
	static int localServerPort = 8888;
	if (!isServer && !isClient)
	{
		ImGui::Begin("Main Menu");
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);
		ImGui::Spacing();
		ImGui::Text("Server");
		ImGui::InputInt("Server port", &localServerPort);
		if (ImGui::Button("Start server"))
		{
			isServer = true;
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
			isClient = true;
			GameManager::getInstance()->CreateClient();
			GameManager::getInstance()->GetModNetClient()->setEnabled(true);
			GameManager::getInstance()->GetModNetClient()->setServerAddress(serverAddressStr, remoteServerPort);
			GameManager::getInstance()->GetModNetClient()->setPlayerInfo(playerNameStr);
			Module* modNetClientptr = GameManager::getInstance()->GetModNetClient();
			modNetClientptr->start();
		}
		ImGui::PopItemWidth();
		ImGui::End();
	}
	if (isServer)
	{
		Module* modNetServerptr = GameManager::getInstance()->GetModNetServer();
		modNetServerptr->preUpdate();
		modNetServerptr->update();
		modNetServerptr->gui();
	}
	if (isClient)
	{
		Module* modNetClientptr = GameManager::getInstance()->GetModNetClient();
		modNetClientptr->preUpdate();
		modNetClientptr->update();
		modNetClientptr->gui();
	}

	//Debug
	/*UINT newint = MyTank1->getPosition().y;
	char newchar[10] = "";
	sprintf(newchar, "%d", newint);
	ImGui::Text("Debug: ");
	ImGui::Text((const char*)newchar);*/

	//NetworkScene::DebugBool(MyTank1->getAwardAble());


	Scene::Update();
	MyTank1->Update();
	MyTank1->setSpeed(Speed(0, 0));
	if (GameManager::getInstance()->getClick1() == ABUTTON_ON)
	{
		MyTank1->setSpeed(Speed(-1, 0));
		MyTank1->setDirection(D3DXVECTOR3(-1, 0, 0));
	}
	if (GameManager::getInstance()->getClick1() == SBUTTON_ON)
	{
		MyTank1->setSpeed(Speed(0, 1));
		MyTank1->setDirection(D3DXVECTOR3(0, 1, 0));
	}
	if (GameManager::getInstance()->getClick1() == WBUTTON_ON)
	{
		MyTank1->setSpeed(Speed(0, -1));
		MyTank1->setDirection(D3DXVECTOR3(0, -1, 0));
	}
	if (GameManager::getInstance()->getClick1() == DBUTTON_ON)
	{
		MyTank1->setSpeed(Speed(1, 0));
		MyTank1->setDirection(D3DXVECTOR3(1, 0, 0));
	}
	if (GameManager::getInstance()->getClick2() == SPACEBUTTON_ON)
	{
		MyTank1->fire();
	}
	if (GameManager::getInstance()->getClick3() == ESCBUTTON_UP)
	{
		PostQuitMessage(0);
	}
	TankArray::getInstance()->VisitAll();
	BulletArray::getInstance()->VisitAll();
}

void NetworkScene::Update(float deltaTime)
{
	//Time.deltaTime = deltaTime;
	//Time.time += (double)Time.deltaTime;

	static int localServerPort = 8888;
	if (!isServer && !isClient)
	{
		ImGui::Begin("Main Menu");
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);
		ImGui::Spacing();
		ImGui::Text("Server");
		ImGui::InputInt("Server port", &localServerPort);
		if (ImGui::Button("Start server"))
		{
			isServer = true;
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
			isClient = true;
			GameManager::getInstance()->CreateClient();
			GameManager::getInstance()->GetModNetClient()->setEnabled(true);
			GameManager::getInstance()->GetModNetClient()->setServerAddress(serverAddressStr, remoteServerPort);
			GameManager::getInstance()->GetModNetClient()->setPlayerInfo(playerNameStr);
			Module* modNetClientptr = GameManager::getInstance()->GetModNetClient();
			modNetClientptr->start();
		}
		ImGui::PopItemWidth();
		ImGui::End();
	}
	if (isServer)
	{
		Module* modNetServerptr = GameManager::getInstance()->GetModNetServer();
		modNetServerptr->preUpdate();
		modNetServerptr->update();
		modNetServerptr->gui();
	}
	if (isClient)
	{
		if (GameManager::getInstance()->getClick1() == ABUTTON_ON) Input.horizontalAxis = -1;
		else if (GameManager::getInstance()->getClick1() == DBUTTON_ON)Input.horizontalAxis = 1;
		else Input.horizontalAxis = 0;
		if (GameManager::getInstance()->getClick1() == WBUTTON_ON) Input.verticalAxis = 1;
		else if (GameManager::getInstance()->getClick1() == SBUTTON_ON)Input.verticalAxis = -1;
		else Input.verticalAxis = 0;

		Module* modNetClientptr = GameManager::getInstance()->GetModNetClient();
		modNetClientptr->preUpdate();
		modNetClientptr->update();
		modNetClientptr->gui();

		GameObject* tankNetworkObject = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject(GameManager::getInstance()->GetModNetClient()->GetNetworkID());
		if (tankNetworkObject != NULL)
			MyTank1->setPosition(tankNetworkObject->position);
	}

	//Debug
	/*UINT newint = MyTank1->getPosition().y;
	char newchar[10] = "";
	sprintf(newchar, "%d", newint);
	ImGui::Text("Debug: ");
	ImGui::Text((const char*)newchar);*/

	//NetworkScene::DebugBool(MyTank1->getAwardAble());


	Scene::Update();
	MyTank1->Update();
	MyTank1->setSpeed(Speed(0, 0));
	if (GameManager::getInstance()->getClick1() == ABUTTON_ON)
	{
		//MyTank1->setSpeed(Speed(-1, 0));
		//MyTank1->setDirection(D3DXVECTOR3(-1, 0, 0));
	}
	if (GameManager::getInstance()->getClick1() == SBUTTON_ON)
	{
	/*	MyTank1->setSpeed(Speed(0, 1));
		MyTank1->setDirection(D3DXVECTOR3(0, 1, 0));*/
	}
	if (GameManager::getInstance()->getClick1() == WBUTTON_ON)
	{
		/*MyTank1->setSpeed(Speed(0, -1));
		MyTank1->setDirection(D3DXVECTOR3(0, -1, 0));*/
	}
	if (GameManager::getInstance()->getClick1() == DBUTTON_ON)
	{
	/*	MyTank1->setSpeed(Speed(1, 0));
		MyTank1->setDirection(D3DXVECTOR3(1, 0, 0));*/
	}
	if (GameManager::getInstance()->getClick2() == SPACEBUTTON_ON)
	{
		MyTank1->fire();
	}
	if (GameManager::getInstance()->getClick3() == ESCBUTTON_UP)
	{
		PostQuitMessage(0);
	}
	TankArray::getInstance()->VisitAll();
	BulletArray::getInstance()->VisitAll();
}

void NetworkScene::clear()
{
}

void NetworkScene::LoadMap()
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
				addStaticChild(sp);
				sp->setPosition(SpritePosition);
				sp->setRet(Ret(Size(16, 16), sp->getPosition()));
				if (nowmap->getMap()[i * 26 + k] == 6 ||
					nowmap->getMap()[i * 26 + k] == 7 ||
					nowmap->getMap()[i * 26 + k] == 8 ||
					nowmap->getMap()[i * 26 + k] == 9)
				{
					home.push_back(sp);
				}
			}
			SpritePosition += D3DXVECTOR3(16, 0, 0);
		}
	}
}
