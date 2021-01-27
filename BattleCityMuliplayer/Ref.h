
#ifndef REF_H
#define REF_H

#include "ReleaseManager.h"
#include "common.h"

struct Speed
{
	Speed(){ x = 0; y = 0; }
	Speed(double X, double Y){ x = X; y = Y; }
	double x;
	double y;
};

class Ref
{
protected:
	int tain;
	bool enabled = true;
public:

	virtual bool init();

	Ref* autorelease();

	void release();

	CREATE_FUNC(Ref);

	int getTain();
	bool IsEnabled();
	void SetEnabled(bool _enabled);

	void retain(){ tain++; }
	void detain(){ tain = 0; }
	virtual ~Ref();
};

#endif