#include"Fish.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <algorithm> 
#include <math.h>
#include "../Constant/Costant.h"
#include "../Point/Point.h"

#pragma region Constructors

Fish::Fish(Point position, float direction, FishSettings settings)
{
	this->Position = Point(position.X,position.Y);
	this->Direction = Vector(TargetPoint_X(direction), TargetPoint_Y(direction)).Normalized();
	this->Settings = settings;
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
	if (Position.X > MATRIX_HALF_WIDTH) MoveTo(Point(-MATRIX_HALF_WIDTH, -Position.Y));
	if (Position.X < -MATRIX_HALF_WIDTH) MoveTo(Point(MATRIX_HALF_WIDTH, -Position.Y));
	if (Position.Y > MATRIX_HALF_HEIGHT) MoveTo(Point(-Position.X, -MATRIX_HALF_HEIGHT));
	if (Position.Y < -MATRIX_HALF_HEIGHT) MoveTo(Point(-Position.X, MATRIX_HALF_HEIGHT));
	Position.X += Direction.X;
	Position.Y += Direction.Y;
}

void Fish::MoveBy(Vector vector)
{
	if (Position.X > MATRIX_HALF_WIDTH) MoveTo(Point(-MATRIX_HALF_WIDTH, -Position.Y));
	if (Position.X < -MATRIX_HALF_WIDTH) MoveTo(Point(MATRIX_HALF_WIDTH, -Position.Y));
	if (Position.Y > MATRIX_HALF_HEIGHT) MoveTo(Point(-Position.X, -MATRIX_HALF_HEIGHT));
	if (Position.Y < -MATRIX_HALF_HEIGHT) MoveTo(Point(-Position.X, MATRIX_HALF_HEIGHT));
	Position.X += vector.X;
	Position.Y += vector.Y;
}

#pragma endregion

#pragma region Draw

void Fish::SetOwnColor()
{
	glColor3f(Settings.color_R, Settings.color_G, Settings.color_B);
}

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

void Fish::Draw()
{
	glBegin(GL_TRIANGLES);
	SetOwnColor();
	// fish front point
	glVertex2f(Position.X + (Direction.X * FISH_FRON_LENGTH)*Settings.size, Position.Y + (Direction.Y * FISH_FRON_LENGTH)*Settings.size);
	Vector leftVect = Vector(-Direction.Y, Direction.X);
	leftVect *= FISH_SIDE_THICKNESS;
	Vector rightVect = Vector(Direction.Y, -Direction.X);
	rightVect *= FISH_SIDE_THICKNESS;
	Vector rearVect = Vector(-Direction.X, -Direction.X);
	rearVect * FISH_BACK_LENGTH;
	leftVect += rearVect;
	rightVect += rearVect;
	// fish rear left point
	glVertex2f(Position.X  + leftVect.X * Settings.size, Position.Y + leftVect.Y * Settings.size);
	// fish rear right point
	glVertex2f(Position.X + rightVect.X * Settings.size, Position.Y + rightVect.Y * Settings.size);
	
	glEnd();
	//DrawCircle(Position.X, Position.Y, FISH_VIEW_RANGE);
	//DrawCircle(Position.X, Position.Y, FISH_COLISION_RANGE);
}

#pragma endregion

#pragma region Fish steering

Vector Fish::VectorToTheAverageHeadingOfLocalFlockmates(Fish* fishes, int count)
{
	if (Settings.independence) return Vector(Direction.X, Direction.Y);
	Vector avarageDirection = Vector(Direction.X, Direction.Y);
	int n = 1;
	for (int i = 0; i < count; i++)
	{
		float x = fishes[i].Position.X;
		float y = fishes[i].Position.Y;
		if (fabs(x - Position.X) < FLOAT_EPSILON && fabs(y - Position.Y) < FLOAT_EPSILON)
		{
			continue;
		}
		if (((Position.X-x)*(Position.X-x)+(Position.Y-y)*(Position.Y-y)) < FISH_VIEW_RANGE*FISH_VIEW_RANGE)
		{
			avarageDirection += fishes[i].Direction;
			n++;
		}
	}
	if (n > 1)
	{
		avarageDirection.Normalize();
		avarageDirection *= Settings.speed;
	}
	return (avarageDirection);
}

Vector Fish::AvoidMouse(float x, float y)
{
	float dist = sqrt((x - Position.X)*(x - Position.X) + (y - Position.Y)*(y - Position.Y));
	if (dist > MOUSE_FEAR_DISTANCE) return  Vector(0, 0);
	else return (Vector(Position.X - x, Position.Y - y).Normalized()) * (MOUSE_FEAR_DISTANCE - dist);
}

Vector Fish::VectorToTheAveragePositionOfLocalFlockmates(Fish* fishes, int count)
{
	if (!Settings.grouping) return Vector(0, 0);
	Point avaragePosition = Point(Position.X, Position.Y);
	int n = 1;
	for (int i = 0; i < count; i++)
	{
		float x = fishes[i].Position.X;
		float y = fishes[i].Position.Y;
		if (fabs(x - Position.X) < FLOAT_EPSILON && fabs(y - Position.Y) < FLOAT_EPSILON)
		{
			continue;
		}
		if (((Position.X - x)*(Position.X - x) + (Position.Y - y)*(Position.Y - y)) < FISH_VIEW_RANGE*FISH_VIEW_RANGE)
		{
			avaragePosition += fishes[i].Position;
			n++;
		}
	}
	avaragePosition /= n;
	if (fabs(avaragePosition.X - Position.X) < FLOAT_EPSILON && fabs(avaragePosition.Y - Position.Y) < FLOAT_EPSILON)
	{
		return Vector(0, 0);
	}
	Vector directionToAvaragePosition = Vector(avaragePosition.X - Position.X, avaragePosition.Y - Position.Y);
	directionToAvaragePosition.Normalize();
	return (directionToAvaragePosition);
}

Vector Fish::VectorToAvoidCrowdingLocalFlockmates(Fish* fishes, int count)
{
	if (!Settings.grouping) return Vector(0, 0);
	Vector resultantVersor = Vector(0, 0);
	for (int i = 0; i < count; i++)
	{
		float x = fishes[i].Position.X;
		float y = fishes[i].Position.Y;
		if (fabs(x - Position.X) < FLOAT_EPSILON && fabs(y - Position.Y) < FLOAT_EPSILON)
		{
			continue;
		}
		Vector fromLocalToNeigh = Vector(x - Position.X, y - Position.Y);
		float fishDistance = fromLocalToNeigh.Length();
		if (fishDistance <= (FISH_COLISION_RANGE * Settings.size + FISH_COLISION_RANGE * fishes[i].Settings.size))
		{
			fromLocalToNeigh.Normalize();
			fromLocalToNeigh *= (FISH_COLISION_RANGE * Settings.size + FISH_COLISION_RANGE * fishes[i].Settings.size) - fishDistance;
			resultantVersor -= fromLocalToNeigh;
		}
	}
	return (resultantVersor*0.3);
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

















