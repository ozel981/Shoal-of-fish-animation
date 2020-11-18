#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "AnimationWindow.h"
#include "../Fish/Fish.h"
#include "../Constant/Costant.h"
#include "../CPUSteering/SequentialSteering.h"

#pragma comment (lib, "glew32s.lib")

void Display();
void Timer(int);
void Init();
void Reshape(int, int);
void KeyboardInput(unsigned char, int, int);

AnimationWindow animationWindow;

#pragma region Constructors

AnimationWindow::AnimationWindow(bool isCPU)
{
	this->isCPU = isCPU;

	srand(time(NULL));
	for (int i = 0; i < FISH_COUNT; i++)
	{
		float x = MATRIX_HALF_WIDTH - (float)(rand() % (int)(2 * MATRIX_HALF_WIDTH));
		float y = MATRIX_HALF_HEIGHT - (float)(rand() % (int)(2 * MATRIX_HALF_HEIGHT));
		FishShol[i] = Fish(Point(x, y), rand() % 360);
	}
}

#pragma endregion

void AnimationWindow::Run()
{
	animationWindow = this;

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

}

// Functions necessary for servicing GLUT OpenGL
#pragma region Friend functions

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// draw

	for (int i = 0; i < FISH_COUNT; i++)
	{
		animationWindow.FishShol[i].Draw();
	}

	glutSwapBuffers();
}

void Timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, Timer, 0);

	if (animationWindow.isCPU) SteerSequential(animationWindow.FishShol, FISH_COUNT);
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
		glutTimerFunc(0, Timer, 0);
	}
	break;
	default:
		break;
	}
	glutPostRedisplay();
}

#pragma endregion

