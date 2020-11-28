#include "Vector.h"
#include <math.h>
#include <algorithm>


#pragma region Constructors
Vector::Vector(float x, float y)
{
	this->X = x;
	this->Y = y;
}
#pragma endregion

#pragma region Normalization

void Vector::Normalize()
{
	float length = Length();
	if (fabs(length) > std::numeric_limits<float>::epsilon())
	{
		X /= length;
		Y /= length;
	}
}

Vector Vector::Normalized()
{
	float length = Length();
	float x = 0;
	float y = 0;
	if (fabs(length) > std::numeric_limits<float>::epsilon())
	{
		x = X / length;
		y = Y / length;
	}
	return Vector(x, y);
}

#pragma endregion

#pragma region Operators

Vector Vector::operator+(Vector vector)
{
	return Vector(X + vector.X, Y + vector.Y);
}

Vector Vector::operator-(Vector vector)
{
	return Vector(X - vector.X, Y - vector.Y);
}

Vector Vector::operator*(float a)
{
	return Vector(X * a, Y * a);
}

Vector Vector::operator/(float a)
{
	if (a > std::numeric_limits<float>::epsilon())
	{
		return Vector(X / a, Y / a);
	}
	return Vector(X, Y);
}

void Vector::operator+=(Vector vector)
{
	X += vector.X;
	Y += vector.Y;
}

void Vector::operator-=(Vector vector)
{
	X -= vector.X;
	Y -= vector.Y;
}

void Vector::operator*=(float a)
{
	X *= a;
	Y *= a;
}

void Vector::operator/=(float a)
{
	if (fabs(a) > std::numeric_limits<float>::epsilon())
	{
		X /= a;
		Y /= a;
	}
}

void Vector::operator=(Vector vector)
{
	X = vector.X;
	Y = vector.Y;
}
#pragma endregion

float Vector::Length()
{
	return sqrt(X*X + Y*Y);
}
