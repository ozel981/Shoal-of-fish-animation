//For test time
#include <iostream>
#include <chrono> 
#include <cstdlib>
//
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "AnimationWindow.h"
#include "../Fish/Fish.h"
#include "../Fish/FishTypes.h"
#include "../Constant/Costant.h"
#include "../CPUSteering/SequentialSteering.h"
#include "../GPUSteering/ParallelSteering.cuh"


#pragma comment (lib, "glew32s.lib")


void Display();
void SetMousePos(int x, int y);
void CloseWindow();
void Timer(int);
void Init();
void Reshape(int, int);
void KeyboardInput(unsigned char, int, int);

extern "C" void ParallelSteering(Fish* h_fish, int count);
extern "C" void InitParallerlSteering(Fish* h_fish, int count);
extern "C" void FinalizeParallerlSteering();

AnimationWindow* animationWindow = &AnimationWindow(true);
float mouseX=-MATRIX_HALF_WIDTH, mouseY = -MATRIX_HALF_HEIGHT;
bool AnimationRun = true;

#pragma region Constructors

AnimationWindow::AnimationWindow(bool isCPU)
{
	this->isCPUx = isCPU;

	srand(time(NULL));
	for (int i = 0; i < FISH_COUNT; i++)
	{
		float x = MATRIX_HALF_WIDTH/2 - (float)(rand() % (int)(2 * MATRIX_HALF_WIDTH/2));
		float y = MATRIX_HALF_HEIGHT/2 - (float)(rand() % (int)(2 * MATRIX_HALF_HEIGHT/2));
		int fishKinf = 0;
		if (i == 0) fishKinf = 0;
		else if (i == 1) fishKinf = 33;
		else if (i == 2) fishKinf = 66;
		else if (i == 3) fishKinf = 99;
		else fishKinf = rand() % 100;

		if (fishKinf < 33) // Normal fish
		{
			FishShol[i] = Fish(Point(x, y), rand() % 360, FishSettings());//NormalFish(Point(x, y), rand() % 360);
		}
		else if (fishKinf < 66) // Fast fish
		{
			FishSettings settings = FishSettings();
			settings.color_R = 1;
			settings.color_G = 0;
			settings.color_B = 0;

			settings.speed = 20;
			FishShol[i] = Fish(Point(x, y), rand() % 360, settings);
		}
		else if (fishKinf < 99) // Small fihs
		{
			FishSettings settings = FishSettings();
			settings.color_R = 1;
			settings.color_G = 1;
			settings.color_B = 0;

			settings.size = 0.5;
			FishShol[i] = Fish(Point(x, y), rand() % 360, settings);
		}
		else // Big fish
		{
			FishSettings settings = FishSettings();
			settings.color_R = 0;
			settings.color_G = 0.6;
			settings.color_B = 0;

			settings.speed = 0.5;
			settings.size = 3;

			settings.grouping = false;
			settings.independence = true;
			FishShol[i] = Fish(Point(x, y), rand() % 360, settings);
		}
	}
}

#pragma endregion

AnimationWindow::~AnimationWindow()
{
	FinalizeParallerlSteering();
}

void AnimationWindow::Run()
{
	animationWindow = this;



	int argc = 1;
	char* argv[1] = { "shoal-of-fish" };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowPosition(200, 100);
	glutInitWindowSize(WIDTH, HEIGHT);

	glutCreateWindow("Shoal of fish");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutTimerFunc(0, Timer, 0);
	glutKeyboardFunc(KeyboardInput);
	glutCloseFunc(CloseWindow);
	glutPassiveMotionFunc(SetMousePos);
	Init();

	if (!(animationWindow->IsCPU()))
	{
		InitParallerlSteering(animationWindow->FishShol, FISH_COUNT);
	}


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
		animationWindow->FishShol[i].Draw();
	}

	glutSwapBuffers();
}

void Timer(int)
{
	glutPostRedisplay();
	if(AnimationRun) glutTimerFunc(1000 / 30, Timer, 0);

	if (animationWindow->IsCPU())
	{

		auto start = std::chrono::high_resolution_clock::now();
		SteerSequential(animationWindow->FishShol, FISH_COUNT, mouseX, mouseY);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		//std::cout << "Elapsed time: " << elapsed.count() << " s\n";
	}
	else
	{

		auto start = std::chrono::high_resolution_clock::now();
		ParallelSteering(animationWindow->FishShol, FISH_COUNT);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		//system("cls");
		//std::cout << "Elapsed time: " << elapsed.count() << " s\n";
	}
}
void CloseWindow()
{
	AnimationRun = false;
	Sleep(100);
	FinalizeParallerlSteering();
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
		if (AnimationRun) AnimationRun = false;
		else
		{
			AnimationRun = true;
			glutTimerFunc(0, Timer, 0);
		}
	}
	break;
	default:
		break;
	}
	glutPostRedisplay();
}

void SetMousePos(int x, int y)
{
	mouseX = (x / WIDTH) * (2 * MATRIX_HALF_WIDTH) - MATRIX_HALF_WIDTH;
	mouseY = - ((y / HEIGHT) * (2 * MATRIX_HALF_HEIGHT) - MATRIX_HALF_HEIGHT);
}
#pragma endregion

