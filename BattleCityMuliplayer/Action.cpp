#include "Action.h"
#include "Sprite.h"

void Action::setTarget(Sprite* target)
{
	Target = target;
	On = true;
}

void ActionArray::pushAction(Action* action)
{
	m_Array.push_back(action);
 	actionNumber++;
}

void ActionArray::visitAll()
{
	for (int i = 0; i < actionNumber;i++)
	{
		if (m_Array[i]->getOn())
		{
			m_Array[i]->Update();
		}
	}
}

void ActionArray::removeAction(Action* ac)
{
	for (int i = 0; i < actionNumber;i++)
	{
		if (ac == m_Array[i])
		{
			m_Array.erase(m_Array.begin() + i);
			actionNumber--;
			return;
		}
	}
}

Action::~Action()
{
	ActionArray::getInstance()->removeAction(this);
	Target->setAction(NULL);
}

ActionArray* ActionArray::getInstance()
{
	static ActionArray* acArray;
	if (acArray == NULL)
	{
		acArray = new ActionArray();
	}
	return acArray;
}

void MoveBySpeed::Update()
{
}

void MoveBySpeed::setSpeed(Speed speed)
{
	MoveBySpeed::speed = speed;
}

MoveBySpeed* MoveBySpeed::create(Speed speed /* = Speed(0,0) */)
{
	MoveBySpeed* move = new MoveBySpeed();
	move->setSpeed(speed);
	move->On = false;
	ActionArray::getInstance()->pushAction(move);
	if (move)
	{
		move->autorelease();
		return move;
	}
	return NULL;
}

void Blink::setTime(int t)
{
	m_time = t;
}

void Blink::setDuration(int d)
{
	m_duration = d;
}

Blink* Blink::create(int duration /* = 0 */,int blinks)
{
	Blink* blink = new Blink();
	blink->setTime(blinks);
	blink->setDuration(duration);
	blink->On = false;
	ActionArray::getInstance()->pushAction(blink);
	blink->m_counter = 0;
	if (blink)
	{
		blink->autorelease();
		return blink;
	}
	return NULL;
}

void Blink::Update()
{
	if (m_counter>=(m_time*FRAME))
	{
		Target->SwitchShow();
		m_counter = 0;
		return ;
	}
	m_counter++;
}

Stop* Stop::create(int t /* = 1 */)
{
	Stop* stop = new Stop();
	stop->setTime(t);
	stop->m_Timer = 0;
	stop->On = false;
	stop->autorelease();
	ActionArray::getInstance()->pushAction(stop);
	return stop;
}

void Stop::setTime(int time)
{
	m_Time = time;
}

void Stop::Update()
{
	m_Timer++;
	if (m_Timer<m_Time*FRAME)
	{
		Target->setMoveState(false);
	}
	if (m_Timer>=m_Time*FRAME)
	{
		Target->setMoveState(true);
		actionStop();
	}
}