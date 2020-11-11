#include"fishModel.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include<math.h>

#define M_PI 3.14159265358979323846
#define DegreeToRadians(x) (x*(M_PI/180))


#define FISH_SIDE_THICKNESS 6
#define FISH_FRON_LENGTH 12
#define FISH_BACK_LENGTH 4


FishModel::FishModel(int x, int y)
{
	this->X = x;
	this->Y = y;
}

int FishModel::x_position(int x, int y)
{
	return ((x-X)*cos(DegreeToRadians(this->degree)) - (y-Y)*sin(DegreeToRadians(this->degree))) + X;
}

int FishModel::y_position(int x, int y)
{
	return ((x-X)*sin(DegreeToRadians(this->degree)) + (y-Y)*cos(DegreeToRadians(this->degree))) + Y;
}

void FishModel::Draw()
{
	glBegin(GL_TRIANGLES);
	glVertex2f(x_position(X, Y + 12), y_position(X, Y + 12));
	glVertex2f(x_position(X - 6, Y - 4), y_position(X - 6, Y - 4));
	glVertex2f(x_position(X + 6, Y - 4), y_position(X + 6, Y - 4));
	glEnd();
}

void FishModel::Move(int dist, int min_y, int max_y)
{
	/*if (direction == 1)
	{
		if (Y + FISH_HEIGTH < max_y) Y += dist;
		else direction = 0;
	}
	else
	{
		if (Y > min_y) Y -= dist;
		else direction = 1;
	}*/
}

void FishModel::SetDegree(int degree)
{
	this->degree = degree % 360;
}