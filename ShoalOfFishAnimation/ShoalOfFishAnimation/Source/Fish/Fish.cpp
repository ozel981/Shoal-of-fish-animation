#include"Fish.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <algorithm> 
#include <math.h>
#include "../Constant/Costant.h"
#include "../Point/Point.h"


#pragma region Constructors


Fish::Fish(Point position, float direction)
{
	this->Position = Point(position.X,position.Y);
	this->Direction = Vector(TargetPoint_X(direction), TargetPoint_Y(direction)).Normalized();
}
Fish::Fish()
{
	Position = Point();
	this->Direction = Vector(TargetPoint_X(0), TargetPoint_Y(0)).Normalized();
}


#pragma endregion

#pragma region Move Functions

void Fish::MoveTo(Point point)
{
	Position.X = point.X;
	Position.Y = point.Y;
}

void Fish::Move()
{
	if (Position.X > MATRIX_HALF_WIDTH) MoveTo(Point(-MATRIX_HALF_WIDTH + 1, -Position.Y));
	if (Position.X < -MATRIX_HALF_WIDTH) MoveTo(Point(MATRIX_HALF_WIDTH - 1, -Position.Y));
	if (Position.Y > MATRIX_HALF_HEIGHT) MoveTo(Point(-Position.X, -MATRIX_HALF_HEIGHT + 1));
	if (Position.Y < -MATRIX_HALF_HEIGHT) MoveTo(Point(-Position.X, MATRIX_HALF_HEIGHT - 1));
	//if (Position.X > MATRIX_HALF_WIDTH || Position.X < -MATRIX_HALF_WIDTH || Position.Y > MATRIX_HALF_HEIGHT || Position.Y < -MATRIX_HALF_HEIGHT) return;
	Vector versorDirection = Direction.Normalized();
	Position.X += versorDirection.X;
	Position.Y += versorDirection.Y;
}

void Fish::MoveBy(Vector vector)
{
	if (Position.X > MATRIX_HALF_WIDTH) MoveTo(Point(-MATRIX_HALF_WIDTH, -Position.Y));
	if (Position.X < -MATRIX_HALF_WIDTH) MoveTo(Point(MATRIX_HALF_WIDTH, -Position.Y));
	if (Position.Y > MATRIX_HALF_HEIGHT) MoveTo(Point(-MATRIX_HALF_WIDTH, -Position.X));
	if (Position.Y < -MATRIX_HALF_WIDTH) MoveTo(Point(MATRIX_HALF_WIDTH, -Position.X));
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
	glVertex2f(Position.X + (Direction.X * FISH_FRON_LENGTH), Position.Y + (Direction.Y * FISH_FRON_LENGTH));
	// fish rear left point
	glVertex2f(Position.X  -Direction.Y * FISH_BACK_LENGTH, Position.Y +  Direction.X * FISH_BACK_LENGTH);
	// fish rear right point
	glVertex2f(Position.X +  Direction.Y * FISH_BACK_LENGTH, Position.Y  -Direction.X * FISH_BACK_LENGTH);
	glEnd();
	//DrawCircle(Position.X, Position.Y, FISH_VIEW_RANGE);
	//DrawCircleRangle(Position.X, Position.Y, FISH_COLISION_RANGE);
}

#pragma endregion

#pragma region Steering Fish

void Fish::SteerToTheAverageHeadingOfLocalFlockmates(Fish* fishes, int count)
{
	Vector avarageDirection = Vector(Direction.X, Direction.Y);
	int n = 1;
	for (int i = 0; i < count; i++)
	{
		float x = fishes[i].Position.X;
		float y = fishes[i].Position.Y;
		if (fabs(x - Position.X) < 0.001 && fabs(y - Position.Y) < 0.001) continue;
		if (sqrt((Position.X - x)*(Position.X - x) + (Position.Y - y)*(Position.Y - y)) < FISH_VIEW_RANGE)
		{
			avarageDirection += fishes[i].Direction;
			n++;
		}
	}
	if (n > 1)
	{
		avarageDirection.Normalize();
	}
	Direction = avarageDirection;
}

void Fish::SteerToTheAveragePositionOfLocalFlockmates(Fish* fishes, int count)
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
	if (fabs(avaragePosition.X - Position.X) < 0.01 && fabs(avaragePosition.Y - Position.Y) < 0.01) return;
	Vector directionToAvaragePosition = Vector(avaragePosition.X - Position.X, avaragePosition.Y - Position.Y);
	directionToAvaragePosition.Normalize();
	directionToAvaragePosition *= 0.3;
	Direction += directionToAvaragePosition;
}

bool Fish::DetectColisionWithFlockmates(Fish* fishes, int count, Vector vector)
{
	for (int i = 0; i < count; i++)
	{
		float x = fishes[i].Position.X;
		float y = fishes[i].Position.Y;		
		//if (x == Position.X && y == Position.Y) continue;
		if (fabs(x - Position.X) < 0.001 && (y - Position.Y) < 0.001) continue;
		float new_x = Position.X + vector.X, new_y = Position.Y + vector.Y;
		if (sqrt((x - new_x)*(x - new_x) + (y - new_y)*(y - new_y)) <= (FISH_COLISION_RANGE*2))
		{
			//printf("%f x %f -> %f x %f %f\n", Position.X, Position.Y, x, y, sqrt((Position.X - x)*(Position.X - x) + (Position.Y - y)*(Position.Y - y)));
			return false;
		}
	}
	return true;
}

#pragma endregion


#pragma region Degree to vector

float Fish::TargetPoint_X(float direction)
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

float Fish::TargetPoint_Y(float direction)
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

















