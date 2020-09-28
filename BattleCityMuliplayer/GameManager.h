/**************************************************************************
*  @file	 : GameManager.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 游戏管理器类
**************************************************************************/
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

class GameManager
{
public:
	/********************************************************
		*  @brief    :	设置当前关数
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setLevel(int m_Level){ Level = m_Level; }
	/********************************************************
		*  @brief    :	得到当前关数
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int getLevel(){ return Level; }
	static GameManager* getInstance();
	~GameManager();
	/********************************************************
		*  @brief    :	创建一个游戏
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool Create(HWND hwnd, HINSTANCE hInstance);
	/********************************************************
		*  @brief    :	检测当前按键
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void checkclick1();
	void checkclick2();
	void checkclick3();
	void checkclick4();
	/********************************************************
		*  @brief    :	绘制精灵
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void draw(Sprite* sp);
	/********************************************************
		*  @brief    :	设置当前场景
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setScene(Scene* scene);
	/********************************************************
		*  @brief    :	得到当前场景
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	Scene* getScene();
	/********************************************************
		*  @brief    :	绘制所有精灵
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void drawSprite();
	/********************************************************
		*  @brief    :	得到当时按键1:WSAD  2:空格 3:上下左右小键盘9ESC 4:小键盘0回车
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	UINT getClick1();
	UINT getClick2();
	UINT getClick3();
	UINT getClick4();
	/********************************************************
		*  @brief    :	设置按键检测状态
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setClick1();
	void setClick2();
	void setClick3();
	void setClick4();
	/********************************************************
		*  @brief    :	更新碰撞物体属性（需改)
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void UpdateColl(Tank* tank,Bullet* bullet);
	void UpdateColl(Bullet* bullet1, Bullet* bullet2);
	void UpdateColl(StaticSprite* sp1, Bullet* bullet);
	void UpdateColl(Award* aw, Tank* tank);
	/********************************************************
		*  @brief    :	设置玩家数量
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setPlayer(int p){ Player = p; }
	/********************************************************
		*  @brief    :	得到玩家数量
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int getPlayer(){ return Player; }
	/********************************************************
		*  @brief    :	得到当前分数
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int getGrade(int Player)
	{
		if (Player == 1)
		{
			return Grade1;
		}
		if (Player==2)
		{
			return Grade2;
		}
	}
	/********************************************************
		*  @brief    :	增加分数
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void addGrade(int Player)
	{ 
		if (Player == 1)
		{
			Grade1+=GradeAdd;
		}
		if (Player == 2)
		{
			Grade2+=GradeAdd;
		}
	}
	/********************************************************
		*  @brief    :	设置分数
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
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
	/********************************************************
		*  @brief    :	得到玩家生命值
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
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
	/********************************************************
		*  @brief    :	设置玩家生命值
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
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
	/********************************************************
		*  @brief    :	玩家生命值减一
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
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
	/********************************************************
		*  @brief    :	设置地图
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setMap();
	/********************************************************
		*  @brief    :	改变自制地图的状态
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void changeMapMake(){ MapMake = !MapMake; }
	/********************************************************
		*  @brief    :	得到自制地图的状态
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool getMapMake(){ return MapMake; }
private:
	bool MapMake;
	int Player;
	GameManager(){};
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
	Sound* award;
	Sound* LifeAdd;
	Sound* boom;
};

#endif // !GAMEMANAGE_H