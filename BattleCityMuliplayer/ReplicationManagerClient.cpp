#include "Networks.h"
#include "GameManager.h"
#include "ReplicationManagerClient.h"

void ReplicationManagerClient::read(const InputMemoryStream & packet, uint32 clientNetworkId)
{
	while (packet.RemainingByteCount() > 0)
	{
		uint32 networkId;
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
			packet >> go->clientInstanceNID;
			if (networkId == clientNetworkId)
			{
				go->isPlayer = true;
				go->doInterpolation = false;
			}
			if (go->clientInstanceNID != 0) //It will be an instanced object on the client
			{
				////Currently only for bullets
				//go->behaviour = new Bullet();
				//go->behaviour->gameObject = go;
				//go->behaviour->isServer = false;
				//go->doInterpolation = false;
				//GameObject* clientPlayer = App->modLinkingContext->getNetworkGameObject(go->clientInstanceNID);
				//if (clientPlayer)
				//{
				//	vec2 bullet_offset = { 10.0f, 20.0f };
				//	vec2 forward = vec2FromDegrees(clientPlayer->angle);
				//	vec2 right = { -forward.y, forward.x };
				//	go->position = clientPlayer->position + forward * bullet_offset.y + right * bullet_offset.x;
				//}
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
			go->final_angle = go->angle;
			go->initial_angle = go->angle;

			GameManager::getInstance()->CreatePlayerTank(networkId, go->position);
		}
		else if (action == ReplicationAction::Update_Position)
		{
			GameObject* go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject(networkId);
			D3DXVECTOR3 position;
			float angle;
			packet >> position.x;
			packet >> position.y;
			packet >> angle;
			if (go != nullptr)
			{
				/*if (go->clientInstanceNID != 0)	go->doInterpolation = true;
				go->newReplicationState(position, angle);
				if (!GameManager::getInstance()->GetModGameObject()->interpolateEntities || !go->doInterpolation)
				{
					go->position = position;
					go->angle = angle;
				}*/

				go->position = position;
			}
		}
		else if (action == ReplicationAction::Destroy)
		{
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
	}
}
