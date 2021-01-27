#include "NetworkScene.h"
#include "GameManager.h"
#include "Tank.h"
#include "Bullet.h"
#include "Map.h"
#include "StaticSprite.h"
#include "ActiveSprite.h"
#include "Networks.h"
#include "KeyboardInput.h"
#include "OverScene.h"



bool NetworkScene::init()
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
	addActiveChild(MyTank1);*/

	/*MyTank2 = Tank::create();
	MyTank2->setPlayer(2);
	MyTank2->setCamp(true);
	MyTank2->setPosition(D3DXVECTOR3(126, 58, 0));
	addActiveChild(MyTank2);*/


	Scene* scene = this;
	GameManager::getInstance()->setScene(scene);
	LoadMap();

	LoadUI();

	isDebug = false;
	debugIndex = 0;
	/*for (int i = 0; i < 20; i++)
	{
		ofstream tempFile(to_string(i) + "Log.txt");
		files.push_back(tempFile);
	}*/

	StartGameTimer = 0;
	win = true;
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

void NetworkScene::LoadUI()
{
	GameManager::getInstance()->setGrade(1, InitGrade);
	GameManager::getInstance()->setGrade(2, InitGrade);

	//Player 1
	{
		playerSign_R_1 = ActiveSprite::create();
		playerSign_R_1->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
		playerSign_R_1->setRenderRet(14, 28, 28, 0);
		playerSign_R_1->setPosition(D3DXVECTOR3(564, 337, 0));
		playerSign_R_1->setRet(Ret(Size(14, 28), playerSign_R_1->getPosition()));
		addActiveChild(playerSign_R_1);

		playerSign_T_1 = ActiveSprite::create();
		playerSign_T_1->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
		playerSign_T_1->setRenderRet(14, 28, 28, 0);
		playerSign_T_1->setPosition(D3DXVECTOR3(30, 17, 0));
		playerSign_T_1->setRet(Ret(Size(14, 28), playerSign_T_1->getPosition()));
		addActiveChild(playerSign_T_1);

		playerIco_1 = ActiveSprite::create();
		playerIco_1->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
		playerIco_1->setRenderRet(14, 14, 14, 0);
		playerIco_1->setPosition(D3DXVECTOR3(557, 353, 0));
		playerIco_1->setRet(Ret(Size(14, 14), playerIco_1->getPosition()));
		addActiveChild(playerIco_1);

		//Score
		for (int i = 0; i < 6; i++)
		{
			ActiveSprite* op = ActiveSprite::create();
			player_1_grade.push_back(op);
			player_1_grade[i]->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
			player_1_grade[i]->setRenderRet(14, 14, 0, 0);
			player_1_grade[i]->setPosition(D3DXVECTOR3(60 + 14 * i, 17, 0));
			player_1_grade[i]->setRet(Ret(Size(14, 14), player_1_grade[i]->getPosition()));
			addActiveChild(player_1_grade[i]);
		}

		/*player_1_blood = ActiveSprite::create();
		player_1_blood->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
		player_1_blood->setPosition(D3DXVECTOR3(571, 353, 0));
		player_1_blood->setRet(Ret(Size(14, 14), player_1_blood->getPosition()));
		addActiveChild(player_1_blood);*/
	}
	//Player 2
	{
		//playerSign_R_2 = ActiveSprite::create();
		//playerSign_R_2->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
		//playerSign_R_2->setRenderRet(14, 28, 56, 0);
		//playerSign_R_2->setPosition(D3DXVECTOR3(564, 375, 0));
		//playerSign_R_2->setRet(Ret(Size(14, 28), playerSign_R_2->getPosition()));
		//addActiveChild(playerSign_R_2);

		//playerSign_T_2 = ActiveSprite::create();
		//playerSign_T_2->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
		//playerSign_T_2->setRenderRet(14, 28, 56, 0);
		//playerSign_T_2->setPosition(D3DXVECTOR3(500, 17, 0));
		//playerSign_T_2->setRet(Ret(Size(14, 28), playerSign_T_2->getPosition()));
		//addActiveChild(playerSign_T_2);

		//playerIco_2 = ActiveSprite::create();
		//playerIco_2->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
		//playerIco_2->setRenderRet(14, 14, 14, 0);
		//playerIco_2->setPosition(D3DXVECTOR3(557, 391, 0));
		//playerIco_2->setRet(Ret(Size(14, 14), playerIco_2->getPosition()));
		//addActiveChild(playerIco_2);

		////Score
		//for (int i = 0; i < 6; i++)
		//{
		//	ActiveSprite* op = ActiveSprite::create();
		//	player_2_grade.push_back(op);
		//	player_2_grade[i]->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
		//	player_2_grade[i]->setRenderRet(14, 14, 0, 0);
		//	player_2_grade[i]->setPosition(D3DXVECTOR3(530 + 14 * i, 17, 0));
		//	player_2_grade[i]->setRet(Ret(Size(14, 14), player_2_grade[i]->getPosition()));
		//	addActiveChild(player_2_grade[i]);
		//}

		///*player_2_blood = ActiveSprite::create();
		//player_2_blood->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
		//player_2_blood->setPosition(D3DXVECTOR3(571, 391, 0));
		//player_2_blood->setRet(Ret(Size(14, 14), player_2_blood->getPosition()));
		//addActiveChild(player_2_blood);*/
	}
	//Enemy list
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			ActiveSprite* op;
			op = ActiveSprite::create();
			op->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
			op->setRenderRet(14, 14, 0, 0);
			op->setPosition(D3DXVECTOR3(557 + j * 14, 60 + i * 14, 0));
			op->setRet(Ret(Size(14, 14), op->getPosition()));
			NumberOfEnemy.push_back(op);
			addActiveChild(op);
		}
	}
	//Level Info
	{
		flag = ActiveSprite::create();
		flag->LoadTexture(_T("../Sprite/flag.bmp"), 32, 32, 1);
		flag->setPosition(D3DXVECTOR3(566, 424, 0));
		flag->setRet(Ret(Size(14, 28), flag->getPosition()));
		addActiveChild(flag);
		for (int i = 0; i < 2; i++)
		{
			ActiveSprite* op = ActiveSprite::create();
			levelArray.push_back(op);
			levelArray[i]->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
			levelArray[i]->setRenderRet(14, 14, 0 + 14 * i, 0);
			levelArray[i]->setPosition(D3DXVECTOR3(590 + 14 * i, 443, 0));
			levelArray[i]->setRet(Ret(Size(14, 14), levelArray[i]->getPosition()));
			addActiveChild(levelArray[i]);
		}
		//for (int i = 0; i < 20; i++) NumberOfEnemy[i]->OnShow();
	}
}

void NetworkScene::Update()
{
	Scene::Update();

	GameManager::getInstance()->UpdateAllPlayerTank();
	//vector<StaticSprite*> asd = StaticSpriteArray::getInstance()->getArray();

	/*MyTank1->Update();
	MyTank1->setSpeed(Speed(0, 0));
	MyTank1->setSpeed(Speed(Input.horizontalAxis, -Input.verticalAxis));
	if (Input.horizontalAxis != 0 || Input.verticalAxis != 0) MyTank1->setDirection(D3DXVECTOR3(Input.horizontalAxis, -Input.verticalAxis, 0));
	if (GameManager::getInstance()->getClick2() == SPACEBUTTON_ON) MyTank1->fire();
	TankArray::getInstance()->VisitAll();
	BulletArray::getInstance()->VisitAll();*/

	/*MyTank2->Update();
	MyTank2->setSpeed(Speed(0, 0));
	MyTank2->setSpeed(Speed(Input.horizontalAxis, -Input.verticalAxis));
	if (Input.horizontalAxis != 0 || Input.verticalAxis != 0) MyTank2->setDirection(D3DXVECTOR3(Input.horizontalAxis, -Input.verticalAxis, 0));
	if (GameManager::getInstance()->getClick2() == SPACEBUTTON_ON) MyTank2->fire();
	TankArray::getInstance()->VisitAll();
	BulletArray::getInstance()->VisitAll();*/

	/*if (GameManager::getInstance()->getClick2() == SPACEBUTTON_ON)
	{
		for (int i = 0; i < 232; i++)
		{
			GameManager::getInstance()->GetCurrentMap()->GetWallArray()->at(i)->SetEnabled(false);
			GameManager::getInstance()->GetCurrentMap()->GetWallArray()->at(i)->SetShow(false);
		}
	}*/

	//static int localServerPort = 8888;
	//if (GameManager::getInstance()->GetModNetServer() == nullptr && GameManager::getInstance()->GetModNetClient() == nullptr)
	//{
	//	//GameManager::getInstance()->GetModLinkingContext()->clear();
	//	//TankArray::getInstance()->removeAllTank();
	//	ImGui::Begin("Main Menu");
	//	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);
	//	ImGui::Spacing();
	//	ImGui::Text("Server");
	//	ImGui::InputInt("Server port", &localServerPort);
	//	if (ImGui::Button("Start server"))
	//	{
	//		GameManager::getInstance()->CreateServer();
	//		GameManager::getInstance()->GetModNetServer()->setEnabled(true);
	//		GameManager::getInstance()->GetModNetServer()->setListenPort(8888);
	//		Module* modNetServerptr = GameManager::getInstance()->GetModNetServer();
	//		modNetServerptr->start();
	//	}
	//	ImGui::Spacing();
	//	ImGui::Separator();
	//	ImGui::Spacing();
	//	ImGui::Text("Client");
	//	static char serverAddressStr[64] = "127.0.0.1";
	//	ImGui::InputText("Server address", serverAddressStr, sizeof(serverAddressStr));
	//	static int remoteServerPort = 8888;
	//	ImGui::InputInt("Server port", &remoteServerPort);
	//	static char playerNameStr[64] = "Player";
	//	ImGui::InputText("Player name", playerNameStr, sizeof(playerNameStr));
	//	static bool showInvalidUserName = false;
	//	if (ImGui::Button("Connect to server"))
	//	{
	//		GameManager::getInstance()->CreateClient();
	//		GameManager::getInstance()->GetModNetClient()->setEnabled(true);
	//		GameManager::getInstance()->GetModNetClient()->setServerAddress(serverAddressStr, remoteServerPort);
	//		GameManager::getInstance()->GetModNetClient()->setPlayerInfo(playerNameStr);
	//		Module* modNetClientptr = GameManager::getInstance()->GetModNetClient();
	//		if (modNetClientptr->needsStart())
	//		{
	//			modNetClientptr->updateEnabledState();
	//			if (modNetClientptr->start() == false);
	//		}
	//	}
	//	ImGui::PopItemWidth();
	//	ImGui::End();
	//}
	//if (GameManager::getInstance()->GetModNetServer())
	//{
	//	Module* modNetServerptr = GameManager::getInstance()->GetModNetServer();
	//	modNetServerptr->preUpdate();
	//	modNetServerptr->update();
	//	modNetServerptr->gui();
	//}
	//if (GameManager::getInstance()->GetModNetClient())
	//{
	//	Module* modNetClientptr = GameManager::getInstance()->GetModNetClient();
	//	modNetClientptr->preUpdate();
	//	modNetClientptr->update();
	//	modNetClientptr->gui();
	//	if (modNetClientptr->needsStop())
	//	{
	//		modNetClientptr->cleanUp();
	//		modNetClientptr->stop();
	//		delete[] modNetClientptr;
	//		GameManager::getInstance()->DeleteClient();
	//	}
	//}

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

	// UI
	{
		n = 100000;
		Grade_1 = GameManager::getInstance()->getGrade(1);
		if (GameManager::getInstance()->getPlayer() == 2) Grade_2 = GameManager::getInstance()->getGrade(2);
		for (int i = 0; i < 6; i++)
		{
			j_1 = Grade_1 / n;
			Grade_1 = Grade_1 % n;
			player_1_grade[i]->setRenderRet(14, 14, 14 * j_1, 0);

			if (GameManager::getInstance()->getPlayer() == 2)
			{
				j_2 = Grade_2 / n;
				Grade_2 = Grade_2 % n;
				player_2_grade[i]->setRenderRet(14, 14, 14 * j_2, 0);
			}
			n = n / 10;
		}
	}


	if (StartGameTimer > 5.0f && StartGameTimer < 6.0f && GameManager::getInstance()->GetModNetServer() != nullptr)
	{
		GameManager::getInstance()->GetModNetServer()->StartGameServerSide();
		StartGameTimer = 7.0f;
	}
	else if (StartGameTimer <= 5.0f) StartGameTimer += Time.deltaTime;

	if (!GameManager::getInstance()->IsWinning())
	{
		/*if (GameManager::getInstance()->getMapMake())
		{
			MapManager::getInstance()->removeLast();
			GameManager::getInstance()->changeMapMake();
		}*/
		OverScene* scene = OverScene::create();
		GameManager::getInstance()->setScene(scene);
		//release();
		return;
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
	GameManager::getInstance()->SetCurrentMap(nowmap);

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
				nowmap->PushWallArray(sp);
			}
			SpritePosition += D3DXVECTOR3(16, 0, 0);
		}
	}
}
