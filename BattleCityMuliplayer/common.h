#ifndef COMMON_H
#define COMMON_H

/**************************************************************************
*  @file	 : common.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 各个头文件的包含以及相关宏的定义
**************************************************************************/
#include <iostream>
#include <vector>
#include <math.h>
#include <cassert>
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <dsound.h>
#include <Mmreg.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
//#include "bass.h"
//
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

//#pragma  comment(lib,"bass.lib")
#pragma comment(lib,"Dinput8.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma message("FDraw.h-->Linking with ddraw.lib")
//#pragma comment(lib,"ddraw.lib")
#pragma  comment(lib,"DXGuid.lib")

using namespace std;

//#define NULL ((void*)0x00000000)
#define  WindowClassName _T("BattleCity")
#define DOU double
#define BulletSpeed 5//子弹的速度
#define TankSpeed 2//坦克的速度
#define InvincibleTime 3//无敌的时间
#define UntouchAbleTime 3
#define EnemyPosition1 (D3DXVECTOR3(128,60,0))
#define EnemyPosition2 (D3DXVECTOR3(320,60,0))
#define EnemyPosition3 (D3DXVECTOR3(512,60,0))
#define EnemyPosition4 (D3DXVECTOR3(128,252,0))
#define EnemyPosition5 (D3DXVECTOR3(512,252,0))
#define EnemyTime 10//检测敌方坦克数量的时间间隔
#define MapNumber 34
#define GradeAdd 100//攻击敌方坦克时得分
#define Cross 1
#define InitLife 6//玩家初始生命值
#define InitGrade 0//玩家初始积分
#define YESFIRE true//敌方坦克是否可以fire
#define FRAME 60
#define AwardNumber 1//每AwardNumber个坦克出现一个奖励
#define StopTime 3


#define DOWNBUTTON_UP (WM_USER+1)
#define DOWNBUTTON_ON (WM_USER+2)
#define DOWNBUTTON_DOWN (WM_USER+3)
#define UPBUTTON_UP (WM_USER+4)
#define UPBUTTON_ON (WM_USER+5)
#define UPBUTTON_DOWN (WM_USER+6)
#define LEFTBUTTON_UP (WM_USER+7)
#define LEFTBUTTON_ON (WM_USER+8)
#define LEFTBUTTON_DOWN (WM_USER+9)
#define RIGHTBUTTON_UP (WM_USER+10)
#define RIGHTBUTTON_ON (WM_USER+11)
#define RIGHTBUTTON_DOWN (WM_USER+12)
#define SPACEBUTTON_UP (WM_USER+13)
#define SPACEBUTTON_ON (WM_USER+14)
#define SPACEBUTTON_DOWN (WM_USER+15)
#define WBUTTON_UP (WM_USER+16)
#define WBUTTON_ON (WM_USER+17)
#define WBUTTON_DOWN (WM_USER+18)
#define ABUTTON_UP (WM_USER+19)
#define ABUTTON_ON (WM_USER+20)
#define ABUTTON_DOWN (WM_USER+21)
#define SBUTTON_UP (WM_USER+22)
#define SBUTTON_ON (WM_USER+23)
#define SBUTTON_DOWN (WM_USER+24)
#define DBUTTON_UP (WM_USER+25)
#define DBUTTON_ON (WM_USER+26)
#define DBUTTON_DOWN (WM_USER+27)
#define NUMBERZEROBUTTON_UP (WM_USER+28)
#define NUMBERZEROBUTTON_ON (WM_USER+29)
#define NUMBERZEROBUTTON_DOWN (WM_USER+30)
#define LBUTTON_UP (WM_USER+31)
#define LBUTTON_ON (WM_USER+32)
#define LBUTTON_DOWN (WM_USER+33)
#define NUMBERNINEBUTTON_DOWN (WM_USER+34)
#define NUMBERNINEBUTTON_ON (WM_USER+35)
#define NUMBERNINEBUTTON_UP (WM_USER+36)
#define ENTERBUTTON_DOWN (WM_USER+37)
#define ENTERBUTTON_ON (WM_USER+38)
#define ENTERBUTTON_UP (WM_USER+39)
#define ESCBUTTON_DOWN (WM_USER+40)
#define ESCBUTTON_ON (WM_USER+41)
#define ESCBUTTON_UP (WM_USER+42)

#define CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new  __TYPE__(); \
    if (pRet && pRet->init()) \
				    { \
        pRet->autorelease(); \
        return pRet; \
				    } \
															    else \
			    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
			    } \
}


#endif // !COMMON_H