#pragma once
#include "../Point/Point.h"
#include "../Vector/Vector.h"

struct Fish
{
private:
	Point Position;
public:
	Vector Direction;
	Fish(Point position, float direction = 0);
	Fish();

private:
	float TargetPoint_X(float direction);
	float TargetPoint_Y(float direction);

public:
	void Draw();

	#pragma region Fish moving
	void Move();
	void MoveTo(Point point);
	void MoveBy(Vector vector);
	#pragma endregion

	#pragma region Fish steering
	Vector VectorToTheAverageHeadingOfLocalFlockmates(Fish* fishes, int count);
	Vector VectorToTheAveragePositionOfLocalFlockmates(Fish* fishes, int count);
	Vector VectorToAvoidCrowdingLocalFlockmates(Fish* fishes, int count);
	#pragma endregion
};


