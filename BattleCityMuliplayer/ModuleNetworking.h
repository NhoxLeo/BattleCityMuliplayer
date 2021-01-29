#pragma once

#define SIMULATE_REAL_WORLD_CONDITIONS

struct InputPacketData
{
	uint32 sequenceNumber = 0;
	uint32 tickCount = 0;
	real32 horizontalAxis = 0.0f;
	real32 verticalAxis = 0.0f;
	uint16 buttonBits = 0;
	float x = 0.0f;
	float y = 0.0f;
	bool shoot = false;
};

class ModuleNetworking : public Module
{
public:

	virtual bool isServer() const { return false; }

	virtual bool isClient() const { return false; }

	bool isConnected() const { return socket != INVALID_SOCKET; }

	void disconnect();

	float GetSimulatedLatency();

protected:

	//////////////////////////////////////////////////////////////////////
	// ModuleNetworking protected methods
	//////////////////////////////////////////////////////////////////////

	SOCKET socket = INVALID_SOCKET;

	bool createSocket();

	bool bindSocketToPort(int port);

	void sendPacket(const OutputMemoryStream& packet, const sockaddr_in& destAddress);

	void sendPacket(const char* data, uint32 size, const sockaddr_in& destAddress);

	void reportError(const char* message);

	uint16 packInputControllerButtons(const InputController& input)
	{
		uint16 buttonBits = 0;
		int buttonIndex = 0;
		for (ButtonState buttonState : input.buttons)
		{
			uint16 bit = (int)(buttonState == ButtonState::Press ||
				buttonState == ButtonState::Pressed);
			buttonBits |= (bit << buttonIndex);
			buttonIndex++;
		}

		//if (input.actionDown == ButtonState::Press)
		//{
		//	int a = 0;
		//}
		//else if (input.actionDown == ButtonState::Pressed)
		//{
		//	int b = 0;
		//}

		return buttonBits;
	}
	void unpackInputControllerButtons(uint16 buttonBits, InputController& input)
	{
		// NOTE(jesus): This table contains the ButtonState depending on the pressed state (true or false) of a button
		static const ButtonState transition[2][2] =      // Index 0 means released, 1 means pressed
		{
			{ButtonState::Idle,    ButtonState::Press},  // 0 0, 0 1
			{ButtonState::Release, ButtonState::Pressed} // 1 0, 1 1
		};

		//if (buttonBits != 0)
		//{
		//	int a = 0;
		//	
		//	if (input.actionDown != ButtonState::Idle)
		//	{
		//		int y = 8;
		//	}
		//}

		int buttonIndex = 0;
		for (ButtonState& buttonState : input.buttons)
		{
			int wasPressed = (int)(buttonState == ButtonState::Press || buttonState == ButtonState::Pressed);
			int isPressed = (int)(bool)(buttonBits & (1 << buttonIndex));
			buttonState = transition[wasPressed][isPressed];
			buttonIndex++;
		}
	}


private:

	//////////////////////////////////////////////////////////////////////
	// Module virtual methods
	//////////////////////////////////////////////////////////////////////

	bool init() override;

	bool start() override;

	bool preUpdate() override;

	bool update() override;

	bool gui() override;

	bool stop() override;

	bool cleanUp() override;



	//////////////////////////////////////////////////////////////////////
	// ModuleNetworking methods
	//////////////////////////////////////////////////////////////////////

	uint32 sentPacketsCount = 0;
	uint32 receivedPacketsCount = 0;
	InputMemoryStream inPacket;

	void processIncomingPackets();

	virtual void onStart() = 0;

	virtual void onGui() = 0;

	virtual void onPacketReceived(const InputMemoryStream& packet, const sockaddr_in& fromAddress) = 0;

	virtual void onUpdate() = 0;

	virtual void onConnectionReset(const sockaddr_in& fromAddress) = 0;

	virtual void onDisconnect() = 0;

	virtual void floatingUI() {};

private:

#if defined(SIMULATE_REAL_WORLD_CONDITIONS)

	//////////////////////////////////////////////////////////////////////
	// Real world conditions simulation
	//////////////////////////////////////////////////////////////////////

	bool simulateLatency = false;
	float simulatedLatency = 0.07f;
	float simulatedJitter = 0.03f;
	bool simulateDrops = false;
	float simulatedDropRatio = 0.004f;
	uint32 simulatedPacketsReceived = 0;
	uint32 simulatedPacketsDropped = 0;
	uint32 replicationPacketsDropped = 0;
	uint32 pingPacketsDropped = 0;
	uint32 unwelcomePacketDropped = 0;
	uint32 welcomePacketDropped = 0;

	void simulatedRealWorldConditions_EnqueuePacket(const InputMemoryStream& packet, const sockaddr_in& fromAddress);

	void simulatedRealWorldConditions_ProcessQueuedPackets();

	struct SimulatedPacket {
		InputMemoryStream packet;
		sockaddr_in fromAddress;
		double receptionTime;
	};

	std::deque<SimulatedPacket> simulatedPacketQueue;
	RandomNumberGenerator simulatedRandom;

#endif

};

