#pragma once

struct FishModel
{
public:
	int X, Y;
	int direction = 1;
	
	void Draw()
	{
		glBegin(GL_TRIANGLES);
		if (direction)
		{
			glVertex2f(X, Y);
			glVertex2f(X + 2, Y);
			glVertex2f(X + 1, Y + 3);
		}
		else
		{
			glVertex2f(X, Y + 3);
			glVertex2f(X + 2, Y + 3);
			glVertex2f(X + 1, Y);
		}

		glEnd();
	}

	void Move(int dist, int min_y, int max_y)
	{
		if (direction == 1)
		{
			if (Y + 3 < max_y) Y += dist;
			else direction = 0;
		}
		else
		{
			if (Y > min_y) Y -= dist;
			else direction = 1;
		}
	}

	FishModel(int x, int y);
};

FishModel::FishModel(int x, int y)
{
	this->X = x;
	this->Y = y;
}