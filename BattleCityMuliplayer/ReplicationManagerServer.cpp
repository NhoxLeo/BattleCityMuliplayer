#include "Networks.h"
#include "GameManager.h"
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

std::map<uint32, ReplicationAction> ReplicationManagerServer::GetCommands()
{
	return commands;
}

void ReplicationManagerServer::InsertCommands(std::pair<uint32, ReplicationAction> command)
{
	commands.insert(command);
}

bool ReplicationManagerServer::write(OutputMemoryStream & packet)
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
			packet << go->position.x;
			packet << go->position.y;
			packet << go->size.x;
			packet << go->size.y;
			packet << go->name;
			packet << go->clientInstanceNID;
		}
		else if ((*it_c).second == ReplicationAction::Update_Position)
		{
			GameObject* go = GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject((*it_c).first);
			go->position = GameManager::getInstance()->GetPlayerTankPosition((int)(*it_c).first);
			packet << go->position.x;
			packet << go->position.y;
			packet << go->angle;
		}
	}

	commands.clear();
	return true;
}
