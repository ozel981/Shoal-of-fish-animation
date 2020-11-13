#include "openGLwindow.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "../Fish/Fish.h"
#include "../Constant/Costant.h"

#pragma comment (lib, "glew32s.lib")

float x_position = -10;
bool rightDirection = 1;

bool moveAllow = true;
Fish Fishs[FISH_COUNT] = { Fish(-250,-250),Fish(-125,-125),Fish(0,0),Fish(100,100),Fish(200,200),Fish(250,250) };

void Display();
void Timer(int);
void Init();
void Reshape(int, int);
void KeyboardInput(unsigned char, int, int);

int DisplayWindow()
{
	/*float kat = 67.9865;
	for (int i = 0; i < 1000; i++)
	{
		printf("xd: %f\n", kat);
		float x = tan(DegreeToRadians(kat));
		kat = RadiansToDegree(atan(x));
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

	for each (Fish Fish in Fishs)
	{
		Fish.Draw();
	}
	//printf("%f:  %f\n", Fishs[0].direction,Fishs[0].GetDirectionToPoint(Fishs[0].TargetPoint_X(),Fishs[0].TargetPoint_Y()));
	printf("%f : %f \n", Fishs[0].TargetPoint_X(), Fishs[0].TargetPoint_Y());
	glutSwapBuffers();
}

void Timer(int)
{
	glutPostRedisplay();
	if(moveAllow) glutTimerFunc(1000 / 30, Timer, 0);

	for (int i = 0; i < FISH_COUNT; i++)
	{
		//Fishs[i].Move(6, -(MATRIX_HEIGHT / 2), (MATRIX_HEIGHT / 2));
		
		Fishs[i].Move();
		//Fishs[i].SetDegree(Fishs[i].direction + 1);
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
