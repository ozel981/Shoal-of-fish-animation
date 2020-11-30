#pragma once
#include <time.h>

#include "../Constant/Constant.h"
#include "../Fish/Fish.h"



class AnimationWindow
{
public:
	bool isCPUx = true;

	Fish FishShol[FISH_COUNT];
public:
	AnimationWindow(bool isCPU);
	~AnimationWindow();
	bool IsCPU() { return isCPUx; }
	void Run();

	#pragma region Friend functions
	friend void Display();
	friend void Timer(int);
	friend void Init();
	friend void Reshape(int, int);
	friend void KeyboardInput(unsigned char, int, int);
	#pragma endregion
};