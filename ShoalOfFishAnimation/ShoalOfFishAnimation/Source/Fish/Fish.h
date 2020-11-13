#pragma once


struct Fish
{
private:
	float direction = 25; // 0-up 180-down 90-left 270-right 
public:
	float X, Y; // fish position	
	float target_X, target_Y;

	Fish(float position_x, float position_y, float direction = 0);

private:
	float NewPositonDependsOnDirection_X(float x, float y);
	float NewPositonDependsOnDirection_Y(float x, float y);
	float GetDirectionToPoint(float x, float y);
	float TargetPoint_X();
	float TargetPoint_Y();

public:
	void Draw();
	void Move();
	void SetDirection(float degree);  // 0-up 180-down 90-left 270-right 
	float GetDirection(); // 0-up 180-down 90-left 270-right 
	void ReactToNearbyFishes(Fish* fishes, int count);

};


