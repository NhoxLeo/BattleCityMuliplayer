#include "Networks.h"
#include "ModuleNetworkingServer.h"
#include "GameManager.h"



//////////////////////////////////////////////////////////////////////
// ModuleNetworkingServer public methods
//////////////////////////////////////////////////////////////////////
void ModuleNetworkingServer::setListenPort(int port)
{
	listenPort = port;
}


//////////////////////////////////////////////////////////////////////
// ModuleNetworking virtual methods
//////////////////////////////////////////////////////////////////////
void ModuleNetworkingServer::onStart()
{
	if (!createSocket()) return;

	// Reuse address
	int enable = 1;
	int res = setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));
	if (res == SOCKET_ERROR) {
		reportError("ModuleNetworkingServer::start() - setsockopt");
		disconnect();
		return;
	}

	// Create and bind to local address
	if (!bindSocketToPort(listenPort)) {
		return;
	}

	state = ServerState::Listening;
	GameManager::getInstance()->GetModGameObject()->interpolateEntities = false;

	secondsSinceLastPing = 0.0f;
}
void ModuleNetworkingServer::onGui()
{
	if (true)
	{
		if (ImGui::CollapsingHeader("ModuleNetworkingServer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Connection checking info:");
			ImGui::Text(" - Ping interval (s): %f", PING_INTERVAL_SECONDS);
			ImGui::Text(" - Disconnection timeout (s): %f", DISCONNECT_TIMEOUT_SECONDS);

			ImGui::Separator();

			ImGui::Text("Replication");
			ImGui::InputFloat("Delivery interval (s)", &replicationDeliveryIntervalSeconds, 0.01f, 0.1f);

			ImGui::Separator();

			ImGui::Text("ZombieSpawnRatio");
			ImGui::InputFloat("Initial Spawning Interval (s)", &initialZombieSpawnRatio, 0.1f, 10.0f);
			ImGui::Text("Final Spawning Interval (s): %f", guiFinalZombieSpawnRatio);
			ImGui::Checkbox("Enable Zombie Spawner", &isSpawnerEnabled);

			ImGui::Separator();

			if (state == ServerState::Listening)
			{
				int count = 0;

				for (int i = 0; i < MAX_CLIENTS; ++i)
				{
					if (clientProxies[i].name != "")
					{
						ImGui::Text("CLIENT %d", count++);
						ImGui::Text(" - address: %d.%d.%d.%d",
							clientProxies[i].address.sin_addr.S_un.S_un_b.s_b1,
							clientProxies[i].address.sin_addr.S_un.S_un_b.s_b2,
							clientProxies[i].address.sin_addr.S_un.S_un_b.s_b3,
							clientProxies[i].address.sin_addr.S_un.S_un_b.s_b4);
						ImGui::Text(" - port: %d", ntohs(clientProxies[i].address.sin_port));
						ImGui::Text(" - name: %s", clientProxies[i].name.c_str());
						ImGui::Text(" - id: %d", clientProxies[i].clientId);
						ImGui::Text(" - Last packet time: %.04f", clientProxies[i].lastPacketReceivedTime);
						ImGui::Text(" - Seconds since repl.: %.04f", clientProxies[i].secondsSinceLastReplication);

						ImGui::Separator();
					}
				}

				//ImGui::Checkbox("Render colliders", &App->modRender->mustRenderColliders);
			}
		}
	}
}
void ModuleNetworkingServer::onPacketReceived(const InputMemoryStream& packet, const sockaddr_in& fromAddress)
{
	if (state == ServerState::Listening)
	{
		// Register player
		ClientProxy* proxy = getClientProxy(fromAddress);

		// Read the packet type
		ClientMessage message;
		packet >> message;

		// Process the packet depending on its type
		if (message == ClientMessage::Hello)
		{
			bool newClient = false;

			if (proxy == nullptr)
			{
				newClient = true;

				std::string playerName;
				packet >> playerName;

				bool usedName = false;
				for (int i = 0; i < MAX_CLIENTS; ++i)
				{
					if (clientProxies[i].name == playerName)
					{
						usedName = true;
						break;
					}
				}

				if (usedName)
				{
					OutputMemoryStream unwelcomePacket;
					unwelcomePacket << ServerMessage::Unwelcome;
					std::string errorMsg = "Player Name already in use";
					unwelcomePacket << errorMsg;
					sendPacket(unwelcomePacket, fromAddress);

					WLOG("Message received: UNWELCOMED hello - from player %s", playerName.c_str());
				}
				else
				{
					proxy = createClientProxy();

					connectedProxies++;

					proxy->address.sin_family = fromAddress.sin_family;
					proxy->address.sin_addr.S_un.S_addr = fromAddress.sin_addr.S_un.S_addr;
					proxy->address.sin_port = fromAddress.sin_port;
					proxy->connected = true;
					proxy->name = playerName;
					proxy->clientId = nextClientId++;

					// Create new network object
					spawnPlayer(*proxy);

					// Send welcome to the new player
					OutputMemoryStream welcomePacket;
					welcomePacket << ServerMessage::Welcome;
					welcomePacket << proxy->clientId;
					welcomePacket << proxy->gameObject->networkId;
					sendPacket(welcomePacket, fromAddress);

					// Send all network objects to the new player
					uint16 networkGameObjectsCount;
					GameObject* networkGameObjects[MAX_NETWORK_OBJECTS];
					GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObjects(networkGameObjects, &networkGameObjectsCount);
					for (uint16 i = 0; i < networkGameObjectsCount; ++i)
					{
						GameObject* gameObject = networkGameObjects[i];

						// TODO(jesus): Notify the new client proxy's replication manager about the creation of this game object
						proxy->replicationManager.create(gameObject->networkId);
					}

					LOG("Message received: hello - from player %s", playerName.c_str());
				}
			}

			if (!newClient)
			{
				// Send welcome to the new player
				OutputMemoryStream unwelcomePacket;
				unwelcomePacket << ServerMessage::Unwelcome;
				std::string errorMsg = "Client address already connected";
				unwelcomePacket << errorMsg;
				sendPacket(unwelcomePacket, fromAddress);

				WLOG("Message received: UNWELCOMED hello - from player %s", proxy->name.c_str());
			}
		}
		else if (message == ClientMessage::Input)
		{
			// Process the input packet and update the corresponding game object
			if (proxy != nullptr)
			{
				InputPacketData inputData;
				// Read input data
				while (packet.RemainingByteCount() > 0)
				{
					packet >> inputData.sequenceNumber;
					packet >> inputData.tickCount;
					packet >> inputData.horizontalAxis;
					packet >> inputData.verticalAxis;
					packet >> inputData.buttonBits;
					packet >> inputData.mouseX;
					packet >> inputData.mouseY;
					packet >> inputData.leftButton;

					if (inputData.sequenceNumber >= proxy->nextExpectedInputSequenceNumber)
					{
						//Process Keyboard
						proxy->gamepad.horizontalAxis = inputData.horizontalAxis;
						proxy->gamepad.verticalAxis = inputData.verticalAxis;
						proxy->gamepad.tickcount = inputData.tickCount;
						unpackInputControllerButtons(inputData.buttonBits, proxy->gamepad);
						proxy->gameObject->behaviour->onInput(proxy->gamepad);
						proxy->gameObject->tickCount = inputData.tickCount;
						//proxy->mouse.buttons[0] = (ButtonState)inputData.leftButton;
						//proxy->gameObject->behaviour->onMouse(proxy->mouse);
						proxy->nextExpectedInputSequenceNumber = inputData.sequenceNumber + 1;
					}
				}
			}
		}
		else if (message == ClientMessage::Ping)
		{
			GameManager::getInstance()->GetDeliveryManager()->processAckdSequenceNumbers(packet);
		}

		if (proxy != nullptr)
		{
			proxy->lastPacketReceivedTime = Time.time;
		}
	}
}
void ModuleNetworkingServer::onUpdate()
{
	if (state == ServerState::Listening)
	{

		secondsSinceLastPing += Time.deltaTime;
		// Replication
		for (ClientProxy& clientProxy : clientProxies)
		{
			if (clientProxy.connected)
			{
				clientProxy.secondsSinceLastReplication += Time.deltaTime;

				// TODO(jesus): If the replication interval passed and the replication manager of this proxy
				//              has pending data, write and send a replication packet to this client.
				if (clientProxy.secondsSinceLastReplication > replicationDeliveryIntervalSeconds && clientProxy.replicationManager.commands.size() > 0)
				{
					OutputMemoryStream packet;
					packet << ServerMessage::Replication;
					packet << clientProxy.nextExpectedInputSequenceNumber; //ACK of last received input

					Delivery* delivery = GameManager::getInstance()->GetDeliveryManager()->writeSequenceNumber(packet);
					delivery->deleagate = new DeliveryDelegateReplication(); //TODOAdPi
					((DeliveryDelegateReplication*)delivery->deleagate)->replicationCommands = clientProxy.replicationManager.GetCommands();
					((DeliveryDelegateReplication*)delivery->deleagate)->repManager = clientProxy.replicationManager;

					clientProxy.secondsSinceLastReplication = 0.0f;
					if (clientProxy.replicationManager.write(packet))
					{
						sendPacket(packet, clientProxy.address);
					}
				}

				//Send ping to clients
				if (secondsSinceLastPing > PING_INTERVAL_SECONDS)
				{
					OutputMemoryStream ping;
					ping << ServerMessage::Ping;

					sendPacket(ping, clientProxy.address);
				}

				//Disconnect client if waited too long
				if (Time.time - clientProxy.lastPacketReceivedTime > DISCONNECT_TIMEOUT_SECONDS)
				{
					onConnectionReset(clientProxy.address);
				}
			}
		}

		//Reset ping timer
		if (secondsSinceLastPing > PING_INTERVAL_SECONDS)
		{
			secondsSinceLastPing = 0.0f;
		}

		//Check for TimeOutPackets DeliveryManager
		GameManager::getInstance()->GetDeliveryManager()->processTimedOutPackets();

		//Update Tank Players on Server
		GameManager::getInstance()->BulletVisitAll();
		//Update Queue of Objects of Previous Frames
		GameManager::getInstance()->AddThisFrameObjects();

		//Server Reset Game Objects when there are no proxies connected
		uint16 networkGameObjectsCount;
		GameObject* networkGameObjects[MAX_NETWORK_OBJECTS];
		GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObjects(networkGameObjects, &networkGameObjectsCount);
		if (connectedProxies == 0 && networkGameObjectsCount > 0)
		{
			for (uint16 i = 0; i < networkGameObjectsCount; ++i)
			{
				GameObject* gameObject = networkGameObjects[i];

				// Unregister the network identity
				GameManager::getInstance()->GetModLinkingContext()->unregisterNetworkGameObject(gameObject);

				// Remove its associated game object
				Destroy(gameObject);

			}
		}
	}
}
void ModuleNetworkingServer::onConnectionReset(const sockaddr_in& fromAddress)
{
	// Find the client proxy
	ClientProxy* proxy = getClientProxy(fromAddress);

	if (proxy)
	{
		// Notify game object deletion to replication managers
		for (int i = 0; i < MAX_CLIENTS; ++i)
		{
			if (clientProxies[i].connected && proxy->clientId != clientProxies[i].clientId)
			{
				// TODO(jesus): Notify this proxy's replication manager about the destruction of this player's game object
				clientProxies[i].replicationManager.destroy(proxy->gameObject->networkId);
			}
		}

		// Unregister the network identity
		GameManager::getInstance()->DeletePlayerTank(proxy->gameObject->networkId);
		GameManager::getInstance()->GetModLinkingContext()->unregisterNetworkGameObject(proxy->gameObject);

		// Remove its associated game object
		Destroy(proxy->gameObject);

		// Clear the client proxy
		destroyClientProxy(proxy);

		connectedProxies--;
	}
}
void ModuleNetworkingServer::onDisconnect()
{
	// Destroy network game objects
	uint16 netGameObjectsCount;
	GameObject* netGameObjects[MAX_NETWORK_OBJECTS];
	GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObjects(netGameObjects, &netGameObjectsCount);
	for (uint32 i = 0; i < netGameObjectsCount; ++i) NetworkDestroy(netGameObjects[i]);
	// Clear all client proxies
	for (ClientProxy& clientProxy : clientProxies) destroyClientProxy(&clientProxy);
	nextClientId = 0;
	state = ServerState::Stopped;
}


//////////////////////////////////////////////////////////////////////
// Client proxies
//////////////////////////////////////////////////////////////////////
ModuleNetworkingServer::ClientProxy* ModuleNetworkingServer::getClientProxy(const sockaddr_in& clientAddress)
{
	// Try to find the client
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clientProxies[i].address.sin_addr.S_un.S_addr == clientAddress.sin_addr.S_un.S_addr &&
			clientProxies[i].address.sin_port == clientAddress.sin_port)
		{
			return &clientProxies[i];
		}
	}

	return nullptr;
}
ModuleNetworkingServer::ClientProxy* ModuleNetworkingServer::createClientProxy()
{
	// If it does not exist, pick an empty entry
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (!clientProxies[i].connected)
		{
			return &clientProxies[i];
		}
	}

	return nullptr;
}
void ModuleNetworkingServer::destroyClientProxy(ClientProxy* proxy)
{
	*proxy = {};
}


//////////////////////////////////////////////////////////////////////
// Spawning
//////////////////////////////////////////////////////////////////////
GameObject* ModuleNetworkingServer::spawnPlayer(ClientProxy& clientProxy)
{
	// Create a new game object with the player properties
	clientProxy.gameObject = Instantiate();
	clientProxy.gameObject->position = D3DXVECTOR3(272, 444, 0);
	//clientProxy.gameObject->size = { 43, 49 };
	clientProxy.gameObject->angle = 45.0f;
	clientProxy.gameObject->order = 3;
	clientProxy.gameObject->name = clientProxy.name;

	//clientProxy.gameObject->texture = App->modResources->robot;
	//clientProxy.gameObject->color.a = 0.75f;
	//// Create collider
	//clientProxy.gameObject->collider = App->modCollision->addCollider(ColliderType::Player, clientProxy.gameObject);
	//clientProxy.gameObject->collider->isTrigger = true;

	// Create behaviour
	clientProxy.gameObject->behaviour = new Player;
	clientProxy.gameObject->behaviour->gameObject = clientProxy.gameObject;

	// Assign a new network identity to the object
	GameManager::getInstance()->GetModLinkingContext()->registerNetworkGameObject(clientProxy.gameObject);
	GameManager::getInstance()->CreatePlayerTank(clientProxy.gameObject->networkId, clientProxy.gameObject->position);

	// Notify all client proxies' replication manager to create the object remotely
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clientProxies[i].connected)
		{
			// TODO(jesus): Notify this proxy's replication manager about the creation of this game object
			clientProxies[i].replicationManager.create(clientProxy.gameObject->networkId);
		}
	}

	return clientProxy.gameObject;
}
//GameObject* ModuleNetworkingServer::spawnBullet(GameObject* parent, vec2 offset)
//{
//	// Create a new game object with the player properties
//	GameObject* gameObject = Instantiate();
//	gameObject->size = { 8, 14 };
//	gameObject->angle = parent->angle;
//	gameObject->order = 4;
//	vec2 forward = vec2FromDegrees(parent->angle);
//	vec2 right = { -forward.y, forward.x };
//	gameObject->position = parent->position + offset.x * right + offset.y * forward;
//	gameObject->texture = App->modResources->bullet;
//	gameObject->collider = App->modCollision->addCollider(ColliderType::Bullet, gameObject);
//
//	// Create behaviour
//	gameObject->behaviour = new Bullet;
//	gameObject->behaviour->gameObject = gameObject;
//
//	// Assign a new network identity to the object
//	App->modLinkingContext->registerNetworkGameObject(gameObject);
//
//	// Notify all client proxies' replication manager to create the object remotely
//	for (int i = 0; i < MAX_CLIENTS; ++i)
//	{
//		if (clientProxies[i].connected)
//		{
//			// TODO(jesus): Notify this proxy's replication manager about the creation of this game object
//			clientProxies[i].replicationManager.create(gameObject->networkId);
//		}
//	}
//
//	return gameObject;
//}
void ModuleNetworkingServer::ZombieSpawner()
{
	static float finalSpawnRatio = initialZombieSpawnRatio;
	if (connectedProxies > 0 && isSpawnerEnabled)
	{
		float safetyRadius = 175.0f; //Area from the center where zombies cannot spawn
		float maxDistance = 850.0f; //Max distance where the zombies can spawn
		float increasingSpawnRatio = 0.01f;
		float fixedTimeincreaseSpawnRatio = 0.1f; //Time to increasing spawn ratio
		float maxFinalSpawnRatio = 1.0 / (connectedProxies / 1.5f);

		timeSinceLastZombieSpawned += Time.deltaTime;
		timeSinceLastIncreaseSpawnRatio += Time.deltaTime;

		//Increase spawn rate each...
		if (timeSinceLastIncreaseSpawnRatio > fixedTimeincreaseSpawnRatio && finalSpawnRatio >= maxFinalSpawnRatio)
		{
			finalSpawnRatio = finalSpawnRatio - increasingSpawnRatio;
			timeSinceLastIncreaseSpawnRatio = 0;
		}



		/*if (timeSinceLastZombieSpawned > finalSpawnRatio)
		{

			vec2 randomDirection = vec2{ RandomFloat(-1.0f,1.0f),RandomFloat(-1.0f,1.0f) };
			float distance = 1800.0f;

			spawnZombie(normalize(randomDirection) * distance);
			timeSinceLastZombieSpawned = 0.0f;
		}
		guiFinalZombieSpawnRatio = finalSpawnRatio;*/
	}
	else
	{
		guiFinalZombieSpawnRatio = initialZombieSpawnRatio;
		finalSpawnRatio = initialZombieSpawnRatio;
	}



}
float ModuleNetworkingServer::RandomFloat(float min, float max)
{
	return ((float)rand() / RAND_MAX) * (max - min) + min;
}
//GameObject* ModuleNetworkingServer::spawnZombie(vec2 position)
//{
//	GameObject* zombie = Instantiate();
//	zombie->size = { 43, 35 };
//	zombie->position = position;
//	zombie->order = 2;
//	zombie->texture = App->modResources->zombie;
//	zombie->collider = App->modCollision->addCollider(ColliderType::Zombie, zombie);
//	zombie->collider->isTrigger = true;
//
//	zombie->behaviour = new Zombie();
//	zombie->behaviour->gameObject = zombie;
//
//	App->modLinkingContext->registerNetworkGameObject(zombie);
//
//	// Notify all client proxies' replication manager to create the object remotely
//	for (int i = 0; i < MAX_CLIENTS; ++i)
//	{
//		if (clientProxies[i].connected)
//		{
//			// TODO(jesus): Notify this proxy's replication manager about the creation of this game object
//			clientProxies[i].replicationManager.create(zombie->networkId);
//		}
//	}
//
//	return zombie;
//}
GameObject* ModuleNetworkingServer::spawnExplosion(GameObject* zombie)
{
	GameObject* object = Instantiate();
	//object->size = { 60, 60 };
	object->position = zombie->position;
	object->order = 6;
	//object->animation = App->modAnimations->useAnimation("explosion");

	//Explosion* script = new Explosion();
	//script->gameObject = object;
	//script->zombie = zombie;
	//object->behaviour = script;

	//App->modLinkingContext->registerNetworkGameObject(object);

	//// Notify all client proxies' replication manager to create the object remotely
	//for (int i = 0; i < MAX_CLIENTS; ++i)
	//{
	//	if (clientProxies[i].connected)
	//	{
	//		// TODO(jesus): Notify this proxy's replication manager about the creation of this game object
	//		clientProxies[i].replicationManager.create(object->networkId);
	//	}
	//}

	return object;
}
//GameObject* ModuleNetworkingServer::spawnBlood(vec2 position, float angle)
//{
//	GameObject* object = Instantiate();
//	object->size = { 50, 50 };
//	object->position = position;
//	object->angle = angle;
//	object->texture = App->modResources->blood;
//	object->order = 0;
//
//	object->behaviour = new Blood();
//	object->behaviour->gameObject = object;
//
//	App->modLinkingContext->registerNetworkGameObject(object);
//
//	// Notify all client proxies' replication manager to create the object remotely
//	for (int i = 0; i < MAX_CLIENTS; ++i)
//	{
//		if (clientProxies[i].connected)
//		{
//			// TODO(jesus): Notify this proxy's replication manager about the creation of this game object
//			clientProxies[i].replicationManager.create(object->networkId);
//		}
//	}
//
//	return object;
//}
//GameObject* ModuleNetworkingServer::spawnRezUI(vec2 position)
//{
//	GameObject* object = Instantiate();
//	object->size = { 66, 85 };
//	object->position = position;
//	object->animation = App->modAnimations->useAnimation("rez");
//	object->order = 5;
//
//	App->modLinkingContext->registerNetworkGameObject(object);
//
//	// Notify all client proxies' replication manager to create the object remotely
//	for (int i = 0; i < MAX_CLIENTS; ++i)
//	{
//		if (clientProxies[i].connected)
//		{
//			// TODO(jesus): Notify this proxy's replication manager about the creation of this game object
//			clientProxies[i].replicationManager.create(object->networkId);
//		}
//	}
//
//	return object;
//}
std::vector<GameObject*> ModuleNetworkingServer::getAllClientPlayers()
{
	std::vector<GameObject*> players;

	for (auto& client : clientProxies)
	{
		if (client.connected)
			players.push_back(client.gameObject);
	}

	return players;
}


//////////////////////////////////////////////////////////////////////
// Update / destruction
//////////////////////////////////////////////////////////////////////
void ModuleNetworkingServer::destroyNetworkObject(GameObject* gameObject)
{
	// Notify all client proxies' replication manager to destroy the object remotely
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clientProxies[i].connected)
		{
			// TODO(jesus): Notify this proxy's replication manager about the destruction of this game object
			clientProxies[i].replicationManager.destroy(gameObject->networkId);
		}
	}

	// Assuming the message was received, unregister the network identity
	GameManager::getInstance()->DeletePlayerTank(gameObject->networkId);
	GameManager::getInstance()->GetModLinkingContext()->unregisterNetworkGameObject(gameObject);

	// Finally, destroy the object from the server
	Destroy(gameObject);
}
void ModuleNetworkingServer::updateNetworkObject(GameObject* gameObject, ReplicationAction updateType)
{
	// Notify all client proxies' replication manager to destroy the object remotely
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clientProxies[i].connected)
		{
			// TODO(jesus): Notify this proxy's replication manager about the update of this game object
			clientProxies[i].replicationManager.update(gameObject->networkId, updateType);
		}
	}
}
//////////////////////////////////////////////////////////////////////
// Global update / destruction of game objects
//////////////////////////////////////////////////////////////////////
void NetworkUpdate(GameObject* gameObject, ReplicationAction updateType)
{
	ASSERT(GameManager::getInstance()->GetModNetServer()->isConnected());
	GameManager::getInstance()->GetModNetServer()->updateNetworkObject(gameObject, updateType);
}
void NetworkDestroy(GameObject* gameObject)
{
	ASSERT(GameManager::getInstance()->GetModNetServer()->isConnected());
	GameManager::getInstance()->GetModNetServer()->destroyNetworkObject(gameObject);
}
std::vector<GameObject*> getPlayers()
{
	return GameManager::getInstance()->GetModNetServer()->getAllClientPlayers();
}
