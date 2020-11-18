#pragma once
#include "../Point/Point.h"
#include "../Vector/Vector.h"

struct Fish
{
public:
	Point Position;
	Vector Direction;

	Fish(Point position, float direction = 0);
	Fish();

public:
	float TargetPoint_X(float direction);
	float TargetPoint_Y(float direction);

public:
	void Draw();
	void Move();
	void MoveTo(Point point);
	void MoveBy(Vector vector);
	void SteerToTheAverageHeadingOfLocalFlockmates(Fish* fishes, int count);
	void SteerToTheAveragePositionOfLocalFlockmates(Fish* fishes, int count);
	bool DetectColisionWithFlockmates(Fish* fishes, int count, Vector vector);
};


