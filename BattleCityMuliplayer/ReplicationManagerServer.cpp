#include "Networks.h"
#include "GameManager.h"
#include "StaticSprite.h"
#include "Map.h"
#include "ReplicationManagerServer.h"

void ReplicationManagerServer::create(uint32 networkId)
{
	commands[networkId] = ReplicationAction::Create;
}

void ReplicationManagerServer::update(uint32 networkId, ReplicationAction updateType)
{
	if (commands.find(networkId) == commands.end() || commands[networkId] == ReplicationAction::Update_Position)
	{
		commands[networkId] = updateType;
	}
}

void ReplicationManagerServer::destroy(uint32 networkId)
{
	commands[networkId] = ReplicationAction::Destroy;
}

void ReplicationManagerServer::server_snapshot(uint32 networkId)
{
	commands[networkId] = ReplicationAction::Server_Snapshot;
}

void ReplicationManagerServer::CreateAward(uint32 networkId)
{
	commands[networkId] = ReplicationAction::Create_Award;
}

void ReplicationManagerServer::ReduceLife(uint32 networkId)
{
	commands[networkId] = ReplicationAction::ReduceLife;
}

void ReplicationManagerServer::ShootEvent(uint32 networkId)
{
	commands[networkId] = ReplicationAction::ShootEvent;
}

std::map<uint32, ReplicationAction> ReplicationManagerServer::GetCommands()
{
	return commands;
}

void ReplicationManagerServer::InsertCommands(std::pair<uint32, ReplicationAction> command)
{
	commands.insert(command);
}

bool ReplicationManagerServer::write(OutputMemoryStream& packet)
{
	if (commands.size() == 0) return false;

	for (std::map<uint32, ReplicationAction>::iterator it_c = commands.begin(); it_c != commands.end(); ++it_c)
	{
		packet << (*it_c).first;
		packet << (*it_c).second;
		if ((*it_c).second == ReplicationAction::Create)
		{
			GameObject* go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject((*it_c).first);
			go->position = GameManager::getInstance()->GetPlayerTankPosition((int)(*it_c).first);
			go->rotation = GameManager::getInstance()->GetPlayerTankRotation((int)(*it_c).first);
			packet << GameManager::getInstance()->GetPlayerTankID((int)(*it_c).first);
			packet << GameManager::getInstance()->GetPlayerTankLevel((int)(*it_c).first);
			packet << go->position.x;
			packet << go->position.y;
			/*packet << go->rotation.x;
			packet << go->rotation.y;*/
			packet << go->name;
			packet << go->isAI;
		}
		else if ((*it_c).second == ReplicationAction::Update_Position)
		{
			GameObject* go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject((*it_c).first);
			if (go != NULL)
			{
				if (go->networkId != (*it_c).first || go->isAI) go->position = GameManager::getInstance()->GetPlayerTankPosition((int)(*it_c).first);
				go->rotation = GameManager::getInstance()->GetPlayerTankRotation((int)(*it_c).first);
				go->speed = GameManager::getInstance()->GetPlayerTankSpeed((int)(*it_c).first);
				packet << go->tickCount;
				packet << go->position.x;
				packet << go->position.y;
				packet << go->speed.x;
				packet << go->speed.y;
				packet << go->isShooted;
				//packet << go->angle;
			}
			else
			{
				packet << 0;
				packet << 0;
				packet << 0;
				packet << 0;
				packet << 0;
				packet << false;
				//packet << go->angle;
			}
		}
		else if ((*it_c).second == ReplicationAction::Server_Snapshot)
		{
			packet << GameManager::getInstance()->IsWinning();
			packet << GameManager::getInstance()->getGrade(1);

			/*vector<int>* destroyedBrickIDs = GameManager::getInstance()->GetModNetServer()->getDestroyedBricksID();
			if (destroyedBrickIDs->size() > 0)
			{
				packet << true;
				packet << destroyedBrickIDs->size();
				if (destroyedBrickIDs->size() > 0)
					for (int i = 0; i < destroyedBrickIDs->size(); i++) packet << destroyedBrickIDs->at(i);
			}
			else packet << false;*/

			vector<StaticSprite*> wallList = StaticSpriteArray::getInstance()->getArray();
			if (wallList.size() > 0) for (int i = 0; i < wallList.size(); i++) packet << wallList.at(i)->IsEnabled();
			if (GameManager::getInstance()->GetCurrentMap())
			{
				//vector<StaticSprite*>* wallList = GameManager::getInstance()->GetCurrentMap()->GetWallArray();
				//if (wallList->size() > 0) for (int i = 0; i < wallList->size(); i++) packet << wallList->at(i)->IsEnabled();
			}
		}
		else if ((*it_c).second == ReplicationAction::Create_Award)
		{
			D3DXVECTOR3 awdPos = GameManager::getInstance()->getAwardPosition();
			int awdType = GameManager::getInstance()->getAwardType();
			packet << awdPos.x;
			packet << awdPos.y;
			packet << awdType;
		}
		else if ((*it_c).second == ReplicationAction::ReduceLife)
		{

		}
		else if ((*it_c).second == ReplicationAction::ShootEvent)
		{
			GameObject* go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject((*it_c).first);
			if (go != NULL)
			{
				go->position = GameManager::getInstance()->GetPlayerTankPosition((int)(*it_c).first);
				go->rotation = GameManager::getInstance()->GetPlayerTankRotation((int)(*it_c).first);
				go->speed = GameManager::getInstance()->GetPlayerTankSpeed((int)(*it_c).first);
				packet << go->tickCount;
				packet << go->position.x;
				packet << go->position.y;
				packet << go->speed.x;
				packet << go->speed.y;
				//packet << go->angle;
			}
			else
			{
				packet << 0;
				packet << 0;
				packet << 0;
				packet << 0;
				packet << 0;
				//packet << go->angle;
			}
		}
	}

	commands.clear();
	return true;
}
