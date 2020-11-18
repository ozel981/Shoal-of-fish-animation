#pragma once
#include <time.h>

#include "../Constant/Costant.h"
#include "../Fish/Fish.h"

class AnimationWindow
{
private:
	bool isCPU = true;

	Fish FishShol[FISH_COUNT];
public:
	AnimationWindow(bool isCPU = true);
	
	void Run();

	#pragma region Friend functions
	friend void Display();
	friend void Timer(int);
	friend void Init();
	friend void Reshape(int, int);
	friend void KeyboardInput(unsigned char, int, int);
	#pragma endregion
};
