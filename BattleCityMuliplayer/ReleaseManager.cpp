#include "ReleaseManager.h"

ReleaseManager* ReleaseManager::getInstance()
{
	static ReleaseManager* Instance;
	if (Instance == NULL)
	{
		Instance = new ReleaseManager();
	}
	return Instance;
}

void ReleaseManager::addRelease(Ref* ref)
{
	AutoRef::getInstance()->addRef(ref);
}

void ReleaseManager::checkRelease()
{
	AutoRef::getInstance()->visitAll();
}