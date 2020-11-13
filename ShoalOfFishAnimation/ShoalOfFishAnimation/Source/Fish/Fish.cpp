#include"Fish.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <algorithm> 
#include <math.h>
#include "../Constant/Costant.h"

int TopAndBottomLimit(float, float, float);

Fish::Fish(float x, float y, float direction)
{
	this->X = x;
	this->Y = y;
	this->direction = direction;
	this->target_X = TargetPoint_X();
	this->target_Y = TargetPoint_Y();
}
Fish::Fish()
{ 
	this->X = 0;
	this->Y = 0;
	this->direction = 0;
	this->target_X = TargetPoint_X();
	this->target_Y = TargetPoint_Y();
}

float Fish::NewPositonDependsOnDirection_X(float x, float y)
{
	return ((x-X)*cos(DegreeToRadians(this->direction)) - (y-Y)*sin(DegreeToRadians(this->direction))) + X;
}

float Fish::NewPositonDependsOnDirection_Y(float x, float y)
{
	return ((x-X)*sin(DegreeToRadians(this->direction)) + (y-Y)*cos(DegreeToRadians(this->direction))) + Y;
}

void Fish::Draw()
{
	glBegin(GL_TRIANGLES);
	// fish front point
	glVertex2f(NewPositonDependsOnDirection_X(X, Y + 12), NewPositonDependsOnDirection_Y(X, Y + 12));
	// fish rear left point
	glVertex2f(NewPositonDependsOnDirection_X(X - 6, Y - 4), NewPositonDependsOnDirection_Y(X - 6, Y - 4));
	// fish rear right point
	glVertex2f(NewPositonDependsOnDirection_X(X + 6, Y - 4), NewPositonDependsOnDirection_Y(X + 6, Y - 4));
	glEnd();
}

void Fish::Move()
{
	if (X > MATRIX_HALF_WIDTH || X < -MATRIX_HALF_WIDTH || Y > MATRIX_HALF_HEIGHT || Y < -MATRIX_HALF_HEIGHT) return;
	float dx = target_X - X;
	float dy = target_Y - Y;
	float l = sqrt(dx * dx + dy * dy);
	dx = dx / l;
	dy = dy / l;
	X += dx;
	Y += dy;
}

void Fish::SetDirection(float degree)
{
	if (direction == degree) return;
	if (degree >= 360)
	{
		this->direction = degree - 360.0;
	}
	else
	{
		this->direction = degree;
	}
	target_X = TargetPoint_X();
	target_Y = TargetPoint_Y();
}

float Fish::GetDirection()
{
	return direction;
}

int Fish::ReactToNearbyFishes(Fish* fishes, int count)
{
	float avarageDirection = direction <= 180 ? direction : direction - 360;
	int n = 1;
	float avarage_X = X;
	float avarage_Y = Y;
	float avarageDiversion_X = 0;
	float avarageDiversion_Y = 0;
	for (int i = 0; i < count; i++)
	{
		float x = fishes[i].X;
		float y = fishes[i].Y;
		if (x == X && y == Y) continue;
		if (sqrt((X - x)*(X - x) + (Y - y)*(Y - y)) < FISH_VIEW_RANGE)
		{
			//avarageDiversion_X += X - x;
			//avarageDiversion_X += X - x;
			float a_x = (X - x);
			float a_y = (Y - y);
			float l = sqrt(a_x*a_x + a_y * a_y);
			a_x /= l;
			a_y /= l;
			a_x *= (FISH_VIEW_RANGE - l);
			a_y *= (FISH_VIEW_RANGE - l);
			avarageDiversion_X += a_x;
			avarageDiversion_Y += a_y;
			avarageDirection += (fishes[i].GetDirection() <= 180 ? fishes[i].GetDirection() : fishes[i].GetDirection() - 360);
			n++;
			avarage_X += x;
			avarage_Y += y;
		}
	}
	if (n > 1)
	{
		avarage_X /= n;
		avarage_Y /= n;
		float midPointDirection = GetDirectionToPoint(avarage_X, avarage_Y);
		midPointDirection = midPointDirection <= 180 ? midPointDirection : midPointDirection - 360;
		
		avarageDiversion_X /= (n - 1);
		avarageDiversion_Y /= (n - 1);
		float antDiversionDirecion = GetDirectionToPoint(X + avarageDiversion_X, Y + avarageDiversion_Y);
		antDiversionDirecion = antDiversionDirecion <= 180 ? antDiversionDirecion : antDiversionDirecion - 360;

		//avarageDirection /= (n);
		avarageDirection += midPointDirection;
		avarageDirection += antDiversionDirecion;
		avarageDirection /= (n + 2);
		if (avarageDirection < 0) return 360 + avarageDirection;
		else return avarageDirection;
	}
	return direction;
}

void Fish::Uloada()
{
	if (uploada)SetDirection(GetDirectionToPoint(d_x, d_y));
	uploada = false;
}

float Fish::GetDirectionToPoint(float x, float y)
{
	if ( Y - y == 0)
	{
		if (x > X) return 270;
		else return 90;
	}
	if (round(X - x) == 0)
	{
		if (y > Y) return 0;
		else return 180;
	}
	if (x > X && y > Y)
	{
		return 360 - RadiansToDegree(atan((x - X) / (y - Y)));
	}
	else if (x <= X && y > Y)
	{
		return -RadiansToDegree(atan((x - X) / (y - Y)));
	}
	else if (x <= X && y <= Y)
	{
		return 180 - RadiansToDegree(atan((x - X) / (y - Y)));
	}
	else
	{
		return 180 - RadiansToDegree(atan((x - X) / (y - Y)));
	}
}

float Fish::TargetPoint_X()
{
	if (direction == 90)
	{
		return -MATRIX_HALF_WIDTH;
	}
	if (direction == 270)
	{
		return MATRIX_HALF_WIDTH;
	}
	if (direction >= 0 && direction < 90)
	{
		return std::max(-MATRIX_HALF_WIDTH,X - (MATRIX_HALF_HEIGHT - Y)*tan(DegreeToRadians(direction)));
	}
	if (direction >= 90 && direction < 180)
	{
		return std::max(-MATRIX_HALF_WIDTH, X - (Y + MATRIX_HALF_HEIGHT)*-tan(DegreeToRadians(direction)));
	}
	if (direction >= 180 && direction < 270)
	{
		return std::min(MATRIX_HALF_WIDTH, X + (Y + MATRIX_HALF_HEIGHT)*tan(DegreeToRadians(direction)));
	}
	if (direction >= 270 && direction < 360)
	{
		return std::min(MATRIX_HALF_WIDTH, X + (MATRIX_HALF_HEIGHT - Y)*-tan(DegreeToRadians(direction)));
	}
	return 0;
}

float Fish::TargetPoint_Y()
{
	if (direction == 90 || direction == 270)
	{
		return Y;
	}
	if (direction == 0) return MATRIX_HALF_HEIGHT;
	if (direction == 180) return -MATRIX_HALF_HEIGHT;
	if (direction >= 0 && direction < 90)
	{
		return std::min(MATRIX_HALF_HEIGHT, Y + (X + MATRIX_HALF_WIDTH)*tan(DegreeToRadians((90.0-direction))));
	}
	if (direction >= 90 && direction < 180)
	{
		return std::max(-MATRIX_HALF_HEIGHT, Y + (X + MATRIX_HALF_WIDTH)*tan(DegreeToRadians((90.0 - direction))));
	}
	if (direction >= 180 && direction < 270)
	{
		return std::max(-MATRIX_HALF_HEIGHT, Y - (MATRIX_HALF_WIDTH - X)*tan(DegreeToRadians((90.0-direction))));
	}
	if (direction >= 270 && direction < 360)
	{
		return std::min(MATRIX_HALF_HEIGHT, Y + (MATRIX_HALF_WIDTH - X)*-tan(DegreeToRadians((90.0 - direction))));
	}
	return 0;
}

int TopAndBottomLimit(float value, float topLimit, float downLimit)
{
	return std::max(downLimit, std::min(topLimit, value));
}
