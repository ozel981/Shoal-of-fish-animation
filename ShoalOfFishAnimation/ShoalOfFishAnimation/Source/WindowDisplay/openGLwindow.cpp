#include "openGLwindow.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "../FishModel/fishModel.h"

#define RADPERDEG 0.0174533

#define FISH_COUNT 6
#define MATRIX_WIDTH 600
#define MATRIX_HEIGHT 600

#pragma comment (lib, "glew32s.lib")

float x_position = -10;
bool rightDirection = 1;

bool moveAllow = true;
FishModel fishModels[FISH_COUNT] = { FishModel(-250,-250),FishModel(-125,-125),FishModel(0,0),FishModel(100,100),FishModel(200,200),FishModel(250,250) };

void Display();
void Timer(int);
void Init();
void Reshape(int, int);
void KeyboardInput(unsigned char, int, int);

int DisplayWindow()
{
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

	for each (FishModel fishModel in fishModels)
	{
		fishModel.Draw();
	}
	/*printf("%d : ", fishModels[0].X);
	printf("%d\n", fishModels[0].Y);
	printf("%d : ", fishModels[0].x_position(fishModels[0].X, fishModels[0].Y + 12));
	printf("%d\n", fishModels[0].y_position(fishModels[0].X, fishModels[0].Y + 12));
	printf("%d : ", fishModels[0].x_position(fishModels[0].X - 6, fishModels[0].Y - 4));
	printf("%d\n", fishModels[0].y_position(fishModels[0].X - 6, fishModels[0].Y - 4));
	printf("%d : ", fishModels[0].x_position(fishModels[0].X + 6, fishModels[0].Y - 4));
	printf("%d\n", fishModels[0].y_position(fishModels[0].X + 6, fishModels[0].Y - 4));
	printf("-----------------------------------------------------------------------\n");
	*/
	glutSwapBuffers();
}

void Timer(int)
{
	glutPostRedisplay();
	if(moveAllow)glutTimerFunc(1000 / 30, Timer, 0);

	for (int i = 0; i < FISH_COUNT; i++)
	{
		//fishModels[i].Move(6, -(MATRIX_HEIGHT / 2), (MATRIX_HEIGHT / 2));
		fishModels[i].SetDegree(fishModels[i].degree + 1);
	}

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
	gluOrtho2D(-(MATRIX_WIDTH / 2), (MATRIX_WIDTH / 2), -(MATRIX_HEIGHT / 2), (MATRIX_HEIGHT / 2));
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
