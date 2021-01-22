#include "OverScene.h"
#include "ActiveSprite.h"
#include "common.h"
#include "StartScene.h"
#include "GameManager.h"
#include "Sound.h"
#include "Networks.h"

bool OverScene::init()
{
	Scene::init();
	//oversound = Sound::create(_T("../sound/Lose.mp3"), false);
	//oversound->Play();
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
	/*counter++;
	if (counter == 5*FRAME)
	{
		StartScene* scene = StartScene::create();
		GameManager::getInstance()->setScene(scene);
		release();
		return;
	}*/

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