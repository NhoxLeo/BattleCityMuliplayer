
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
public:

	virtual bool init();

	Ref* autorelease();

	void release();

	CREATE_FUNC(Ref);

	int getTain();

	void retain(){ tain++; }
	void detain(){ tain = 0; }
	virtual ~Ref();
};

#endif