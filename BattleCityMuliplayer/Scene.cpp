#include "Scene.h"
#include "Map.h"
#include "StaticSprite.h"
#include "ActiveSprite.h"
#include "Award.h"

bool Scene::init()
{
	retain();
	nowAward = NULL;
	ActiveNumber = 0;
	StaticNumber = 0;
	return true;
}

void Scene::addActiveChild(ActiveSprite* child)
{
	child->OnShow();
	Active_children.push_back(child);
	ActiveNumber++;
	child->retain();
	child->setScene(this);
}

void Scene::addStaticChild(StaticSprite* child)
{
	child->OnShow();
	Static_children.push_back(child);
	StaticNumber++;
	child->retain();
	child->setScene(this);
}

void Scene::removeActiveChild(ActiveSprite* sp)
{
	for (int i = 0; i < ActiveNumber;i++)
	{
		if (sp == Active_children[i])
		{
			Active_children.erase(Active_children.begin() + i);
			ActiveNumber--;
			return;
		}
	}
}

void Scene::removeStaticChild(StaticSprite* sp)
{
	for (int i = 0; i < StaticNumber; i++)
	{
		if (sp == Static_children[i])
		{
			Static_children.erase(Static_children.begin() + i);
			StaticNumber--;
			return;
		}
	}
}


void Scene::DrawAllActive()
{
	for (int t = 0; t < ActiveNumber;t++)
	{
		if (Active_children[t]->getRenderRet()==NULL)
		{
			Active_children[t]->draw();
		}
		else
		{
			Active_children[t]->drawWithPartTexture(*Active_children[t]->getRenderRet()->RenderPoint, *Active_children[t]->getRenderRet()->RenderSize);
		}
	}
}

void Scene::DrawAllStatic()
{
	for (int t = 0; t < StaticNumber; t++)
	{
		if (Static_children[t]->getRenderRet() == NULL)
		{
			Static_children[t]->draw();
		}
		else
		{
			Static_children[t]->drawWithPartTexture(*Static_children[t]->getRenderRet()->RenderPoint, *Static_children[t]->getRenderRet()->RenderSize);
		}
	}
	if (nowAward != NULL)
	{
		nowAward->drawWithPartTexture(*nowAward->getRenderRet()->RenderPoint, *nowAward->getRenderRet()->RenderSize);
	}
}

Scene::~Scene()
{
	for (int t = 0; t < StaticNumber; t++)
	{
		Static_children[t]->release();
	}
	for (int t = 0; t < ActiveNumber;t++)
	{
		Active_children[t]->release();
	}
}

void Scene::homeBoom()
{
	home[0]->setRenderRet(32, 32, 192, 0);
	home[1]->setRenderRet(32, 32, 208, 0);
	home[2]->setRenderRet(32, 32, 192, 16);
	home[3]->setRenderRet(32, 32, 208, 16);
	win = false;
}

void Scene::setAward()
{
	if (nowAward != NULL)
	{
		nowAward->detain();
	}
	nowAward = Award::create();
	nowAward->retain();
	nowAward->OnShow();
	nowAward->setScene(this);
}