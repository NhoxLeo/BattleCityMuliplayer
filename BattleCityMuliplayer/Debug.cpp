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

//void Debug::Log(UINT i)
//{
//	ofstream debug("Log.txt");
//	debug << i;
//	debug.close();
//}