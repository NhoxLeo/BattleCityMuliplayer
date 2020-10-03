#include "Networks.h"
#include "ModuleNetworkingClient.h"



//////////////////////////////////////////////////////////////////////
// ModuleNetworkingClient public methods
//////////////////////////////////////////////////////////////////////
void ModuleNetworkingClient::setServerAddress(const char* pServerAddress, uint16 pServerPort)
{
	serverAddressStr = pServerAddress;
	serverPort = pServerPort;
}
void ModuleNetworkingClient::setPlayerInfo(const char* pPlayerName)
{
	playerName = pPlayerName;
}
GameObject* ModuleNetworkingClient::spawnLaser(GameObject* player)
{
	/*GameObject* gameObject = Instantiate();
	gameObject->size = { 7, 1000 };
	gameObject->angle = player->angle;
	gameObject->order = 2;
	D3DXVECTOR3 forward = vec2FromDegrees(player->angle);
	D3DXVECTOR3 right = { -forward.y, forward.x };
	D3DXVECTOR3 offset = { 10.0f, 500.0f };
	gameObject->position = player->position + offset.x * right + offset.y * forward;
	gameObject->texture = App->modResources->laser;*/

	return NULL;
	//return gameObject;
}


//////////////////////////////////////////////////////////////////////
// ModuleNetworking virtual methods
//////////////////////////////////////////////////////////////////////
void ModuleNetworkingClient::onStart()
{
	if (!createSocket()) return;

	if (!bindSocketToPort(0)) {
		disconnect();
		return;
	}

	// Create remote address
	serverAddress = {};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPort);
	int res = inet_pton(AF_INET, serverAddressStr.c_str(), &serverAddress.sin_addr);
	if (res == SOCKET_ERROR) {
		reportError("ModuleNetworkingClient::startClient() - inet_pton");
		disconnect();
		return;
	}

	state = ClientState::Start;

	inputDataFront = 0;
	inputDataBack = 0;

	//App->modUI->isPlaying = true;
	//App->modUI->debugUI = false;

	secondsSinceLastInputDelivery = 0.0f;
	secondsSinceLastPing = 0.0f;
	lastPacketReceivedTime = Time.time;

	//App->modGameObject->interpolateEntities = true;
}
void ModuleNetworkingClient::onGui()
{
	if (state == ClientState::Stopped) return;

	if (true)
	{
		if (ImGui::CollapsingHeader("ModuleNetworkingClient", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (state == ClientState::WaitingWelcome)
			{
				ImGui::Text("Waiting for server response...");
			}
			else if (state == ClientState::Playing)
			{
				ImGui::Text("Connected to server");
				ImGui::Text(" - Replication Ping: %f", replicationPing);
				ImGui::Separator();
				ImGui::Text("Player info:");
				ImGui::Text(" - Id: %u", playerId);
				ImGui::Text(" - Name: %s", playerName.c_str());
				ImGui::Separator();
				ImGui::Text("Player info:");
				ImGui::Text(" - Network id: %u", networkId);
				D3DXVECTOR3 playerPosition = {};
				/*GameObject* playerGameObject = App->modLinkingContext->getNetworkGameObject(networkId);
				if (playerGameObject != nullptr) {
					playerPosition = playerGameObject->position;
				}*/
				ImGui::Text(" - Coordinates: (%f, %f)", playerPosition.x, playerPosition.y);
				ImGui::Separator();
				ImGui::Text("Connection checking info:");
				ImGui::Text(" - Ping interval (s): %f", PING_INTERVAL_SECONDS);
				ImGui::Text(" - Disconnection timeout (s): %f", DISCONNECT_TIMEOUT_SECONDS);
				ImGui::Separator();
				ImGui::Text("Input:");
				ImGui::InputFloat("Delivery interval (s)", &inputDeliveryIntervalSeconds, 0.01f, 0.1f, 4);
				ImGui::Separator();
				//ImGui::Checkbox("Entity interpolation", &App->modGameObject->interpolateEntities);
				ImGui::Checkbox("Client prediction", &clientPrediction);
				if (clientPrediction) ImGui::Checkbox("Server Reconciliation", &serverReconciliation);
			}
		}
	}
	else 
	{
		ImGui::Begin("Game Statistics (since login):");
		ImGui::Text("Zombies killed: %d", zombieDeathCount);
		ImGui::Text("Dead Count: %d", deadCount);
		ImGui::Text("Allies revived: %d", alliesRevived);

		ImGui::Separator();
		ImGui::NewLine();
		if (ImGui::Button("Logout"))
			disconnect();

		ImGui::End();
	}

}
void ModuleNetworkingClient::onPacketReceived(const InputMemoryStream& packet, const sockaddr_in& fromAddress)
{
	lastPacketReceivedTime = Time.time;
	ServerMessage message;
	packet >> message;

	if (state == ClientState::WaitingWelcome)
	{
		if (message == ServerMessage::Welcome)
		{
			packet >> playerId;
			packet >> networkId;
			LOG("ModuleNetworkingClient::onPacketReceived() - Welcome from server");
			state = ClientState::Playing;
		}
		else if (message == ServerMessage::Unwelcome)
		{
			WLOG("ModuleNetworkingClient::onPacketReceived() - Unwelcome from server :-(");
			std::string errorMsg;
			packet >> errorMsg;
			WLOG("Server says: %s", errorMsg.c_str());
			disconnect();
		}
	}
	else if (state == ClientState::Playing)
	{
		// TODO(jesus): Handle incoming messages from server
		if (message == ServerMessage::Replication)
		{
			replicationPing = Time.time - lastReplicationTime;
			lastReplicationTime = Time.time;

			//Receive ACK of input
			uint32 lastPackedProccessed;
			packet >> lastPackedProccessed;
			inputDataFront = lastPackedProccessed;

			//App->delManager->processSequenceNumber(packet);
			replicationManager.read(packet, networkId);

			//Apply all new performed inputs
			if (clientPrediction && serverReconciliation)
				processAllInputs();
		}
	}
}
void ModuleNetworkingClient::onUpdate()
{
	if (state == ClientState::Stopped) return;

	if (state == ClientState::Start)
	{
		// Send the hello packet with player data

		OutputMemoryStream stream;
		stream << ClientMessage::Hello;
		stream << playerName;

		sendPacket(stream, serverAddress);

		state = ClientState::WaitingWelcome;
	}
	else if (state == ClientState::WaitingWelcome)
	{
	}
	else if (state == ClientState::Playing)
	{
		secondsSinceLastInputDelivery += Time.deltaTime;
		secondsSinceLastMouseDelivery += Time.deltaTime;
		secondsSinceLastPing += Time.deltaTime;

		//// Client side prediction
		//GameObject* playerClientGameObject = App->modLinkingContext->getNetworkGameObject(networkId);
		//if (clientPrediction && playerClientGameObject)
		//{
		//	MouseController mouse;
		//	mouse.x = Mouse.x - Window.width / 2;
		//	mouse.y = Mouse.y - Window.height / 2;
		//	mouse.buttons[0] = Mouse.buttons[0];
		//	playerClientGameObject->behaviour->onInput(Input);
		//	playerClientGameObject->behaviour->onMouse(mouse);
		//}
		// Send input packet
		//if (inputDataBack - inputDataFront < ArrayCount(inputData))
		//{
		//	uint32 currentInputData = inputDataBack++;
		//	InputPacketData& inputPacketData = inputData[currentInputData % ArrayCount(inputData)];
		//	inputPacketData.sequenceNumber = currentInputData;
		//	inputPacketData.horizontalAxis = Input.horizontalAxis;
		//	inputPacketData.verticalAxis = Input.verticalAxis;
		//	inputPacketData.buttonBits = packInputControllerButtons(Input);
		//	inputPacketData.mouseX = Mouse.x - Window.width / 2;
		//	inputPacketData.mouseY = Mouse.y - Window.height / 2;
		//	inputPacketData.leftButton = Mouse.buttons[0];
		//	// Create packet (if there's input and the input delivery interval exceeded)
		//	if (secondsSinceLastInputDelivery > inputDeliveryIntervalSeconds)
		//	{
		//		secondsSinceLastInputDelivery = 0.0f;
		//		OutputMemoryStream packet;
		//		packet << ClientMessage::Input;
		//		for (uint32 i = inputDataFront; i < inputDataBack; ++i)
		//		{
		//			InputPacketData& inputPacketData = inputData[i % ArrayCount(inputData)];
		//			packet << inputPacketData.sequenceNumber;
		//			packet << inputPacketData.horizontalAxis;
		//			packet << inputPacketData.verticalAxis;
		//			packet << inputPacketData.buttonBits;
		//			packet << inputPacketData.mouseX;
		//			packet << inputPacketData.mouseY;
		//			packet << inputPacketData.leftButton;
		//		}
		//		// Clear the queue
		//		inputDataFront = inputDataBack;
		//		sendPacket(packet, serverAddress);
		//	}
		//}
		////Send pings to server
		//if (secondsSinceLastPing > PING_INTERVAL_SECONDS)
		//{
		//	secondsSinceLastPing = 0.0f;

		//	OutputMemoryStream ping;
		//	ping << ClientMessage::Ping;
		//	//App->delManager->writeSequenceNumberPendingAck(ping);
		//	sendPacket(ping, serverAddress);
		//	ping.Clear();
		//}
		////Disconnect if waited too long
		//if (Time.time - lastPacketReceivedTime > DISCONNECT_TIMEOUT_SECONDS)
		//{
		//	disconnect();
		//}
	}

	// Make the camera focus the player game object
	/*GameObject* playerGameObject = App->modLinkingContext->getNetworkGameObject(networkId);
	if (playerGameObject != nullptr) App->modRender->cameraPosition = playerGameObject->position;*/
}
void ModuleNetworkingClient::onConnectionReset(const sockaddr_in& fromAddress)
{
	disconnect();
}
void ModuleNetworkingClient::onDisconnect()
{
	state = ClientState::Stopped;

	// Get all network objects and clear the linking context
	uint16 networkGameObjectsCount;
	GameObject* networkGameObjects[MAX_NETWORK_OBJECTS] = {};

	/*App->modLinkingContext->getNetworkGameObjects(networkGameObjects, &networkGameObjectsCount);
	App->modLinkingContext->clear();

	players.clear();
	App->modUI->isPlaying = false;
	App->modUI->debugUI = true;*/

	//Reset game statistics on logout
	zombieDeathCount = 0;
	deadCount = 0;
	alliesRevived = 0;

	// Destroy all network objects
	//for (uint32 i = 0; i < networkGameObjectsCount; ++i)
	//{
	//	Destroy(networkGameObjects[i]);
	//}

	//App->modRender->cameraPosition = {};
}
void ModuleNetworkingClient::floatingUI()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0,0,0,0.3f });
	/*GameObject* clientPlayer = App->modLinkingContext->getNetworkGameObject(networkId);
	if (clientPlayer)
	{
		int count = 0;
		for (std::list<GameObject*>::iterator it = players.begin(); it != players.end(); ++it)
		{
			ImVec2 textSize = ImGui::CalcTextSize((*it)->name.c_str());
			ImGui::SetNextWindowPos({ (*it)->position.x + Window.width / 2 - clientPlayer->position.x - textSize.x / 2 - 7,(*it)->position.y + Window.height / 2 - clientPlayer->position.y - textSize.y - 30 });
			ImGui::Begin(std::to_string(count).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
			ImGui::Text((*it)->name.c_str());
			ImGui::End();

			count++;
		}
	}*/
	ImGui::PopStyleColor(1);
}
void ModuleNetworkingClient::processAllInputs()
{
	//GameObject* playerClientGameObject = App->modLinkingContext->getNetworkGameObject(networkId);
	//if (playerClientGameObject && inputDataBack - inputDataFront < ArrayCount(inputData))
	//{
	//	for (uint32 i = inputDataFront; i < inputDataBack; ++i) //For all current inputs (not processed by the server)
	//	{
	//		InputPacketData& inputPacketData = inputData[i % ArrayCount(inputData)];

	//		InputController input;
	//		MouseController mouse;

	//		//Process Keyboard
	//		input.horizontalAxis = inputPacketData.horizontalAxis;
	//		input.verticalAxis = inputPacketData.verticalAxis;
	//		unpackInputControllerButtons(inputPacketData.buttonBits, input);
	//		playerClientGameObject->behaviour->onInput(input);

	//		//Process Mouse
	//		mouse.x = inputPacketData.mouseX;
	//		mouse.y = inputPacketData.mouseY;
	//		mouse.buttons[0] = (ButtonState)inputPacketData.leftButton;
	//		playerClientGameObject->behaviour->onMouse(mouse);
	//	}
	//}
}
