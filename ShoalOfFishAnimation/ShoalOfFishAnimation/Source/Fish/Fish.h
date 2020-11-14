#pragma once
#include "../Point/Point.h"
#include "../Vector/Vector.h"

struct Fish
{
private:
	float direction = 0; // 0-up 180-down 90-left 270-right 
public:
	Point Position;
	Vector Direction;
	float d_x, d_y;
	bool uploada = false;

	Fish(Point position, float direction = 0);
	Fish();

public:
	float NewPositonDependsOnDirection_X(Point point);
	float NewPositonDependsOnDirection_Y(Point point);
	float GetDirectionToPoint(Point point);
	float TargetPoint_X();
	float TargetPoint_Y();

public:
	void Draw();
	void Move();
	void MoveBy(Vector vector);
	void SetDirection(float degree);  // 0-up 180-down 90-left 270-right 
	float GetDirection(); // 0-up 180-down 90-left 270-right 
	float SteerTowardsTheAverageHeadingOfLocalFlockmates(Fish* fishes, int count);
	Vector VectorToTheAveragePositionOfLocalFlockmates(Fish* fishes, int count);
	bool DetectColisionWithFlockmates(Fish* fishes, int count, Vector vector);
};


