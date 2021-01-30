
#ifndef GAMEMANAGE_H
#define GAMEMANAGE_H

#include "common.h"

class ActiveSprite;
class StaticSprite;
class Bullet;
class Tank;
class Scene;
class Sprite;
class Award;
class Map;
class Sound;
class ModuleNetworkingServer;
class ModuleNetworkingClient;
class DeliveryManager;
class ModuleLinkingContext;
class ModuleGameObject;

class GameManager
{
public:

	void setLevel(int m_Level) { Level = m_Level; }

	int getLevel() { return Level; }
	static GameManager* getInstance();
	~GameManager();

	bool Create(HWND hwnd, HINSTANCE hInstance);

	void checkclick1();
	void checkclick2();
	void checkclick3();
	void checkclick4();

	void draw(Sprite* sp);

	void setScene(Scene* scene);

	Scene* getScene();

	void drawSprite();

	UINT getClick1();
	UINT getClick2();
	UINT getClick3();
	UINT getClick4();

	void setClick1();
	void setClick2();
	void setClick3();
	void setClick4();

	D3DXVECTOR3 Lerp(D3DXVECTOR3 a, D3DXVECTOR3 b, float f)
	{
		D3DXVECTOR3 result;
		result.x = a.x + f * (b.x - a.x);
		result.y = a.y + f * (b.y - a.y);
		return result;
	}

	void UpdateColl(Tank* tank, Bullet* bullet);
	void UpdateColl(Bullet* bullet1, Bullet* bullet2);
	void UpdateColl(StaticSprite* sp1, Bullet* bullet);
	void UpdateColl(Award* aw, Tank* tank);

	void setPlayer(int p) { Player = p; }
	int getPlayer() { return Player; }
	int getGrade(int Player)
	{
		return Grade1;
		/*if (Player == 1)
		{
			return Grade1;
		}
		if (Player == 2)
		{
			return Grade2;
		}*/
	}
	void addGrade(int Player)
	{
		Grade1 += GradeAdd;
		/*if (Player == 1)
		{
			Grade1 += GradeAdd;
		}
		if (Player == 2)
		{
			Grade2 += GradeAdd;
		}*/
	}
	void setGrade(int player, int grade)
	{
		Grade1 = grade;
		/*if (player == 1)
		{
			Grade1 = grade;
		}
		if (player == 2)
		{
			Grade2 = grade;
		}*/
	}
	int getMyLife(int Player)
	{
		if (Player == 1)
		{
			return MyLife1;
		}
		if (Player == 2)
		{
			return MyLife2;
		}
	}
	void setMyLife(int Life, int Player)
	{
		if (Player == 1)
		{
			MyLife1 = Life;
		}
		if (Player == 2)
		{
			MyLife2 = Life;
		}
	}
	void LostLife(int Player)
	{
		if (Player == 1)
		{
			MyLife1--;
		}
		if (Player == 2)
		{
			MyLife2--;
		}
	}
	void setMap();
	void SetCurrentMap(Map* _map) { currentmap = _map; };
	Map* GetCurrentMap() { return currentmap; };
	void changeMapMake() { MapMake = !MapMake; }
	bool getMapMake() { return MapMake; }
	D3DXVECTOR3 getAwardPosition();
	int getAwardType();
	void setAward();
	void setAward(int type,D3DXVECTOR3 pos);
	bool IsWinning() { return win; }
	void SetWinning(bool _win) { win = _win; }
	int GetFrameSnapshotTick() { return frametick; }
	void SetFrameSnapshotTick(int _tick) { frametick = _tick; }
	vector<bool>* GetWallEnabledArray();
	void SetWallEnabledArray(vector<bool>* _list);
	void LoadNetworkScene();

	void CreatePlayerTank(UINT32 _networkID, UINT32 _playerID,int level, D3DXVECTOR3 position);
	void CreateAIPlayerTank(UINT32 _networkID,int level, D3DXVECTOR3 position);
	void DeletePlayerTank(UINT32 _networkID);
	void DeleteAllPlayerTank();
	void UpdateAllPlayerTank();
	void AllPlayerTankVisitAll();
	void AllAIPlayerTankVisitAll();
	void TankVisitAll(UINT32 _networkID, CollisionCheckMethod method);
	void UpdateAllTanks();
	void UpdatePlayerTank(UINT32 _networkID, D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 speed);
	void UpdatePlayerTankWithLatency(UINT32 _networkID, D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 speed,int lateframes);
	void UpdatePlayerTankWithInput(UINT32 _networkID, D3DXVECTOR3 _input);
	Tank* GetPlayerTank(int _networkID);
	int GetPlayerTankID(int _networkID);
	D3DXVECTOR3 GetPlayerTankPosition(int _networkID);
	D3DXVECTOR3 GetPlayerTankRotation(int _networkID);
	D3DXVECTOR3 GetPlayerTankSpeed(int _networkID);
	int GetPlayerTankLevel(int _networkID);
	int GetTanksCount();
	int GetPlayerTanksCount();
	void AITankControl();
	void ReduceLife(int _networkID);

	Bullet* CreatePlayerBullet(UINT32 _networkID, D3DXVECTOR3 position);
	Bullet* CreatePlayerBulletWithLatency(UINT32 _networkID, D3DXVECTOR3 position,int lateframes);
	void CreateAndUpdatePlayerBulletWithLatency(UINT32 _networkID, CollisionCheckMethod method, int lateframes);
	void BulletVisitAll();
	void BulletVisitAll(UINT32 _networkID, CollisionCheckMethod method);
	void BulletVisitAllWithLatency(UINT32 _networkID, CollisionCheckMethod method, int lateframes);

	ModuleNetworkingServer* GetModNetServer() { return modNetServer; }
	ModuleNetworkingClient* GetModNetClient() { return modNetClient; }
	void CreateServer();
	void DeleteServer();
	void CreateClient();
	void DeleteClient();
	DeliveryManager* GetDeliveryManager() { return delManager; }
	ModuleLinkingContext* GetModLinkingContext() { return modLinkingContext; }
	ModuleGameObject* GetModGameObject() { return modGameObject; }
	void NetworkUpdate();

	deque<std::vector<Tank*>*>* GetLastFrameObjectsInfo() { return lastFrameObjectsInfo; }
	void AddThisFrameObjects();
	void ServerSnapShotDeleteBrickID(int _cloneID);
	void IncreaseReadyClients() { clientsReady++; }
	int GetReadyClients() { return clientsReady; }
	void ClientSendReadyPackage();

private:
	int frametick = 0;
	bool MapMake;
	int Player;
	GameManager() {};
	Scene* nowScene;
	static UINT nowclick1;
	static UINT nowclick2;
	static UINT nowclick3;
	static UINT nowclick4;
	int MyLife1;
	int MyLife2;
	int Level;
	int Grade1;
	int Grade2;
	int win;
	Map* currentmap;
	/*Sound* award;
	Sound* LifeAdd;
	Sound* boom;*/
	int clientsReady = 0;

	ModuleNetworkingServer* modNetServer = nullptr;
	ModuleNetworkingClient* modNetClient = nullptr;
	DeliveryManager* delManager = nullptr;
	ModuleLinkingContext* modLinkingContext = nullptr;
	ModuleGameObject* modGameObject = nullptr;

	deque<std::vector<Tank*>*>* lastFrameObjectsInfo;
};

#endif // !GAMEMANAGE_H