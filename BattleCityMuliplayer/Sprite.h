/**************************************************************************
*  @file	 : Sprite.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : æ´¡È¿‡
**************************************************************************/
#ifndef SPRITE_H
#define SPRITE_H

#include "Ref.h"
#include "Ret.h"
#include "Texture.h"

class ActionManager;
class Action;
class Scene;


struct RenderRet
{
	RenderRet(){ RenderSize = new Size; RenderPoint = new D3DXVECTOR3; }
	Size* RenderSize;
	D3DXVECTOR3* RenderPoint;
};

class Sprite:public Ref
{
public:
	Sprite(){};
	~Sprite();
	CREATE_FUNC(Sprite);

	void setRenderRet(int height,int width,int x,int y);

	RenderRet* getRenderRet();

	bool init();

	void setCenter(D3DXVECTOR3 center){ m_Center = center; }
	
	void setPosition(D3DXVECTOR3 position);

	D3DXVECTOR3 getPosition();

	int getHeight();

	int getwidth();

	Size getSize();

	void OnShow(){ Show = true; }

	void StopShow(){ Show = false; }

	void SwitchShow(){ Show = !Show; }

	bool getShowState(){ return Show; }

	void draw();

	void drawWithPartTexture(D3DXVECTOR3 p,Size s);

	void LoadTexture(Texture* texture);

	void runAction(Action*);

	void LoadTexture(LPCTSTR path, UINT width, UINT height, D3DCOLOR color);

	LPD3DXSPRITE getSprite();

	void setScene(Scene* scene);

	Scene* getScene(){ return theScene; }

	void setRet(Ret m_ret);

	Ret getRet(){ return ret; }

	D3DXVECTOR3 getCenter(){ return m_Center; }

	void setMoveState(bool move){ Moveable = move; }

	bool getMoveState(){ return Moveable; }

	void setAction(Action*);
protected:
	RenderRet* TheRenderRet;
	LPD3DXSPRITE m_D3DSprite;
	Texture* m_Texture;
	Ret ret;
	D3DXVECTOR3 m_position;
	bool Show;
	D3DXVECTOR3 m_Center;
	Scene* theScene;
	bool Moveable;
	Action* theAction;
};


class SpriteArray
{
private:
	SpriteArray(){}
	vector<Sprite*> Spritearray;
	int SpriteNumber;
public:

	static SpriteArray* getInstance();

	void addSprite(Sprite*);

	vector<Sprite*> getSprite();

	int getSpriteNumber();

	void removeSprite(Sprite* sp);
};

#endif