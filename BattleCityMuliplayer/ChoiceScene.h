/**************************************************************************
*  @file	 : ChoiceScene.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    :  游戏选关场景
**************************************************************************/
#ifndef CHOICESCENE_H
#define CHOICESCENE_H

#include "common.h"
#include "Scene.h"

class ActiveSprite;

class ChoiceScene :public Scene
{
public:
	void Update();
	CREATE_FUNC(ChoiceScene);
	virtual bool init();
private:
	int nowLevel;
	ActiveSprite* decade;
	ActiveSprite* unit;
};

#endif