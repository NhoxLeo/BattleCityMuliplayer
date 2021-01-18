#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "common.h"
#include "Scene.h"

class ActiveSprite;

class StartScene :public Scene
{
public:

	void Update();
	CREATE_FUNC(StartScene);
	virtual bool init();
private:
	ActiveSprite* Cursor;
	int CursorPosition;
};

#endif