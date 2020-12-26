#include <iostream> 
#include <fstream> 
#include <iostream> 
#include "Tank.h"
//#include <minwindef.h>

using namespace std;

class Debug 
{
private: 
	bool isDebug;
	
public:

	Debug();
	~Debug();

	void SetDebug(bool debug);
	bool GetDebug();

	void Log(int i);
	//void Log(UINT i);
	void LogTank(ofstream& debug,Tank* tank);
	void LogTank20(Tank* tank);
};

class DebugArray
{
private:
	int debugNumber;
	vector<Debug*> debugArray;
public:

};