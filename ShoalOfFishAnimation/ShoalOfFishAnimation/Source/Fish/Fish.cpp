#include"Fish.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <algorithm> 
#include <math.h>
#include "../Constant/Costant.h"

int TopAndBottomLimit(float, float, float);

#pragma region Constructors


Fish::Fish(Point position, float direction)
{
	this->Position = position;
	this->direction = direction;
	this->Direction = Vector(TargetPoint_X(), TargetPoint_Y());
}
Fish::Fish()
{
	Position = Point();
	this->direction = 0;
	this->Direction = Vector(TargetPoint_X(), TargetPoint_Y());
}


#pragma endregion

#pragma region Move Functions

void Fish::Move()
{
	if (Position.X > MATRIX_HALF_WIDTH || Position.X < -MATRIX_HALF_WIDTH || Position.Y > MATRIX_HALF_HEIGHT || Position.Y < -MATRIX_HALF_HEIGHT) return;
	Vector versorDirection = Direction.Normalized();
	Position.X += versorDirection.X;
	Position.Y += versorDirection.Y;
}

void Fish::MoveBy(Vector vector)
{
	if (Position.X > MATRIX_HALF_WIDTH || Position.X < -MATRIX_HALF_WIDTH || Position.Y > MATRIX_HALF_HEIGHT || Position.Y < -MATRIX_HALF_HEIGHT) return;
	Position.X += vector.X;
	Position.Y += vector.Y;
}

#pragma endregion

#pragma region Draw

void DrawCircle(float cx, float cy, float r) {
	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < 10000; ii++) {
		float theta = 2.0f * 3.1415926f * float(ii) / float(10000);//get the current angle 
		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 
		glVertex2f(x + cx, y + cy);//output vertex 
	}
	glEnd();
}

void DrawCircleRangle(float cx, float cy, float r) {
	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < 10000; ii++) {
		float theta = 2.0f * 3.1415926f * float(ii) / float(10000);//get the current angle 
		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 
		glVertex2f(x + cx, y + cy);//output vertex 
	}
	glEnd();
}

void Fish::Draw()
{
	glBegin(GL_TRIANGLES);
	// fish front point
	glVertex2f(NewPositonDependsOnDirection_X(Point(Position.X, Position.Y + 12)), NewPositonDependsOnDirection_Y(Point(Position.X, Position.Y + 12)));
	// fish rear left point
	glVertex2f(NewPositonDependsOnDirection_X(Point(Position.X - 6, Position.Y - 4)), NewPositonDependsOnDirection_Y(Point(Position.X - 6, Position.Y - 4)));
	// fish rear right point
	glVertex2f(NewPositonDependsOnDirection_X(Point(Position.X + 6, Position.Y - 4)), NewPositonDependsOnDirection_Y(Point(Position.X + 6, Position.Y - 4)));
	glEnd();
	DrawCircle(Position.X, Position.Y, FISH_VIEW_RANGE);
	DrawCircleRangle(Position.X, Position.Y, FISH_COLISION_RANGE);
}

#pragma endregion

#pragma region Directions

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
	this->Direction = Vector(TargetPoint_X(), TargetPoint_Y());
}

float Fish::GetDirection()
{
	return direction;
}


float Fish::GetDirectionToPoint(Point point)
{
	if (Position.Y - point.Y == 0)
	{
		if (point.X > Position.X) return 270;
		else return 90;
	}
	if (round(Position.X - point.X) == 0)
	{
		if (point.Y > Position.Y) return 0;
		else return 180;
	}
	if (point.X > Position.X && point.Y > Position.Y)
	{
		return 360 - RadiansToDegree(atan((point.X - Position.X) / (point.X - Position.Y)));
	}
	else if (point.X <= Position.X && point.Y > Position.Y)
	{
		return -RadiansToDegree(atan((point.X - Position.X) / (point.Y - Position.Y)));
	}
	else if (point.X <= Position.X && point.Y <= Position.Y)
	{
		return 180 - RadiansToDegree(atan((point.X - Position.X) / (point.Y - Position.Y)));
	}
	else
	{
		return 180 - RadiansToDegree(atan((point.X - Position.X) / (point.Y - Position.Y)));
	}
}

#pragma endregion

#pragma region Steering Fish

float Fish::SteerTowardsTheAverageHeadingOfLocalFlockmates(Fish* fishes, int count)
{
	float avarageDirection = direction <= 180 ? direction : direction - 360;
	int n = 1;
	for (int i = 0; i < count; i++)
	{
		float x = fishes[i].Position.X;
		float y = fishes[i].Position.Y;
		if (x == Position.X && y == Position.Y) continue;
		if (sqrt((Position.X - x)*(Position.X - x) + (Position.Y - y)*(Position.Y - y)) < FISH_VIEW_RANGE)
		{
			avarageDirection += (fishes[i].GetDirection() <= 180 ? fishes[i].GetDirection() : fishes[i].GetDirection() - 360);
			n++;
		}
	}
	if (n > 1)
	{
		avarageDirection /= n;
		if (avarageDirection < 0) return 360 + avarageDirection;
		else return avarageDirection;
	}
	return direction;
}

Vector Fish::VectorToTheAveragePositionOfLocalFlockmates(Fish* fishes, int count)
{
	Point avaragePosition = Point(Position.X, Position.Y);
	int n = 1;
	for (int i = 0; i < count; i++)
	{
		float x = fishes[i].Position.X;
		float y = fishes[i].Position.Y;
		if (x == Position.X && y == Position.Y) continue;
		if (sqrt((Position.X - x)*(Position.X - x) + (Position.Y - y)*(Position.Y - y)) < FISH_VIEW_RANGE)
		{
			n++;
			avaragePosition += fishes[i].Position;
		}
	}
	avaragePosition /= n;
	if (abs(avaragePosition.X - Position.X) < 0.01 && abs(avaragePosition.Y - Position.Y) < 0.01) return Vector(0, 0);
	Vector directionToAvaragePosition = Vector(avaragePosition.X - Position.X, avaragePosition.Y - Position.Y);
	directionToAvaragePosition.Normalize();
	return directionToAvaragePosition;
}

bool Fish::DetectColisionWithFlockmates(Fish* fishes, int count, Vector vector)
{
	for (int i = 0; i < count; i++)
	{
		float x = fishes[i].Position.X;
		float y = fishes[i].Position.Y;		
		//if (x == Position.X && y == Position.Y) continue;
		if (abs(x - Position.X) < 0.001 && (y - Position.Y) < 0.001) continue;
		float new_x = Position.X + vector.X, new_y = Position.Y + vector.Y;
		if (sqrt((x - new_x)*(x - new_x) + (y - new_y)*(y - new_y)) < (FISH_COLISION_RANGE*2))
		{
			//printf("%f x %f -> %f x %f %f\n", Position.X, Position.Y, x, y, sqrt((Position.X - x)*(Position.X - x) + (Position.Y - y)*(Position.Y - y)));
			return false;
		}
	}
	return true;
}

#pragma endregion

#pragma region Position

float Fish::NewPositonDependsOnDirection_X(Point point)
{
	return ((point.X - Position.X)*cos(DegreeToRadians(this->direction)) - (point.Y - Position.Y)*sin(DegreeToRadians(this->direction))) + Position.X;
}

float Fish::NewPositonDependsOnDirection_Y(Point point)
{
	return ((point.X - Position.X)*sin(DegreeToRadians(this->direction)) + (point.Y - Position.Y)*cos(DegreeToRadians(this->direction))) + Position.Y;
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
		return std::max(-MATRIX_HALF_WIDTH, Position.X - (MATRIX_HALF_HEIGHT - Position.Y)*tan(DegreeToRadians(direction)));
	}
	if (direction >= 90 && direction < 180)
	{
		return std::max(-MATRIX_HALF_WIDTH, Position.X - (Position.Y + MATRIX_HALF_HEIGHT)*-tan(DegreeToRadians(direction)));
	}
	if (direction >= 180 && direction < 270)
	{
		return std::min(MATRIX_HALF_WIDTH, Position.X + (Position.Y + MATRIX_HALF_HEIGHT)*tan(DegreeToRadians(direction)));
	}
	if (direction >= 270 && direction < 360)
	{
		return std::min(MATRIX_HALF_WIDTH, Position.X + (MATRIX_HALF_HEIGHT - Position.Y)*-tan(DegreeToRadians(direction)));
	}
	return 0;
}

float Fish::TargetPoint_Y()
{
	if (direction == 90 || direction == 270)
	{
		return  Position.Y;
	}
	if (direction == 0) return MATRIX_HALF_HEIGHT;
	if (direction == 180) return -MATRIX_HALF_HEIGHT;
	if (direction >= 0 && direction < 90)
	{
		return std::min(MATRIX_HALF_HEIGHT, Position.Y + (Position.X + MATRIX_HALF_WIDTH)*tan(DegreeToRadians((90.0 - direction))));
	}
	if (direction >= 90 && direction < 180)
	{
		return std::max(-MATRIX_HALF_HEIGHT, Position.Y + (Position.X + MATRIX_HALF_WIDTH)*tan(DegreeToRadians((90.0 - direction))));
	}
	if (direction >= 180 && direction < 270)
	{
		return std::max(-MATRIX_HALF_HEIGHT, Position.Y - (MATRIX_HALF_WIDTH - Position.X)*tan(DegreeToRadians((90.0 - direction))));
	}
	if (direction >= 270 && direction < 360)
	{
		return std::min(MATRIX_HALF_HEIGHT, Position.Y + (MATRIX_HALF_WIDTH - Position.X)*-tan(DegreeToRadians((90.0 - direction))));
	}
	return 0;
}

#pragma endregion


int TopAndBottomLimit(float value, float topLimit, float downLimit)
{
	return std::max(downLimit, std::min(topLimit, value));
}
















