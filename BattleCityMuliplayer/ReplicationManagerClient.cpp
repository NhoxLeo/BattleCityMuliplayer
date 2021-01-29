#include "Networks.h"
#include "GameManager.h"
#include "StaticSprite.h"
#include "Map.h"
#include "ReplicationManagerClient.h"

void ReplicationManagerClient::read(const InputMemoryStream& packet, uint32 clientNetworkId)
{
	while (packet.RemainingByteCount() > 0)
	{
		uint32 networkId, tickCount, playerID, level;
		ReplicationAction action;
		packet >> networkId;
		packet >> action;
		if (action == ReplicationAction::Create)
		{
			GameObject* go = nullptr;
			go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject(networkId, true);
			if (go != nullptr)
			{
				GameManager::getInstance()->GetModLinkingContext()->unregisterNetworkGameObject(go);
				Destroy(go);
			}
			go = Instantiate();
			GameManager::getInstance()->GetModLinkingContext()->registerNetworkGameObjectWithNetworkId(go, networkId);
			packet >> playerID;
			packet >> level;
			packet >> go->position.x;
			packet >> go->position.y;
			//packet >> go->rotation.x;
			//packet >> go->rotation.y;
			packet >> go->name;
			packet >> go->isAI;
			if (networkId == clientNetworkId)
			{
				go->isPlayer = true;
				go->doInterpolation = false;
			}
			if (go->isPlayer)
			{
				Player* script = new Player();
				script->gameObject = go;
				script->isServer = false;
				go->behaviour = script;
			}
			go->final_position = go->position;
			go->initial_position = go->position;
			go->final_rotation = go->rotation;
			go->initial_rotation = go->rotation;

			if (go->isAI)
				GameManager::getInstance()->CreateAIPlayerTank(networkId, (int)level, go->position);
			else
				GameManager::getInstance()->CreatePlayerTank(networkId, playerID, (int)level, go->position);
		}
		else if (action == ReplicationAction::Update_Position)
		{
			GameObject* go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject(networkId);
			D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), rotation = D3DXVECTOR3(0, 0, 0), speed = D3DXVECTOR3(0, 0, 0);
			float angle;
			bool isShooted;
			packet >> tickCount;
			packet >> position.x;
			packet >> position.y;
			packet >> speed.x;
			packet >> speed.y;
			packet >> isShooted;
			//packet >> angle;
			if (go != nullptr)
			{
				go->tickCount = tickCount;
				go->lateFrames = (int)((GetTickCount() - tickCount) / 16.67f - (REPLICATION_INTERVAL_SECONDS / 0.16f));
				//if (go->clientInstanceNID != 0)	go->doInterpolation = true;
				//go->newReplicationState(position, rotation);
				if (/*!GameManager::getInstance()->GetModGameObject()->interpolateEntities || !go->doInterpolation*/ true)
				{
					go->position = position;
					go->rotation = D3DXVECTOR3(speed.x, speed.y, 0);
					go->speed = speed;
					if (networkId == GameManager::getInstance()->GetModNetClient()->GetNetworkID()) GameManager::getInstance()->UpdatePlayerTank(go->networkId, go->position, go->rotation, speed);
					else GameManager::getInstance()->UpdatePlayerTankWithLatency(go->networkId, go->position, go->rotation, speed, go->lateFrames);
					if ((speed.x != 0 || speed.y != 0) && (go->speed.x == 0 && go->speed.y == 0)) go->syncWaitTime = 0;
				}
			}
		}
		else if (action == ReplicationAction::ReduceLife)
		{

		}
		else if (action == ReplicationAction::Destroy)
		{
			/*if (networkId == clientNetworkId) GameManager::getInstance()->GetModNetClient()->disconnect();
			else */
			GameManager::getInstance()->DeletePlayerTank(networkId);
			//GameManager::getInstance()->ReduceLife(networkId);
			GameObject* go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject(networkId, true);
			if (go != NULL)
			{
				if (go->isAI) GameManager::getInstance()->addGrade(1);
			}
		}
		else if (action == ReplicationAction::Server_Snapshot)
		{

			bool Iswinning = true;
			int grade = 0;
			bool destroyedBricks = false;
			packet >> Iswinning;
			packet >> grade;

			//packet >> destroyedBricks;
			//if (destroyedBricks)
			//{
			//	int destroyedBrickSize = 0;
			//	packet >> destroyedBrickSize;
			//	for (int i = 0; i < destroyedBrickSize; i++)
			//	{
			//		int cloneID = 0;
			//		packet >> cloneID;
			//		//destroyedBrickArray->push_back(cloneID);
			//		StaticSpriteArray::getInstance()->removeStaticSpriteWithID(cloneID);
			//	}
			//}
			//GameManager::getInstance()->SetWinning(Iswinning);

			bool wallcheck = true;
			vector<StaticSprite*> wallList = StaticSpriteArray::getInstance()->getArray();
			for (int i = 0; i < wallList.size(); i++)
			{
				packet >> wallcheck;
				wallList.at(i)->SetEnabled(wallcheck);
				wallList.at(i)->SetShow(wallcheck);
			}
			//GameManager::getInstance()->setGrade(1, grade);
		}
		else if (action == ReplicationAction::Create_Award)
		{
			int awdType = -1;
			D3DXVECTOR3 awdPos = D3DXVECTOR3(0, 0, 0);
			packet >> awdPos.x;
			packet >> awdPos.y;
			packet >> awdType;
			if (awdType != -1 && (awdPos.x + awdPos.y != 0))
				GameManager::getInstance()->setAward(awdType, awdPos);
		}
		else if (action == ReplicationAction::ShootEvent)
		{
			GameObject* go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject(networkId);
			D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), rotation = D3DXVECTOR3(0, 0, 0), speed = D3DXVECTOR3(0, 0, 0);
			float angle;
			bool isShooted;
			packet >> tickCount;
			packet >> position.x;
			packet >> position.y;
			packet >> speed.x;
			packet >> speed.y;
			//packet >> angle;
			if (go != nullptr)
			{
				go->tickCount = tickCount;
				go->lateFrames = (int)((GetTickCount() - tickCount) / 16.67f - (REPLICATION_INTERVAL_SECONDS / 0.16f));
				go->position = position;
				go->rotation = D3DXVECTOR3(speed.x, speed.y, 0);
				go->speed = speed;
				//if (networkId == GameManager::getInstance()->GetModNetClient()->GetNetworkID()) GameManager::getInstance()->UpdatePlayerTank(go->networkId, go->position, go->rotation, speed);
				//else GameManager::getInstance()->UpdatePlayerTankWithLatency(go->networkId, go->position, go->rotation, speed, go->lateFrames);
				if ((speed.x != 0 || speed.y != 0) && (go->speed.x == 0 && go->speed.y == 0)) go->syncWaitTime = 0;
				if (networkId != GameManager::getInstance()->GetModNetClient()->GetNetworkID())
				{
					GameManager::getInstance()->CreateAndUpdatePlayerBulletWithLatency(go->networkId, CollisionCheckMethod::OneExceptAll, go->lateFrames);
				}
			}
		}
	}
}
