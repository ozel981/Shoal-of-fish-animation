#include "openGLwindow.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "../Fish/Fish.h"
#include "../Constant/Costant.h"
#include <time.h>

#pragma comment (lib, "glew32s.lib")

float x_position = -10;
bool rightDirection = 1;

bool moveAllow = true;
Fish Fishs[FISH_COUNT];

void Display();
void Timer(int);
void Init();
void Reshape(int, int);
void KeyboardInput(unsigned char, int, int);

int DisplayWindow()
{
	srand(time(NULL));
	for (int i = 0; i < FISH_COUNT; i++)
	{
		float x = MATRIX_HALF_WIDTH - (float)(rand() % (int)(2 * MATRIX_HALF_WIDTH));
		float y = MATRIX_HALF_HEIGHT - (float)(rand() % (int)(2 * MATRIX_HALF_HEIGHT));
		//printf("%d: %f x %f\n",i, x, y);
		Fishs[i] = Fish(Point(x,y), rand() % 360);
	}
	/*for (int i = 0; i < 600; i++)
	{
		Fishs[FISH_COUNT + i] = Fish(-300, -300 + i,270.0);
		Fishs[FISH_COUNT + 600 + i] = Fish(300, -300 + i, 90.0);
		Fishs[FISH_COUNT + 1200 + i] = Fish(-300 + i, -300, 0.0);
		Fishs[FISH_COUNT + 1800 + i] = Fish(-300 + i, 300, 180.0);
	}*/
	/*printf("%f x %f\n", Fishs[0].X, Fishs[0].Y);
	for (int i = 0; i <= 360; i++)
	{
		float x = 100 * cos(DegreeToRadians(i)) + 100;
		float y = 100 * sin(DegreeToRadians(i)) - 100;
		printf("%f x %f -> %f\n",x,y, Fishs[0].GetDirectionToPoint(x, y));
	}*/

	int argc = 1;
	char* argv[1] = { "shoal-of-fish" };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowPosition(200, 100);
	glutInitWindowSize(500, 500);

	glutCreateWindow("Shoal of fish");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutTimerFunc(0, Timer, 0);
	glutKeyboardFunc(KeyboardInput);
	Init();

	glutMainLoop();

	return 0;
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// draw

	for (int i = 0; i < FISH_COUNT; i++)
	{
		Fishs[i].Draw();
	}

	glutSwapBuffers();
}

void Timer(int)
{
	glutPostRedisplay();
	if(moveAllow) glutTimerFunc(1000 / 30, Timer, 0);
	float fishes[FISH_COUNT];
	Vector vectors[FISH_COUNT];
	/*for (int i = 0; i < FISH_COUNT; i++)
	{
		fishes[i] = Fishs[i].SteerTowardsTheAverageHeadingOfLocalFlockmates(Fishs, FISH_COUNT);
	}*/
	for (int i = 0; i < FISH_COUNT; i++)
	{
		vectors[i] = Fishs[i].VectorToTheAveragePositionOfLocalFlockmates(Fishs, FISH_COUNT);
	}

	for (int i = 0; i < FISH_COUNT; i++)
	{
		/*Fishs[i].SetDirection(fishes[i]);
		Fishs[i].Move();*/
		if (Fishs[i].DetectColisionWithFlockmates(Fishs, FISH_COUNT, vectors[i]))
		{

			Fishs[i].MoveBy(vectors[i]);
		}

		//printf("%d: %f x %f\n", i, Fishs[i].Position.X, Fishs[i].Position.Y);
	}
	moveAllow = false;

}

void Init()
{
	glClearColor(0, 0, 0, 1);
}

void Reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-(MATRIX_HALF_WIDTH), (MATRIX_HALF_WIDTH), -(MATRIX_HALF_HEIGHT), (MATRIX_HALF_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
}

void KeyboardInput(unsigned char key, int x, int y)
{
	switch (key) 
	{
		case ' ':
		{
			moveAllow = !moveAllow;
			if(moveAllow) glutTimerFunc(0, Timer, 0);
		}
		break;
		default:
		break;
	}
	glutPostRedisplay();
}
