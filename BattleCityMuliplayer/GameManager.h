/**************************************************************************
*  @file	 : GameManager.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : ��Ϸ��������
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
		*  @brief    :	���õ�ǰ����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setLevel(int m_Level){ Level = m_Level; }
	/********************************************************
		*  @brief    :	�õ���ǰ����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int getLevel(){ return Level; }
	static GameManager* getInstance();
	~GameManager();
	/********************************************************
		*  @brief    :	����һ����Ϸ
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool Create(HWND hwnd, HINSTANCE hInstance);
	/********************************************************
		*  @brief    :	��⵱ǰ����
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
		*  @brief    :	���ƾ���
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void draw(Sprite* sp);
	/********************************************************
		*  @brief    :	���õ�ǰ����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setScene(Scene* scene);
	/********************************************************
		*  @brief    :	�õ���ǰ����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	Scene* getScene();
	/********************************************************
		*  @brief    :	�������о���
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void drawSprite();
	/********************************************************
		*  @brief    :	�õ���ʱ����1:WSAD  2:�ո� 3:��������С����9ESC 4:С����0�س�
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
		*  @brief    :	���ð������״̬
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
		*  @brief    :	������ײ�������ԣ����)
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
		*  @brief    :	�����������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setPlayer(int p){ Player = p; }
	/********************************************************
		*  @brief    :	�õ��������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int getPlayer(){ return Player; }
	/********************************************************
		*  @brief    :	�õ���ǰ����
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
		*  @brief    :	���ӷ���
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
		*  @brief    :	���÷���
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
		*  @brief    :	�õ��������ֵ
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
		*  @brief    :	�����������ֵ
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
		*  @brief    :	�������ֵ��һ
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
		*  @brief    :	���õ�ͼ
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setMap();
	/********************************************************
		*  @brief    :	�ı����Ƶ�ͼ��״̬
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void changeMapMake(){ MapMake = !MapMake; }
	/********************************************************
		*  @brief    :	�õ����Ƶ�ͼ��״̬
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