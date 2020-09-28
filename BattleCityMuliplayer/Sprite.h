/**************************************************************************
*  @file	 : Sprite.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : 精灵类
**************************************************************************/
#ifndef SPRITE_H
#define SPRITE_H

#include "Ref.h"
#include "Ret.h"
#include "Texture.h"

class ActionManager;
class Action;
class Scene;

/**************************************************************************
*  @file	 : Sprite.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 渲染矩形结构体
**************************************************************************/
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
	/********************************************************
		*  @brief    :	设置渲染的矩形区域
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setRenderRet(int height,int width,int x,int y);
	/********************************************************
		*  @brief    :	得到渲染的区域
		*  @input	 :
		*  @output	 :
		*  @return   :  RenderRet*
		*  @author   :	Alex
		********************************************************/
	RenderRet* getRenderRet();
	/********************************************************
		*  @brief    :	精灵类初始化
		*  @input	 :
		*  @output	 :
		*  @return   :  bool
		*  @author   :	Alex
		********************************************************/
	bool init();
	/********************************************************
		*  @brief    :	设置精灵的中心
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setCenter(D3DXVECTOR3 center){ m_Center = center; }
	/********************************************************
		*  @brief    :	设置精灵的位置（设置时会更新精灵矩形的位置）
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setPosition(D3DXVECTOR3 position);
	/********************************************************
		*  @brief    :	获得精灵的位置
		*  @input	 :
		*  @output	 :
		*  @return   :  D3DXVECTOR3
		*  @author   :	Alex
		********************************************************/
	D3DXVECTOR3 getPosition();
	/********************************************************
		*  @brief    :	获得精灵的高度
		*  @input	 :
		*  @output	 :
		*  @return   :  int
		*  @author   :	Alex
		********************************************************/
	int getHeight();
	/********************************************************
		*  @brief    :	获得精灵的宽度
		*  @input	 :
		*  @output	 :
		*  @return   :  int
		*  @author   :	Alex
		********************************************************/
	int getwidth();
	/********************************************************
		*  @brief    :	获得精灵的大小
		*  @input	 :
		*  @output	 :
		*  @return   :  Size
		*  @author   :	Alex
		********************************************************/
	Size getSize();
	/********************************************************
		*  @brief    :	让精灵显示
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void OnShow(){ Show = true; }
	/********************************************************
		*  @brief    :	让精灵停止显示
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void StopShow(){ Show = false; }
	/********************************************************
		*  @brief    :	转换显示状态
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void SwitchShow(){ Show = !Show; }
	/********************************************************
		*  @brief    :	返回精灵的显示状态
		*  @input	 :
		*  @output	 :
		*  @return   :  bool
		*  @author   :	Alex
		********************************************************/
	bool getShowState(){ return Show; }
	/********************************************************
		*  @brief    :	渲染精灵
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	
		********************************************************/
	void draw();
	/********************************************************
		*  @brief    :	渲染部分纹理
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void drawWithPartTexture(D3DXVECTOR3 p,Size s);
	/********************************************************
		*  @brief    :	加载纹理
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void LoadTexture(Texture* texture);
	/********************************************************
	*  @brief    :	让精灵执行动作
	*  @input	 :
	*  @output	 :
	*  @return   :
	*  @author   :	Alex
	********************************************************/
	void runAction(Action*);
	/********************************************************
		*  @brief    :	加载精灵纹理
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void LoadTexture(LPCTSTR path, UINT width, UINT height, D3DCOLOR color);
	/********************************************************
		*  @brief    :	得到D3D精灵
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	LPD3DXSPRITE getSprite();
	/********************************************************
		*  @brief    :	设置精灵所在的场景
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setScene(Scene* scene);
	/********************************************************
		*  @brief    :	得到精灵所在的场景
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	Scene* getScene(){ return theScene; }
	/********************************************************
		*  @brief    :	设置精灵的碰撞矩形
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setRet(Ret m_ret);
	/********************************************************
		*  @brief    :	得到精灵的碰撞矩形
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	Ret getRet(){ return ret; }
	/********************************************************
		*  @brief    :	得到精灵的中心
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	D3DXVECTOR3 getCenter(){ return m_Center; }
	/********************************************************
		*  @brief    :	设置该精灵是否可以移动
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setMoveState(bool move){ Moveable = move; }
	/********************************************************
		*  @brief    :	得到该精灵的移动状态
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool getMoveState(){ return Moveable; }
	/********************************************************
		*  @brief    :	设置该精灵的动作
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setAction(Action*);
protected:
	RenderRet* TheRenderRet;
	LPD3DXSPRITE m_D3DSprite;
	Texture* m_Texture;
	Ret ret;//精灵矩形
	D3DXVECTOR3 m_position;//统一以左上角为坐标原点
	bool Show;
	D3DXVECTOR3 m_Center;
	Scene* theScene;
	bool Moveable;
	Action* theAction;
};

/**************************************************************************
*  @file	 : Sprite.h
*  @version  : ver  1.0
*  @author   : Alex
*  @date     : 
*  @brief    : 精灵数组类
**************************************************************************/
class SpriteArray
{
private:
	SpriteArray(){}
	vector<Sprite*> Spritearray;
	int SpriteNumber;
public:
	/********************************************************
		*  @brief    :	得到精灵数组的单例
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	static SpriteArray* getInstance();
	/********************************************************
		*  @brief    :	添加精灵
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void addSprite(Sprite*);
	/********************************************************
		*  @brief    :	得到精灵数组
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	vector<Sprite*> getSprite();
	/********************************************************
		*  @brief    :	得到精灵数量
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int getSpriteNumber();
	/********************************************************
		*  @brief    :	移除精灵
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void removeSprite(Sprite* sp);
};

#endif