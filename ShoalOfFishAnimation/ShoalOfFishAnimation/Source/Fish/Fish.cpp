#include"Fish.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <algorithm> 
#include <math.h>
#include "../Constant/Costant.h"

int TopAndBottomLimit(float, float, float);

Fish::Fish(float x, float y)
{
	this->X = x;
	this->Y = y;
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
	
	for (int i = 0; i < 10; i++)
	{

		if (direction >= 0 && direction < 30) Y++;
		else if (direction >= 30 && direction < 60) { X--; Y++; }
		else if (direction >= 60 && direction < 120) { X--; }
		else if (direction >= 120 && direction < 150) { X--; Y--; }
		else if (direction >= 150 && direction < 210) { Y--; }
		else if (direction >= 210 && direction < 240) { Y--; X++; }
		else if (direction >= 240 && direction < 300) { X++; }
		else if (direction >= 300 && direction < 330) { X++; Y++; }
		else if (direction >= 330 && direction < 360) { Y++; }
	}



	printf("%f -> ", direction);
	direction = GetDirectionToPoint(TargetPoint_X(), TargetPoint_Y());
	printf("%f \n", direction);
}

void Fish::SetDegree(float direction)
{
	this->direction = ((int)direction % 360);
}

void Fish::ReactToNearbyFishes(Fish* fishes, int count)
{
	for (int i = 0; i < count; i++)
	{
		float x = fishes[i].X;
		float y = fishes[i].Y;
		if ((X - x)*(X - x) + (Y - y)*(Y - y) < 100);
		{
			// TODO : write reaction
		}
	}
}

float Fish::GetDirectionToPoint(float x, float y)
{
	if (Y - y == 0)
	{
		if (x > X) return 270;
		else return 90;
	}
	if (direction > 90 && direction < 270)
	{
		if (direction >= 0 && direction < 180)
		{
			return 180 - RadiansToDegree(atan((x - X) / (y - Y)));
		}
		else
		{
			return 180 - RadiansToDegree(atan((x - X) / (y - Y)));
		}
	}
	else
	{
		if (direction >= 0 && direction < 180)
		{
			return  -RadiansToDegree(atan((x - X) / (y - Y)));
		}
		else
		{
			return 360 - RadiansToDegree(atan((x - X) / (y - Y)));
		}

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
		//return tan(DegreeToRadians(direction));
		return std::max(-MATRIX_HALF_WIDTH, X - (Y + MATRIX_HALF_HEIGHT)*-tan(DegreeToRadians(direction)));
	}
	if (direction >= 180 && direction < 270)
	{
		return std::min(MATRIX_HALF_WIDTH, X + (Y + MATRIX_HALF_HEIGHT)*tan(DegreeToRadians(direction)));
	}
	if (direction >= 270 && direction < 360)
	{
		//return tan(DegreeToRadians(direction));
		return std::min(MATRIX_HALF_WIDTH, X + (MATRIX_HALF_HEIGHT - Y)*-tan(DegreeToRadians(direction)));
	}
	return 0;
	//return X-((tan(DegreeToRadians(direction))*(MATRIX_HALF_HEIGHT)) * (direction > 90 && direction < 270 ? -1 : 1));
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
		//return tan(DegreeToRadians((90.0 - direction)));
		return std::min(MATRIX_HALF_HEIGHT, Y + (X + MATRIX_HALF_WIDTH)*tan(DegreeToRadians((90.0-direction))));
	}
	if (direction >= 90 && direction < 180)
	{
		//return tan(DegreeToRadians((90.0 - direction)));
		return std::max(-MATRIX_HALF_HEIGHT, Y + (X + MATRIX_HALF_WIDTH)*tan(DegreeToRadians((90.0 - direction))));
	}
	if (direction >= 180 && direction < 270)
	{
		//return tan(DegreeToRadians((direction)));
		return std::max(-MATRIX_HALF_HEIGHT, Y - (MATRIX_HALF_WIDTH - X)*tan(DegreeToRadians((90.0-direction))));
	}
	if (direction >= 270 && direction < 360)
	{
		//return tan(DegreeToRadians((90.0 - direction)));
		return std::min(MATRIX_HALF_HEIGHT, Y + (MATRIX_HALF_WIDTH - X)*-tan(DegreeToRadians((90.0 - direction))));
	}
	return 0;
}

int TopAndBottomLimit(float value, float topLimit, float downLimit)
{
	return std::max(downLimit, std::min(topLimit, value));
}
