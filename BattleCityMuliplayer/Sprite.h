/**************************************************************************
*  @file	 : Sprite.h
*  @version  : ver  1.0
*  @author   : 
*  @date     : 
*  @brief    : ������
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
*  @brief    : ��Ⱦ���νṹ��
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
		*  @brief    :	������Ⱦ�ľ�������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setRenderRet(int height,int width,int x,int y);
	/********************************************************
		*  @brief    :	�õ���Ⱦ������
		*  @input	 :
		*  @output	 :
		*  @return   :  RenderRet*
		*  @author   :	Alex
		********************************************************/
	RenderRet* getRenderRet();
	/********************************************************
		*  @brief    :	�������ʼ��
		*  @input	 :
		*  @output	 :
		*  @return   :  bool
		*  @author   :	Alex
		********************************************************/
	bool init();
	/********************************************************
		*  @brief    :	���þ��������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setCenter(D3DXVECTOR3 center){ m_Center = center; }
	/********************************************************
		*  @brief    :	���þ����λ�ã�����ʱ����¾�����ε�λ�ã�
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setPosition(D3DXVECTOR3 position);
	/********************************************************
		*  @brief    :	��þ����λ��
		*  @input	 :
		*  @output	 :
		*  @return   :  D3DXVECTOR3
		*  @author   :	Alex
		********************************************************/
	D3DXVECTOR3 getPosition();
	/********************************************************
		*  @brief    :	��þ���ĸ߶�
		*  @input	 :
		*  @output	 :
		*  @return   :  int
		*  @author   :	Alex
		********************************************************/
	int getHeight();
	/********************************************************
		*  @brief    :	��þ���Ŀ��
		*  @input	 :
		*  @output	 :
		*  @return   :  int
		*  @author   :	Alex
		********************************************************/
	int getwidth();
	/********************************************************
		*  @brief    :	��þ���Ĵ�С
		*  @input	 :
		*  @output	 :
		*  @return   :  Size
		*  @author   :	Alex
		********************************************************/
	Size getSize();
	/********************************************************
		*  @brief    :	�þ�����ʾ
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void OnShow(){ Show = true; }
	/********************************************************
		*  @brief    :	�þ���ֹͣ��ʾ
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void StopShow(){ Show = false; }
	/********************************************************
		*  @brief    :	ת����ʾ״̬
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void SwitchShow(){ Show = !Show; }
	/********************************************************
		*  @brief    :	���ؾ������ʾ״̬
		*  @input	 :
		*  @output	 :
		*  @return   :  bool
		*  @author   :	Alex
		********************************************************/
	bool getShowState(){ return Show; }
	/********************************************************
		*  @brief    :	��Ⱦ����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	
		********************************************************/
	void draw();
	/********************************************************
		*  @brief    :	��Ⱦ��������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void drawWithPartTexture(D3DXVECTOR3 p,Size s);
	/********************************************************
		*  @brief    :	��������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void LoadTexture(Texture* texture);
	/********************************************************
	*  @brief    :	�þ���ִ�ж���
	*  @input	 :
	*  @output	 :
	*  @return   :
	*  @author   :	Alex
	********************************************************/
	void runAction(Action*);
	/********************************************************
		*  @brief    :	���ؾ�������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void LoadTexture(LPCTSTR path, UINT width, UINT height, D3DCOLOR color);
	/********************************************************
		*  @brief    :	�õ�D3D����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	LPD3DXSPRITE getSprite();
	/********************************************************
		*  @brief    :	���þ������ڵĳ���
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setScene(Scene* scene);
	/********************************************************
		*  @brief    :	�õ��������ڵĳ���
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	Scene* getScene(){ return theScene; }
	/********************************************************
		*  @brief    :	���þ������ײ����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setRet(Ret m_ret);
	/********************************************************
		*  @brief    :	�õ��������ײ����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	Ret getRet(){ return ret; }
	/********************************************************
		*  @brief    :	�õ����������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	D3DXVECTOR3 getCenter(){ return m_Center; }
	/********************************************************
		*  @brief    :	���øþ����Ƿ�����ƶ�
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void setMoveState(bool move){ Moveable = move; }
	/********************************************************
		*  @brief    :	�õ��þ�����ƶ�״̬
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	bool getMoveState(){ return Moveable; }
	/********************************************************
		*  @brief    :	���øþ���Ķ���
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
	Ret ret;//�������
	D3DXVECTOR3 m_position;//ͳһ�����Ͻ�Ϊ����ԭ��
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
*  @brief    : ����������
**************************************************************************/
class SpriteArray
{
private:
	SpriteArray(){}
	vector<Sprite*> Spritearray;
	int SpriteNumber;
public:
	/********************************************************
		*  @brief    :	�õ���������ĵ���
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	static SpriteArray* getInstance();
	/********************************************************
		*  @brief    :	��Ӿ���
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void addSprite(Sprite*);
	/********************************************************
		*  @brief    :	�õ���������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	vector<Sprite*> getSprite();
	/********************************************************
		*  @brief    :	�õ���������
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	int getSpriteNumber();
	/********************************************************
		*  @brief    :	�Ƴ�����
		*  @input	 :
		*  @output	 :
		*  @return   :  
		*  @author   :	Alex
		********************************************************/
	void removeSprite(Sprite* sp);
};

#endif