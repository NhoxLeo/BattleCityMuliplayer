#include "StaticSprite.h"
#include "Scene.h"
#include "Texture.h"

Texture* theTexture = NULL;

StaticSprite* StaticSprite::Create(int Type)
{
	StaticSprite* sp = new StaticSprite();
	if (sp&&sp->init(Type))
	{
		sp->autorelease();
		return sp;
	}
	delete sp;
	sp = NULL;
	return NULL;
}

bool StaticSprite::init(int Type)
{
	if (theTexture == NULL)
	{
		theTexture = new Texture;
		theTexture->LoadTexture(_T("../Sprite/tile.bmp"), 224, 32, 1);
	}
	Sprite::init();
	m_type = Type;
	StaticSpriteArray::getInstance()->pushSprite(this);
	LoadTexture(theTexture);
	setRet(Ret(Size(32, 32), D3DXVECTOR3(0, 0, 0)));
	switch (m_type)
	{
	case 1:
		setRenderRet(16, 16, 0, 0);
		break;
	case 2:
		setRenderRet(16, 16, 32, 0);
		break;
	case 3:
		setRenderRet(16, 16, 64, 0);
		break;
	case 4:
		setRenderRet(16, 16, 96, 0);
		break;
	case 6:
		setRenderRet(16, 16, 160, 0);
		break;
	case 7:
		setRenderRet(16, 16, 176, 0);
		break;
	case 8:
		setRenderRet(16, 16, 160, 16);
		break;
	case 9:
		setRenderRet(16, 16, 176, 16);
	default:
		break;
	}
	return true;
}

StaticSpriteArray* StaticSpriteArray::getInstance()
{
	static StaticSpriteArray* Instance;
	if (Instance == NULL)
	{
		Instance = new StaticSpriteArray();
		Instance->StaticSpriteNumber = 0;
	}
	return Instance;
}

void StaticSpriteArray::pushSprite(StaticSprite* sp)
{
 	StaticArray.push_back(sp);
	StaticSpriteNumber++;
}

void StaticSpriteArray::removeStaticSprite(StaticSprite* sp)
{
	for (int i = 0; i < StaticSpriteNumber;i++)
	{
		if (StaticArray[i] == sp)
		{
			StaticArray.erase(StaticArray.begin() + i);
			StaticSpriteNumber--;
			return;
		}
	}
}

void StaticSprite::removeFromParent()
{
	theScene->removeStaticChild(this);
}

StaticSprite::~StaticSprite()
{
	StaticSpriteArray::getInstance()->removeStaticSprite(this);
 	if (theScene != NULL)
 	{
 		removeFromParent();
 	}
}