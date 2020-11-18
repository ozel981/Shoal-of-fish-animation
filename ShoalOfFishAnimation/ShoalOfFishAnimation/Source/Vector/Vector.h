#pragma once
#include "../Point/Point.h"

struct Vector
{
	float X;
	float Y;

	Vector(float x = 0, float y = 0);

	#pragma region Operators
	Vector operator +(Vector vector);
	Vector operator -(Vector vector);
	Vector operator *(float a);
	Vector operator /(float a);
	void operator +=(Vector vector);
	void operator -=(Vector vector);
	void operator *=(float a);
	void operator /=(float a);
	void operator = (Vector vector);
	#pragma endregion

	float Length();

	void Normalize();
	Vector Normalized();
};