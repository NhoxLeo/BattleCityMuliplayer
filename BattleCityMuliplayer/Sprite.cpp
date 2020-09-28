#include "Sprite.h"
#include "Action.h"
#include "MyRender.h"
#include "Scene.h"


void Sprite::setRenderRet(int height, int width, int x, int y)
{
	TheRenderRet = new RenderRet();
	TheRenderRet->RenderPoint = new D3DXVECTOR3(x,y,0);
	TheRenderRet->RenderSize = new Size(height, width);
}

RenderRet* Sprite::getRenderRet()
{
	return TheRenderRet;
}

void Sprite::setScene(Scene* scene)
{
	theScene = scene;
}

bool Sprite::init()
{
	Show = false;
	m_D3DSprite = NULL;
	theScene = NULL;
	D3DXCreateSprite(MyRender::getInstance()->getDevice(), &m_D3DSprite);
	TheRenderRet = NULL;
	ret = Ret(Size(0, 0), D3DXVECTOR3(0, 0, 0));
	m_position = ret.getPosition();
	m_Center = D3DXVECTOR3(0, 0, 0);
	SpriteArray::getInstance()->addSprite(this);
	return true;
}

void Sprite::setPosition(D3DXVECTOR3 position)
{
	ret.setPosition(position);
	this->m_position = D3DXVECTOR3(position.x,position.y,0);
	setRet(Ret(getSize(), m_position));
}

D3DXVECTOR3 Sprite::getPosition()
{
	return m_position;
}

int Sprite::getHeight()
{
	return ret.getSize().getHeight();
}

int Sprite::getwidth()
{
	return ret.getSize().getwidth();
}

Size Sprite::getSize()
{
	return ret.getSize();
}

void Sprite::setRet(Ret m_ret)
{
	ret = m_ret;
	m_Center = D3DXVECTOR3(ret.getSize().getwidth() / 2,ret.getSize().getHeight() / 2,0);
}

void Sprite::draw()
{
	if (Show)
	{
		if (SUCCEEDED(m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND)))
		{
			m_D3DSprite->Draw(m_Texture->getTexture(), NULL, &m_Center, &m_position, 0xffffffff);
			m_D3DSprite->End();
		}
	}
}

void Sprite::drawWithPartTexture(D3DXVECTOR3 p,Size s)
{
	if (Show)
	{
		RECT rect;
		rect.bottom = p.y + s.getHeight();
		rect.top = p.y;
		rect.left = p.x;
		rect.right = p.x + s.getwidth();
		if (SUCCEEDED(m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND)))
		{
			m_D3DSprite->Draw(m_Texture->getTexture(), &rect, &m_Center, &m_position, 0xffffffff);
			m_D3DSprite->End();
		}
	}
}

void Sprite::runAction(Action* action)
{
	action->setTarget(this);
	action->retain();
	theAction = action;
}

void Sprite::LoadTexture(Texture* texture)
{
	m_Texture = texture;
}

void Sprite::LoadTexture(LPCTSTR path, UINT width, UINT height, D3DCOLOR color)
{
	Texture* t = new Texture();
	t->LoadTexture(path, width, height, color);
	m_Texture = t;
}

Sprite::~Sprite()
{
	SpriteArray::getInstance()->removeSprite(this);
	m_D3DSprite = NULL;
	if (theAction != NULL)
	{
		theAction->release();
	}
}

SpriteArray* SpriteArray::getInstance()
{
	static SpriteArray* Instance;
	if (Instance == NULL)
	{
		Instance = new SpriteArray();
		Instance->SpriteNumber = 0;
	}
	return Instance;
}

vector<Sprite*> SpriteArray::getSprite()
{
	return Spritearray;
}

int SpriteArray::getSpriteNumber()
{
	return SpriteNumber;
}

void SpriteArray::addSprite(Sprite* sp)
{
	Spritearray.push_back(sp);
	SpriteNumber++;
}

LPD3DXSPRITE Sprite::getSprite()
{
	return m_D3DSprite;
}

void SpriteArray::removeSprite(Sprite* sp)
{
	for (int i = 0; i < SpriteNumber; i++)
	{
		if (Spritearray[i] == sp)
		{
			Spritearray.erase(Spritearray.begin() + i);
			break;
		}
	}
	SpriteNumber--;
}

void Sprite::setAction(Action* ac)
{
	theAction = ac;
}