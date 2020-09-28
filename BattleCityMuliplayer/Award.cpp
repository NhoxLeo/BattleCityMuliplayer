#include "Award.h"
#include "Action.h"
#include "Scene.h"

void Award::Update()
{
	if (Timer == 10*FRAME)
	{
		detain();
	}
	if (Timer == 7*FRAME)
	{
		Blink* action = Blink::create();
		action->setDuration(3);
		action->setTime(0.5);
		runAction(action);
	}
	Timer++;
}

bool Award::init()
{
	Sprite::init();
	int x = rand() % 384 + 128;
	int y = rand() % 384 + 60;
	m_type = rand() % 6;
	Timer = 0;
	LoadTexture(_T("../Sprite/bonus.bmp"), 180, 28, 1);
	setRet(Ret(Size(32, 32), D3DXVECTOR3(x, y, 0)));
	setPosition(D3DXVECTOR3(x, y, 0));
	setRenderRet(30, 28, 30 * m_type, 0);
	return true;
}

Award::~Award()
{
	thescene->clearAward();
}