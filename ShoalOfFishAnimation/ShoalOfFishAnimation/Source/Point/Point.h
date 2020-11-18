#pragma once

struct Point
{
public:
	float X;
	float Y;

	Point(float x = 0, float y = 0);

	#pragma region Operations
	Point operator +(Point point);
	Point operator -(Point point);
	Point operator *(float a);
	Point operator /(float a);
	void operator +=(Point point);
	void operator -=(Point point);
	void operator *=(float a);
	void operator /=(float a);
	#pragma endregion

};