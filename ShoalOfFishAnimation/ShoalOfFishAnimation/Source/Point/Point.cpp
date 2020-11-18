#include "Point.h"
#include <math.h>

Point::Point(float x, float y)
{
	X = x;
	Y = y;
}

Point Point::operator+(Point point)
{
	return Point(X + point.X, Y + point.Y);
}

Point Point::operator-(Point point)
{
	return Point(X - point.X, Y - point.Y);
}

Point Point::operator*(float a)
{
	return Point(X*a, Y*a);
}

Point Point::operator/(float a)
{
	return Point(X/a, Y/a);
}

void Point::operator+=(Point point)
{
	X += point.X;
	Y += point.Y;
}

void Point::operator-=(Point point)
{
	X -= point.X;
	Y -= point.Y;
}

void Point::operator*=(float a)
{
	X *= a;
	Y *= a;
}

void Point::operator/=(float a)
{
	if (fabs(a) > 0.0001)
	{
		X /= a;
		Y /= a;
	}
}
