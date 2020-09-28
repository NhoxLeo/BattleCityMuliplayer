#include "Ref.h"

bool Ref::init()
{
	tain = 0;
	return true;
}

Ref* Ref::autorelease()
{
	ReleaseManager::getInstance()->addRelease(this);
	return this;
}

void Ref::release()
{
	delete this;
}

int Ref::getTain()
{
	return tain;
}

Ref::~Ref()
{
	AutoRef::getInstance()->removeRef(this);
}