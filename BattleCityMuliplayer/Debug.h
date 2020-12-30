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
	D3DXVECTOR3 debugPosition;
	Speed debugSpeed;
	D3DXVECTOR3 debugDirection;

public:

	Debug();
	~Debug();

	void SetDebugPositionX(int x);
	void SetDebugPositionY(int y);
	D3DXVECTOR3 GetDebugPosition();
	void SetDebugDirectionX(int x);
	void SetDebugDirectionY(int y);



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