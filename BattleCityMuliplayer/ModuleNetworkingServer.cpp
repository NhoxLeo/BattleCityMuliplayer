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

	//state = ServerState::Listening;
	state = ServerState::Lobby;
	GameManager::getInstance()->GetModGameObject()->interpolateEntities = false;

	secondsSinceLastPing = 0.0f;
	serverSnapshotCounter = 0.0f;
	destroyedBricksID = new vector<int>();
	AITanksObject = new vector<GameObject*>();
	maxAITanks = 5;
	GameManager::getInstance()->GetModLinkingContext()->clear();

	for (ClientProxy& proxy : clientProxies) proxy.connected = false;
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

			ImGui::Separator();

			if (state == ServerState::Lobby || state == ServerState::Listening)
			{
				if (ImGui::Button("Start Game"))
				{
					state = ServerState::Listening;
					// Notify all client proxies' replication manager to create the object remotely
					for (int i = 0; i < MAX_CLIENTS; ++i)
					{
						if (clientProxies[i].connected)
						{
							OutputMemoryStream packet;
							packet << ServerMessage::StartGame;
							sendPacket(packet, clientProxies[i].address);

							// Send all network objects to the new player
							uint16 networkGameObjectsCount;
							GameObject* networkGameObjects[MAX_NETWORK_OBJECTS];
							GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObjects(networkGameObjects, &networkGameObjectsCount);
							// Notify the new client proxy's replication manager about the creation of other game objects
							for (uint16 k = 0; k < networkGameObjectsCount; ++k)
								clientProxies[i].replicationManager.create(networkGameObjects[k]->networkId);
							// Notify all client proxies' replication manager to create the object remotely
							for (int l = 0; l < MAX_CLIENTS; ++l)
							{
								if (clientProxies[l].connected && clientProxies[l].gameObject->networkId != clientProxies[i].gameObject->networkId)
									clientProxies[l].replicationManager.create(clientProxies[i].gameObject->networkId);
							}
							D3DXVECTOR3 playerpos = D3DXVECTOR3(126, 58, 0);
							switch (clientProxies[i].clientId % 2)
							{
							case 0:
								playerpos = PlayerPosition;
								break;
							case 1:
								playerpos = PlayerPosition1;
								break;
							}
							clientProxies[i].gameObject->position = playerpos;
							GameManager::getInstance()->CreatePlayerTank(clientProxies[i].gameObject->networkId, clientProxies[i].clientId, 1, clientProxies[i].gameObject->position);
						}
					}
				}
				int count = 0;
				/*for (int i = 0; i < MAX_CLIENTS; ++i)
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
				}*/
				if (ImGui::Button("Spawn AI") && GameManager::getInstance()->GetTanksCount() > 0) AITankSpawner(false, EnemyPosition1);
				if (ImGui::Button("Create Award"))
				{
					GameManager::getInstance()->setAward();
					for (ClientProxy& clientProxy : clientProxies)
						if (clientProxy.connected) clientProxy.replicationManager.CreateAward(clientProxy.gameObject->networkId);
				}
				if (ImGui::Button("Server Snapshot"))
				{
					for (int i = 0; i < MAX_CLIENTS; ++i)
					{
						if (clientProxies[i].connected)
						{
							// TODO(jesus): Notify this proxy's replication manager about the creation of this game object
							clientProxies[i].replicationManager.server_snapshot(clientProxies[i].gameObject->networkId);
						}
					}
				}
			}
		}
	}
}
void ModuleNetworkingServer::onPacketReceived(const InputMemoryStream& packet, const sockaddr_in& fromAddress)
{
	if (state == ServerState::Lobby)
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
				bool usedName = false, firstClientconnected = false, secondClientconnected = false;
				for (int i = 0; i < MAX_CLIENTS; ++i)
				{
					/*if (clientProxies[i].name == playerName)
					{
						usedName = true;
						break;
					}*/
					if (clientProxies[i].connected == true && (int)clientProxies[i].clientId == 0) firstClientconnected = true;
					if (clientProxies[i].connected == true && (int)clientProxies[i].clientId == 1) secondClientconnected = true;
					if (firstClientconnected && secondClientconnected) break;
				}
				if (/*usedName*/ firstClientconnected && secondClientconnected)
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
					if (!firstClientconnected) proxy->clientId = 0;
					else
					{
						if (!secondClientconnected) proxy->clientId = 1;
						else proxy->clientId = nextClientId++;
					}

					// Create a new game object with the player properties
					proxy->gameObject = Instantiate();
					proxy->gameObject->position = D3DXVECTOR3(272, 444, 0);
					//proxy->gameObject->size = { 43, 49 };
					proxy->gameObject->angle = 45.0f;
					proxy->gameObject->order = 3;
					proxy->gameObject->isAI = false;
					proxy->gameObject->name = proxy->name;
					// Create behaviour
					proxy->gameObject->behaviour = new Player;
					proxy->gameObject->behaviour->gameObject = proxy->gameObject;
					// Assign a new network identity to the object
					GameManager::getInstance()->GetModLinkingContext()->registerNetworkGameObject(proxy->gameObject);

					// Send welcome to the new player
					OutputMemoryStream welcomePacket;
					welcomePacket << ServerMessage::Welcome;
					welcomePacket << proxy->clientId;
					welcomePacket << proxy->gameObject->networkId;
					sendPacket(welcomePacket, fromAddress);

					//// Send all network objects to the new player
					//uint16 networkGameObjectsCount;
					//GameObject* networkGameObjects[MAX_NETWORK_OBJECTS];
					//GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObjects(networkGameObjects, &networkGameObjectsCount);
					//// Notify the new client proxy's replication manager about the creation of other game objects
					//for (uint16 i = 0; i < networkGameObjectsCount; ++i)  proxy->replicationManager.create(networkGameObjects[i]->networkId);
					//// Notify all client proxies' replication manager to create the object remotely
					//for (int i = 0; i < MAX_CLIENTS; ++i)
					//	if (clientProxies[i].connected && clientProxies[i].gameObject->networkId != proxy->gameObject->networkId) clientProxies[i].replicationManager.create(proxy->gameObject->networkId);
					//GameManager::getInstance()->CreatePlayerTank(proxy->gameObject->networkId, proxy->clientId, proxy->gameObject->position);
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
		else if (message == ClientMessage::Ping) GameManager::getInstance()->GetDeliveryManager()->processAckdSequenceNumbers(packet);
		if (connectedProxies > 0)
		{
			OutputMemoryStream clientSize;
			clientSize << ServerMessage::ClientSize;
			clientSize << connectedProxies;
			sendPacket(clientSize, fromAddress);
		}
		if (proxy != nullptr) proxy->lastPacketReceivedTime = Time.time;
	}
	else if (state == ServerState::Listening)
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
				bool usedName = false, firstClientconnected = false, secondClientconnected = false;
				for (int i = 0; i < MAX_CLIENTS; ++i)
				{
					/*if (clientProxies[i].name == playerName)
					{
						usedName = true;
						break;
					}*/
					if (clientProxies[i].connected == true && (int)clientProxies[i].clientId == 0)						firstClientconnected = true;
					if (clientProxies[i].connected == true && (int)clientProxies[i].clientId == 1)						secondClientconnected = true;
					if (firstClientconnected && secondClientconnected) break;
				}
				if (/*usedName*/ firstClientconnected && secondClientconnected)
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
					if (!firstClientconnected) proxy->clientId = 0;
					else
					{
						if (!secondClientconnected) proxy->clientId = 1;
						else proxy->clientId = nextClientId++;
					}

					// Create a new game object with the player properties
					proxy->gameObject = Instantiate();
					proxy->gameObject->position = D3DXVECTOR3(272, 444, 0);
					//proxy->gameObject->size = { 43, 49 };
					proxy->gameObject->angle = 45.0f;
					proxy->gameObject->order = 3;
					proxy->gameObject->isAI = false;
					proxy->gameObject->name = proxy->name;
					// Create behaviour
					proxy->gameObject->behaviour = new Player;
					proxy->gameObject->behaviour->gameObject = proxy->gameObject;
					// Assign a new network identity to the object
					GameManager::getInstance()->GetModLinkingContext()->registerNetworkGameObject(proxy->gameObject);

					// Send welcome to the new player
					OutputMemoryStream welcomePacket;
					welcomePacket << ServerMessage::Welcome;
					welcomePacket << proxy->clientId;
					welcomePacket << proxy->gameObject->networkId;
					sendPacket(welcomePacket, fromAddress);

					//// Send all network objects to the new player
					//uint16 networkGameObjectsCount;
					//GameObject* networkGameObjects[MAX_NETWORK_OBJECTS];
					//GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObjects(networkGameObjects, &networkGameObjectsCount);
					//// Notify the new client proxy's replication manager about the creation of other game objects
					//for (uint16 i = 0; i < networkGameObjectsCount; ++i)  proxy->replicationManager.create(networkGameObjects[i]->networkId);
					//// Notify all client proxies' replication manager to create the object remotely
					//for (int i = 0; i < MAX_CLIENTS; ++i)
					//	if (clientProxies[i].connected && clientProxies[i].gameObject->networkId != proxy->gameObject->networkId) clientProxies[i].replicationManager.create(proxy->gameObject->networkId);
					//GameManager::getInstance()->CreatePlayerTank(proxy->gameObject->networkId, proxy->clientId, proxy->gameObject->position);
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
		else if (message == ClientMessage::Ping) GameManager::getInstance()->GetDeliveryManager()->processAckdSequenceNumbers(packet);
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
					packet >> inputData.shoot;
					//packet >> inputData.buttonBits;
					if (inputData.sequenceNumber >= proxy->nextExpectedInputSequenceNumber)
					{
						proxy->gamepad.horizontalAxis = inputData.horizontalAxis;
						proxy->gamepad.verticalAxis = inputData.verticalAxis;
						proxy->gamepad.tickcount = inputData.tickCount;
						proxy->gamepad.shoot = inputData.shoot;
						//unpackInputControllerButtons(inputData.buttonBits, proxy->gamepad);
						proxy->gameObject->behaviour->onInput(proxy->gamepad);
						proxy->gameObject->tickCount = inputData.tickCount;
						proxy->nextExpectedInputSequenceNumber = inputData.sequenceNumber + 1;
					}
				}

				if (GameManager::getInstance()->IsWinning())
				{
					for (int k = 0; k < AITanksObject->size(); k++)
					{
						AITanksObject->at(k)->tickCount = GetTickCount();
						//for (int i = 0; i < MAX_CLIENTS; ++i)
						//{
						//	if (clientProxies[i].connected)
						//	{
						//		// TODO(jesus): Notify this proxy's replication manager about the creation of this game object
						//		clientProxies[i].replicationManager.update(AITanksObject->at(k)->networkId, ReplicationAction::Update_Position);
						//	}
						//}
						proxy->replicationManager.update(AITanksObject->at(k)->networkId, ReplicationAction::Update_Position);
					}
				}
			}
		}
		if (proxy != nullptr) proxy->lastPacketReceivedTime = Time.time;
	}
}
void ModuleNetworkingServer::onUpdate()
{
	if (state == ServerState::Lobby)
	{
		secondsSinceLastPing += Time.deltaTime;
		// Replication
		for (ClientProxy& clientProxy : clientProxies)
			//for (int i = 0; i < MAX_CLIENTS; ++i)
		{
			//ClientProxy clientProxy = clientProxies[i];
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
					onConnectionReset(clientProxy.address);
			}
		}
		//Reset ping timer
		if (secondsSinceLastPing > PING_INTERVAL_SECONDS) secondsSinceLastPing = 0.0f;
		if (secondsSinceLastServerSnapshot > SERVER_SNAPSHOT_SECONDS) secondsSinceLastServerSnapshot = 0.0f;
		//Check for TimeOutPackets DeliveryManager
		GameManager::getInstance()->GetDeliveryManager()->processTimedOutPackets();
	}
	else if (state == ServerState::Listening)
	{
		serverSnapshotCounter += Time.deltaTime;
		secondsSinceLastPing += Time.deltaTime;
		secondsSinceLastServerSnapshot += Time.deltaTime;
		AITankSpawnerCounter += Time.deltaTime;
		// Replication
		for (ClientProxy& clientProxy : clientProxies)
			//for (int i = 0; i < MAX_CLIENTS; ++i)
		{
			//ClientProxy clientProxy = clientProxies[i];
			if (clientProxy.connected)
			{
				clientProxy.secondsSinceLastReplication += Time.deltaTime;
				// TODO(jesus): If the replication interval passed and the replication manager of this proxy
				//              has pending data, write and send a replication packet to this client.
				if (!GameManager::getInstance()->IsWinning())
				{
					OutputMemoryStream packet;
					packet << ServerMessage::StopGame;
					sendPacket(packet, clientProxy.address);
					state = ServerState::Lobby;
				}
				else
				{
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
				}
				//Send ping to clients
				if (secondsSinceLastPing > PING_INTERVAL_SECONDS)
				{
					OutputMemoryStream ping;
					ping << ServerMessage::Ping;
					sendPacket(ping, clientProxy.address);
				}
				/*if (secondsSinceLastServerSnapshot > SERVER_SNAPSHOT_SECONDS)
				{
					OutputMemoryStream snapshot;
					snapshot << ServerMessage::Snapshot;
					vector<bool>* wallList = GameManager::getInstance()->GetWallEnabledArray();
					if (wallList->size() > 0) for (int i = 0; i < wallList->size(); i++) snapshot << wallList->at(i);
					sendPacket(snapshot, clientProxy.address);
				}*/

				//Disconnect client if waited too long
				if (Time.time - clientProxy.lastPacketReceivedTime > DISCONNECT_TIMEOUT_SECONDS)
					onConnectionReset(clientProxy.address);
			}
		}
		//Reset ping timer
		if (secondsSinceLastPing > PING_INTERVAL_SECONDS) secondsSinceLastPing = 0.0f;
		if (secondsSinceLastServerSnapshot > SERVER_SNAPSHOT_SECONDS) secondsSinceLastServerSnapshot = 0.0f;
		//Check for TimeOutPackets DeliveryManager
		GameManager::getInstance()->GetDeliveryManager()->processTimedOutPackets();

		if (serverSnapshotCounter > 1 && GameManager::getInstance()->GetTanksCount() > 0)
		{
			for (int i = 0; i < MAX_CLIENTS; ++i)
			{
				if (clientProxies[i].connected)
				{
					//clientProxies[i].replicationManager.server_snapshot(clientProxies[i].gameObject->networkId);
					vector<bool>* vec = GameManager::getInstance()->GetWallEnabledArray();
					int arraysize = vec->size();
					OutputMemoryStream serverSnapshot;
					serverSnapshot << ServerMessage::Snapshot;
					serverSnapshot << arraysize;
					bool check;
					for (int i = 0; i < vec->size(); i++)
					{
						check = vec->at(i);
						serverSnapshot << check;
					}
					sendPacket(serverSnapshot, clientProxies[i].address);
				}
			}
			serverSnapshotCounter = 0;
		}
		if (GameManager::getInstance()->IsWinning())
		{
			//Update Tank Players on Server
			GameManager::getInstance()->AITankControl();
			GameManager::getInstance()->AllAIPlayerTankVisitAll();
			GameManager::getInstance()->BulletVisitAll();
			GameManager::getInstance()->UpdateAllTanks();
			if (AITankSpawnerCounter > 5 && connectedProxies > 0 && maxAITanks > 0 && AITanksObject->size() == 0)
			{
				AITankSpawner(true, EnemyPosition1);
				maxAITanks -= 1;
			}
			if (AITankSpawnerCounter > 6) secondsSinceLastServerSnapshot = 0.0f;
			if (maxAITanks == 0 && AITanksObject->size() == 0) GameManager::getInstance()->SetWinning(false);
		}
		GameManager::getInstance()->AddThisFrameObjects();

		//Server Reset Game Objects when there are no proxies connected
		uint16 networkGameObjectsCount;
		GameObject* networkGameObjects[MAX_NETWORK_OBJECTS];
		GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObjects(networkGameObjects, &networkGameObjectsCount);
		if (connectedProxies == 0 && networkGameObjectsCount > 0 && AITanksObject->size() == 0)
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
	clientProxy.gameObject->isAI = false;
	clientProxy.gameObject->name = clientProxy.name;
	// Create behaviour
	clientProxy.gameObject->behaviour = new Player;
	clientProxy.gameObject->behaviour->gameObject = clientProxy.gameObject;

	// Assign a new network identity to the object
	GameManager::getInstance()->GetModLinkingContext()->registerNetworkGameObject(clientProxy.gameObject);
	GameManager::getInstance()->CreatePlayerTank(clientProxy.gameObject->networkId, clientProxy.clientId, 1, clientProxy.gameObject->position);

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
void ModuleNetworkingServer::AITankSpawner(bool _randomPos, D3DXVECTOR3 _pos)
{
	D3DXVECTOR3 enemypos = _pos;
	if (_randomPos)
	{
		switch (rand() % 5)
		{
		case 0:
			enemypos = EnemyPosition1;
			break;
		case 1:
			enemypos = EnemyPosition2;
			break;
		case 2:
			enemypos = EnemyPosition3;
			break;
		case 3:
			enemypos = EnemyPosition4;
			break;
		case 4:
			enemypos = EnemyPosition5;
			break;
		}
	}
	GameObject* aiTank = Instantiate();
	aiTank->position = enemypos;
	aiTank->order = 3;
	aiTank->isAI = true;
	aiTank->behaviour = new Player();
	aiTank->behaviour->gameObject = aiTank;

	GameManager::getInstance()->GetModLinkingContext()->registerNetworkGameObject(aiTank);
	GameManager::getInstance()->CreateAIPlayerTank(aiTank->networkId, rand() % 3 + 1, aiTank->position);

	aiTank->name = (int)aiTank->networkId;

	AITanksObject->push_back(GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObject(aiTank->networkId));

	// Notify all client proxies' replication manager to create the object remotely
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clientProxies[i].connected)
		{
			// TODO(jesus): Notify this proxy's replication manager about the creation of this game object
			clientProxies[i].replicationManager.create(aiTank->networkId);
		}
	}
}
float ModuleNetworkingServer::RandomFloat(float min, float max)
{
	return ((float)rand() / RAND_MAX) * (max - min) + min;
}
void ModuleNetworkingServer::CreateAwardEvent()
{
	GameManager::getInstance()->setAward();
	for (ClientProxy& clientProxy : clientProxies)
		if (clientProxy.connected) clientProxy.replicationManager.CreateAward(clientProxy.gameObject->networkId);
}
void ModuleNetworkingServer::StartGameServerSide()
{
	state = ServerState::Listening;
	// Notify all client proxies' replication manager to create the object remotely
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clientProxies[i].connected)
		{
			OutputMemoryStream packet;
			packet << ServerMessage::StartGame;
			sendPacket(packet, clientProxies[i].address);

			// Send all network objects to the new player
			uint16 networkGameObjectsCount;
			GameObject* networkGameObjects[MAX_NETWORK_OBJECTS];
			GameManager::getInstance()->GetModLinkingContext()->getNetworkGameObjects(networkGameObjects, &networkGameObjectsCount);
			// Notify the new client proxy's replication manager about the creation of other game objects
			for (uint16 k = 0; k < networkGameObjectsCount; ++k)
				clientProxies[i].replicationManager.create(networkGameObjects[k]->networkId);
			// Notify all client proxies' replication manager to create the object remotely
			for (int l = 0; l < MAX_CLIENTS; ++l)
			{
				if (clientProxies[l].connected && clientProxies[l].gameObject->networkId != clientProxies[i].gameObject->networkId)
					clientProxies[l].replicationManager.create(clientProxies[i].gameObject->networkId);
			}
			D3DXVECTOR3 playerpos = D3DXVECTOR3(126, 58, 0);
			switch (clientProxies[i].clientId % 2)
			{
			case 0:
				playerpos = PlayerPosition;
				break;
			case 1:
				playerpos = PlayerPosition1;
				break;
			}
			clientProxies[i].gameObject->position = playerpos;
			GameManager::getInstance()->CreatePlayerTank(clientProxies[i].gameObject->networkId, clientProxies[i].clientId, 1, clientProxies[i].gameObject->position);
		}
	}
}
int ModuleNetworkingServer::ClientSize()
{
	int num = 0;
	for (ClientProxy& proxy : clientProxies)
	{
		if (proxy.connected)
			num += 1;
	}
	return num;
}
void ModuleNetworkingServer::SwitchScene(int _sceneID)
{
	for (ClientProxy& proxy : clientProxies)
	{
		if (proxy.connected)
		{
			OutputMemoryStream welcomePacket;
			welcomePacket << ServerMessage::SwitchScene;
			welcomePacket << _sceneID;
			sendPacket(welcomePacket, proxy.address);
		}
	}
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
