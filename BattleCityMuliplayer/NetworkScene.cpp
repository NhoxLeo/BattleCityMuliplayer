#include "NetworkScene.h"
#include "GameManager.h"
#include "Tank.h"
#include "Bullet.h"
#include "Map.h"
#include "StaticSprite.h"
#include "ActiveSprite.h"
#include "Networks.h"
#include "KeyboardInput.h"



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

	MyTank2 = Tank::create();
	MyTank2->setPlayer(2);
	MyTank2->setCamp(true);
	MyTank2->setPosition(D3DXVECTOR3(126, 58, 0));
	addActiveChild(MyTank2);


	Scene* scene = this;
	GameManager::getInstance()->setScene(scene);
	LoadMap();
	
	isDebug = false;
	debugIndex = 0;

	/*for (int i = 0; i < 20; i++)
	{
		ofstream tempFile(to_string(i) + "Log.txt");
		files.push_back(tempFile);
	}*/
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
	Scene::Update();

	GameManager::getInstance()->UpdateAllPlayerTank();
	//vector<StaticSprite*> asd = StaticSpriteArray::getInstance()->getArray();


	MyTank1->Update();
	MyTank1->setSpeed(Speed(0, 0));
	MyTank1->setSpeed(Speed(Input.horizontalAxis, -Input.verticalAxis));
	if (Input.horizontalAxis != 0 || Input.verticalAxis != 0) MyTank1->setDirection(D3DXVECTOR3(Input.horizontalAxis, -Input.verticalAxis, 0));
	if (GameManager::getInstance()->getClick2() == SPACEBUTTON_ON) MyTank1->fire();
	TankArray::getInstance()->VisitAll();
	BulletArray::getInstance()->VisitAll();

	MyTank2->Update();
	MyTank2->setSpeed(Speed(0, 0));
	MyTank2->setSpeed(Speed(Input.horizontalAxis, -Input.verticalAxis));
	if (Input.horizontalAxis != 0 || Input.verticalAxis != 0) MyTank2->setDirection(D3DXVECTOR3(Input.horizontalAxis, -Input.verticalAxis, 0));
	if (GameManager::getInstance()->getClick2() == SPACEBUTTON_ON) MyTank2->fire();
	TankArray::getInstance()->VisitAll();
	BulletArray::getInstance()->VisitAll();


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
			if (modNetClientptr->needsStart())
			{
				modNetClientptr->updateEnabledState();
				if (modNetClientptr->start() == false);
			}
		}
		if (ImGui::Button("Debug log"))
		{
			//debug.Log(MyTank1->getHeight());
			//debug.LogTank(MyTank1);

			//debug.LogTank20(MyTank1);
			
			isDebug = true;
			debugIndex = 0;

		}

		if (ImGui::Button("Read log and save position."))
		{
			string line;
			ifstream fileRead("10Log.txt");
			ofstream fileWrite("20Log.txt");
			if (fileRead.is_open())
			{
				int index=0;
				while (getline(fileRead, line))
				{
					//c = line.c_str();
					//fileWrite << line;
					size_t posv = line.find(": ");
					string v = line.substr(posv + 2);
					string f = line.substr(9,4);
					string i = line.substr(0, 1);

					if (f == "posX")
					{
						stringstream str(v);
						int x=0;
						str >> x;
						debug.SetDebugPositionX(x);
					}
					else if (f == "posY")
					{
						stringstream str(v);
						int y = 0;
						str >> y;
						debug.SetDebugPositionY(y);
					}
					//fileWrite << i;
					//fileWrite << "\n";

					stringstream str(i);
					int z = 0;
					str >> z;
					index = z;

					if (index != 0 && index != debugs.size())
					{
						fileWrite << "Index: ";
						fileWrite << index;
						fileWrite << "\n";

						debugs.push_back(debug);
						fileWrite << debug.GetDebugPosition().x;
						fileWrite << "\n";
						fileWrite << debug.GetDebugPosition().y;
						fileWrite << "\n";
					}
					
				}
				fileWrite << "debugs' size: ";
				fileWrite << debugs.size();

				fileRead.close();
				fileWrite.close();
			}
		

		}

		if (ImGui::Button("Reverse"))
		{
			MyTank1->setPosition(debugs[0].GetDebugPosition());
			MyTank2->setPosition(debugs[1].GetDebugPosition());
			//MyTank2->setPosition();



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
		if (modNetClientptr->needsStop())
		{
			modNetClientptr->cleanUp();
			modNetClientptr->stop();
			delete[] modNetClientptr;
			GameManager::getInstance()->DeleteClient();
			isClient = false;
		}
		/*	uint16 networkGameObjectsCount;
			GameObject* networkGameObjects[MAX_NETWORK_OBJECTS] = {};
			GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObjects(networkGameObjects, &networkGameObjectsCount);
			for (GameObject* a : networkGameObjects)
			{
				if (a != NULL && a->state == GameObject::State::CREATING)
				{
					ImGui::Spacing();
					ImGui::Text(" ID : %f", a->networkId);
					ImGui::Text(" Sync Wait Time : %f", a->syncWaitTime);
				}
			}*/
	}

	if (isDebug == true)
	{
		debugs.clear();
		if (debugIndex < 20)
		{
			ofstream file(to_string(debugIndex) + "Log.txt");

			for (int i = 0; i < TankArray::getInstance()->getTankArray().size(); i++)
			{
				file << "  Tank's posX: ";
				file << TankArray::getInstance()->getTankArray()[i]->getPosition().x;
				file << "\n";
				file << to_string(i + 1) + ".Tank's posY: ";
				file << TankArray::getInstance()->getTankArray()[i]->getPosition().y;
				file << "\n";
				file << "  Tank's speX: ";
				file << TankArray::getInstance()->getTankArray()[i]->getSpeed().x;
				file << "\n";
				file << "  Tank's speY: ";
				file << TankArray::getInstance()->getTankArray()[i]->getSpeed().y;
				file << "\n";
				file << "  Tank's dirX: ";
				file << TankArray::getInstance()->getTankArray()[i]->getDirection().x;
				file << "\n";
				file << "  Tank's dirY: ";
				file << TankArray::getInstance()->getTankArray()[i]->getDirection().y;
				file << "\n-----------------\n";
			}

			file.close();
			debugIndex++;
		}
		else
		{
			isDebug = false;
		}
	}
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
