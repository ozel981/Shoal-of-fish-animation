#include "openGLwindow.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "../FishModel/fishModel.h"

#define RADPERDEG 0.0174533

#define FISH_COUNT 6
#define MATRIX_WIDTH 100
#define MATRIX_HEIGHT 100

#pragma comment (lib, "glew32s.lib")

float x_position = -10;
bool rightDirection = 1;
FishModel fishModels[FISH_COUNT] = { FishModel(-10,1),FishModel(-5,2),FishModel(-0,-3),FishModel(-20,-10),FishModel(-40,40),FishModel(20,14) };

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// draw

	for each (FishModel fishModel in fishModels)
	{
		fishModel.Draw();
	}

	/*glBegin(GL_POLYGON);
	glVertex2f(x_position, 1);
	glVertex2f(x_position, -1);
	glVertex2f(x_position + 2, -1);
	glVertex2f(x_position + 2, 1);
	glEnd();*/

	// do computation

	

	glutSwapBuffers();
}
void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 0);

	for (int i = 0; i < FISH_COUNT; i++)
	{
		fishModels[i].Move(1, -(MATRIX_HEIGHT/2), (MATRIX_HEIGHT/2));
	}
	
}

void init()
{
	glClearColor(0,0,0,1);
}

void reshape(int width,int height)
{
	glViewport(0,0, (GLsizei)width, (GLsizei)height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-(MATRIX_WIDTH/2), (MATRIX_WIDTH/2), -(MATRIX_HEIGHT/2), (MATRIX_HEIGHT / 2));
	glMatrixMode(GL_MODELVIEW);
}

int DisplayWindow()
{
	int argc = 1;
	char* argv[1] = { "shoal-of-fish" };


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowPosition(200, 100);
	glutInitWindowSize(500, 500);

	glutCreateWindow("Shoal of fish");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	init();

	glutMainLoop();

	return 0;
}

