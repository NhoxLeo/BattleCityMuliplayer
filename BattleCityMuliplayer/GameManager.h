
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

	void UpdateColl(Tank* tank, Bullet* bullet);
	void UpdateColl(Bullet* bullet1, Bullet* bullet2);
	void UpdateColl(StaticSprite* sp1, Bullet* bullet);
	void UpdateColl(Award* aw, Tank* tank);

	void setPlayer(int p) { Player = p; }
	int getPlayer() { return Player; }
	int getGrade(int Player)
	{
		if (Player == 1)
		{
			return Grade1;
		}
		if (Player == 2)
		{
			return Grade2;
		}
	}
	void addGrade(int Player)
	{
		if (Player == 1)
		{
			Grade1 += GradeAdd;
		}
		if (Player == 2)
		{
			Grade2 += GradeAdd;
		}
	}
	void setGrade(int player, int grade)
	{
		if (player == 1)
		{
			Grade1 = grade;
		}
		if (player == 2)
		{
			Grade2 = grade;
		}
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
	void changeMapMake() { MapMake = !MapMake; }
	bool getMapMake() { return MapMake; }

	ModuleNetworkingServer* GetModNetServer() { return modNetServer; }
	ModuleNetworkingClient* GetModNetClient() { return modNetClient; }
	void CreateServer();
	void CreateClient();

private:
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
	/*Sound* award;
	Sound* LifeAdd;
	Sound* boom;*/

	ModuleNetworkingServer* modNetServer = nullptr;
	ModuleNetworkingClient* modNetClient = nullptr;
};

#endif // !GAMEMANAGE_H