#include "Vector.h"
#include <math.h>

Vector::Vector(float x, float y)
{
	this->X = x;
	this->Y = y;

}

void Vector::Normalize()
{
	float length = sqrt(X*X + Y * Y);
	X /= length;
	Y /= length;
}

Vector Vector::operator+(Vector point)
{
	return Vector(X + point.X, Y + point.Y);
}

Vector Vector::operator-(Vector point)
{
	return Vector(X - point.X, Y - point.Y);
}

Vector Vector::operator*(float a)
{
	return Vector(X * a, Y * a);
}

Vector Vector::operator/(float a)
{
	return Vector(X / a, Y / a);
}

void Vector::operator+=(Vector point)
{
	X += point.X;
	Y += point.Y;
}

void Vector::operator-=(Vector point)
{
	X -= point.X;
	Y -= point.Y;
}

void Vector::operator*=(float a)
{
	X *= a;
	Y *= a;
}

void Vector::operator/=(float a)
{
	if (fabs(a) > 0.0001)
	{
		X /= a;
		Y /= a;
	}
}

Vector Vector::Normalized()
{
	float length = sqrt(X*X + Y * Y);
	float x = X / length;
	float y = Y / length;
	return Vector(x, y);
}