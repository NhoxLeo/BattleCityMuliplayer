#include "AutoRef.h"
#include "Ref.h"
#include "Bullet.h"
#include "Sprite.h"
#include "ActiveSprite.h"

int AutoRef::RefNumber = 0;
vector<Ref*> AutoRef::AutoReleaseRef;

AutoRef* AutoRef::getInstance()
{
	static AutoRef* autoref;
	if (autoref == NULL)
	{
		autoref = new AutoRef();
	}
	return autoref;
}

void AutoRef::addRef(Ref* ref)
{
	AutoReleaseRef.push_back(ref);
	RefNumber++;
}

void AutoRef::visitAll()
{
	int i = 0;
	for (; i < RefNumber;)
	{
		if (AutoReleaseRef[i]->getTain()<=0)
		{
			delete AutoReleaseRef[i];
			//AutoReleaseRef[i] = NULL;
 			//AutoReleaseRef.erase(AutoReleaseRef.begin() + i);
 			//RefNumber--;
		}
		else
		{
			i++;
		}
	}
}

void AutoRef::removeRef(Ref* ref)
{
	for (int i = 0; i < RefNumber;i++)
	{
		if (AutoReleaseRef[i] == ref)
		{
			AutoReleaseRef.erase(AutoReleaseRef.begin() + i);
			RefNumber--;
			break;
		}
	}
}