#pragma once
#include "../Point/Point.h"

struct Vector
{
	float X;
	float Y;

	Vector(float x = 0, float y = 0);

	Vector operator +(Vector point);
	Vector operator -(Vector point);

	void Normalize();
	Vector Normalized();
};