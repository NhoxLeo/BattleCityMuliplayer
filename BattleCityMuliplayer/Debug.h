#include <iostream> 
#include <fstream> 
#include <iostream> 
#include "Tank.h"
//#include <minwindef.h>

using namespace std;

class Debug 
{

	
public:

	Debug();
	~Debug();

	void Log(int i);
	//void Log(UINT i);
	void LogTank(Tank* tank);

};
