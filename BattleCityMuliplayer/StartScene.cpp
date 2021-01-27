#include "StartScene.h"
#include "Scene.h"
#include "GameManager.h"
#include "ChoiceScene.h"
#include "NetworkScene.h"
#include "LobbyScene.h"
#include "Map.h"
#include "ActiveSprite.h"
#include "MapScene.h"
#include "Networks.h"

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
	if (GameManager::getInstance()->getClick4() == ENTERBUTTON_UP)
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
			GameManager::getInstance()->setPlayer(1);
			LobbyScene* scene = LobbyScene::create();
			GameManager::getInstance()->setScene(scene);
			release();
			return;
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
}