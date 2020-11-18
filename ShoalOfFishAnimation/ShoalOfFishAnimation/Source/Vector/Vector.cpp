#include "Vector.h"
#include <math.h>

Vector::Vector(float x, float y)
{
	this->X = x;
	this->Y = y;

}

void Vector::Normalize()
{
	float length = Length();
	X /= length;
	Y /= length;
}

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
	return Vector(X / a, Y / a);
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
	if (fabs(a) > 0.0001)
	{
		X /= a;
		Y /= a;
	}
}

Vector Vector::Normalized()
{
	float length = Length();
	float x = X / length;
	float y = Y / length;
	return Vector(x, y);
}

void Vector::operator=(Vector vector)
{
	X = vector.X;
	Y = vector.Y;
}

float Vector::Length()
{
	return sqrt(X*X + Y*Y);
}