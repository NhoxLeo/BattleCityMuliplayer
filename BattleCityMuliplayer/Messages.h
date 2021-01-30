#pragma once

enum class ClientMessage
{
	Hello,
	Input,
	Ready,
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
