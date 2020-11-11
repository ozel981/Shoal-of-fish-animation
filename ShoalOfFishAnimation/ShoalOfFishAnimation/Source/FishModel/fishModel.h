#pragma once


struct FishModel
{
public:
	int X, Y; // fish position
	int degree = 0; // 0-up 180-down 90-left 270-right 

	FishModel(int x, int y);

public:
	int x_position(int x, int y);
	int y_position(int x, int y);

public:
	void Draw();
	void Move(int dist, int min_y, int max_y);
	void SetDegree(int degree);

};


