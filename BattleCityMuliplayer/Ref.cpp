#include "Ref.h"

bool Ref::init()
{
	tain = 0;
	enabled = true;
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

bool Ref::IsEnabled()
{
	return enabled;
}

void Ref::SetEnabled(bool _enabled)
{
	enabled = _enabled;
}

Ref::~Ref()
{
	AutoRef::getInstance()->removeRef(this);
}