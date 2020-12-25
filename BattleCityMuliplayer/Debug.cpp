#include "Debug.h"

Debug::Debug()
{

}

Debug::~Debug()
{

}

void Debug::Log(int i)
{
	ofstream debug ("Log.txt");
	debug << i;
	debug.close();
}

void Debug::SetDebug(bool debug)
{
	isDebug = debug;
}
bool Debug::GetDebug()
{
	return isDebug;
}
//void Debug::Log(UINT i)
//{
//	ofstream debug("Log.txt");
//	debug << i;
//	debug.close();
//}

void Debug::LogTank(ofstream& debug,Tank* tank)
{
	//ofstream debug("Log.txt");

	debug << "Tank's x: ";
	debug << tank->getPosition().x;
	debug << "\n";
	debug << "Tank's y: ";
	debug << tank->getPosition().y;
	debug << "\n";
	debug << "Tank's speed.x: ";
	debug << tank->getSpeed().x;
	debug << "\n";
	debug << "Tank's speed.y: ";
	debug << tank->getSpeed().y;
	debug << "\n";
	debug << "Tank's direction.x: ";
	debug << tank->getDirection().x;
	debug << "\n";
	debug << "Tank's direction.y: ";
	debug << tank->getDirection().y;
	debug << "\n";
	debug.close();
}

void Debug::LogTank20(Tank* tank)
{
	for (int i = 0; i < 20; i++)
	{
		ofstream debug("Log" + to_string(i) + ".txt");
		debug << "Tank's x: ";
		debug << tank->getPosition().x;
		debug << "\n";
		debug << "Tank's y: ";
		debug << tank->getPosition().y;
		debug << "\n";
		debug << "Tank's speed.x: ";
		debug << tank->getSpeed().x;
		debug << "\n";
		debug << "Tank's speed.y: ";
		debug << tank->getSpeed().y;
		debug << "\n";
		debug << "Tank's direction.x: ";
		debug << tank->getDirection().x;
		debug << "\n";
		debug << "Tank's direction.y: ";
		debug << tank->getDirection().y;
		debug << "\n";
		debug.close();
	}
	
}