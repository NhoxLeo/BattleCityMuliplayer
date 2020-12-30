#include "Networks.h"
#include "GameManager.h"
#include "StaticSprite.h"
#include "ReplicationManagerClient.h"

void ReplicationManagerClient::read(const InputMemoryStream& packet, uint32 clientNetworkId)
{
	while (packet.RemainingByteCount() > 0)
	{
		uint32 networkId, tickCount;
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
			packet >> go->position.x;
			packet >> go->position.y;
			packet >> go->size.x;
			packet >> go->size.y;
			packet >> go->name;
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
				//script->laser = GameManager::getInstance()->GetModNetClient()->spawnLaser(go);
				//script->laser->doInterpolation = false;
				go->behaviour = script;
			}
			go->final_position = go->position;
			go->initial_position = go->position;
			go->final_rotation = go->rotation;
			go->initial_rotation = go->rotation;
			//go->final_angle = go->angle;
			//go->initial_angle = go->angle;

			GameManager::getInstance()->CreatePlayerTank(networkId, go->position);
		}
		else if (action == ReplicationAction::Update_Position)
		{
			GameObject* go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject(networkId);
			D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0), rotation = D3DXVECTOR3(0, 0, 0), speed = D3DXVECTOR3(0, 0, 0);
			float angle;
			packet >> tickCount;
			packet >> position.x;
			packet >> position.y;
			packet >> rotation.x;
			packet >> rotation.y;
			packet >> speed.x;
			packet >> speed.y;
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
					go->rotation = rotation;
					go->speed = speed;
					if (networkId == GameManager::getInstance()->GetModNetClient()->GetNetworkID()) GameManager::getInstance()->UpdatePlayerTank(go->networkId, go->position, go->rotation, speed);
					else
					{
						if (speed.x + speed.y == 0)  GameManager::getInstance()->UpdatePlayerTank(go->networkId, go->position, go->rotation, speed);
						else GameManager::getInstance()->UpdatePlayerTankWithLatency(go->networkId, go->position, go->rotation, speed, go->lateFrames);
					}
					if ((speed.x != 0 || speed.y != 0) && (go->speed.x == 0 && go->speed.y == 0)) go->syncWaitTime = 0;
				}
			}
		}
		else if (action == ReplicationAction::Destroy)
		{
			if (networkId == clientNetworkId)
			{
				GameManager::getInstance()->DeleteAllPlayerTank();
				GameManager::getInstance()->GetModNetClient()->disconnect();
			}
			else
			{
				GameManager::getInstance()->DeletePlayerTank(networkId);
			}
			//GameObject* go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject(networkId);
			//if (go)
			//{
			//	//if (go->texture && go->texture->UID == App->modResources->zombie->UID) //If it is a zombie
			//	//{
			//	//	App->modNetClient->zombieDeathCount++;
			//	//}
			//	//if (go->texture && go->texture->UID == GameManager::getInstance()->getmod()->modResources->robot->UID) //If it is a player
			//	//{
			//	//	GameManager::getInstance()->GetModNetClient()->players.remove(go);
			//	//}
			//	GameManager::getInstance()->GetModLinkingContext()->unregisterNetworkGameObject(go);
			//	Destroy(go);
			//}
		}
		else if (action == ReplicationAction::Server_Snapshot)
		{
			bool destroyedBricks = false;
			packet >> destroyedBricks;
			if (destroyedBricks)
			{
				int destroyedBrickSize = 0;
				packet >> destroyedBrickSize;
				vector<int>* destroyedBrickArray = new vector<int>();
				for (int i = 0; i < destroyedBrickSize; i++)
				{
					int cloneID = 0;
					packet >> cloneID;
					//destroyedBrickArray->push_back(cloneID);
					StaticSpriteArray::getInstance()->removeStaticSpriteWithID(cloneID);
				}
			}
		}
	}
}
