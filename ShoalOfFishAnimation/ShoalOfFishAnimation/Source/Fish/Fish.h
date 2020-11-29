#pragma once
#include <cuda_runtime.h>
#include <GL/glew.h>

#include "../Point/Point.h"
#include "../Vector/Vector.h"
#include "FishSettings.h"


class Fish
{
public:
	FishSettings Settings;

	Point Position;
	Vector Direction;

	Fish(Point position, float direction, FishSettings settings);
	Fish();

private:
	float TargetPoint_X(float direction);
	float TargetPoint_Y(float direction);
	void SetOwnColor();


public:
	virtual void Draw();

	#pragma region Fish moving
	void Move();
	void MoveTo(Point point);
	void MoveBy(Vector vector);
	#pragma endregion

	#pragma region Fish steering
	Vector VectorToTheAverageHeadingOfLocalFlockmates(Fish* fishes, int count);
	Vector VectorToTheAveragePositionOfLocalFlockmates(Fish* fishes, int count);
	Vector VectorToAvoidCrowdingLocalFlockmates(Fish* fishes, int count);
	Vector AvoidMouse(float x, float y);
	#pragma endregion

};




