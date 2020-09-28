#include "ActionManager.h"
#include "common.h"

ActionManager* ActionManager::getInstance()
{
	static ActionManager* Instance;
	if (Instance == NULL)
	{
		Instance = new ActionManager();
	}
	return Instance;
}

void ActionManager::setTarget(Sprite* target, Action* action)
{
	action->setTarget(target);
	action->actionStart();
}

void ActionManager::Ac()
{
	ActionArray::getInstance()->visitAll();
}