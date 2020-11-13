#pragma once


struct Fish
{
public:
	float X, Y; // fish position
	float direction =  15; // 0-up 180-down 90-left 270-right 

	Fish(float position_x, float position_y);

public:
	float NewPositonDependsOnDirection_X(float x, float y);
	float NewPositonDependsOnDirection_Y(float x, float y);
	float GetDirectionToPoint(float x, float y);
	float TargetPoint_X();
	float TargetPoint_Y();

public:
	void Draw();
	void Move();
	void SetDegree(float degree);
	void ReactToNearbyFishes(Fish* fishes, int count);

};


