/**************************************************************************
*  @file	 : OverScene.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : ”Œœ∑ ß∞‹ª≠√Ê
**************************************************************************/
#ifndef OVERSCENE_H
#define OVERSCENE_H

#include "common.h"
#include "Scene.h"

class Sound;

class OverScene :public Scene
{
public:
	void Update();
	CREATE_FUNC(OverScene);
	virtual bool init();
private:
	int counter;
	Sound* oversound;
};

#endif