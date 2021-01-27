#pragma once

enum class ClientMessage
{
	Hello,
	Input,
	Ping
};

enum class ServerMessage
{
	Welcome,
	Unwelcome,
	Replication,
	StartGame,
	StopGame,
	Snapshot,
	SwitchScene,
	ClientSize,
	Ping
};
