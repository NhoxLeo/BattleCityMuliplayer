
#ifndef ACTION_H
#define ACTION_H

#include "common.h"
#include "Ref.h"

class Sprite;

struct Speed;

class Action:public Ref
{
protected:
	Sprite* Target;
	bool On;
public:

	virtual void Update() = 0;
	
	void setTarget(Sprite*);
	
	void actionStart(){ On = true; }

	void actionStop(){ On = false; tain = -1; }
	
	bool getOn(){ return On; }
	
	Sprite* getTarget(){ return Target; }
	~Action();
};


class ActionArray
{
	ActionArray(){};
	int actionNumber = 0;
	vector<Action*>  m_Array;
public:
	
	void removeAction(Action* ac);
	
	static ActionArray* getInstance();
	
	void pushAction(Action* action);
	
	void visitAll();

	vector<Action*> getAction(){ return m_Array; }

	int getNumber(){ return actionNumber; }
};

class MoveBySpeed:public Action
{
	Speed speed;
public:
	MoveBySpeed(){};
	void Update();
	void setSpeed(Speed speed);
	static MoveBySpeed* create(Speed speed = Speed(0,0));
};

class Blink:public Action
{
	int m_counter;
	int m_duration;
	int m_time;
public:
	
	void Update();
	
	void setTime(int t);
	
	void setDuration(int d);

	static Blink* create(int  duration = 1,int time = 1);
};


class Stop :public Action
{
	int m_Timer;
	int m_Time;
public:

	void Update();

	void setTime(int time);

	static Stop* create(int t = 1);
};

#endif