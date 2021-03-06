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
#include <string>

#include "AnimationWindow.h"
#include "../Fish/Fish.h"
#include "../Fish/FishFactory.h"
#include "../Constant/Constant.h"
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

extern "C" std::string ParallelSteering(Fish* h_fish, float MouseX, float MouseY);
extern "C" void InitParallelSteering();
extern "C" void FinalizeParallelSteering();

AnimationWindow* animationWindow = &AnimationWindow(true);
float mouseX=-MATRIX_HALF_WIDTH, mouseY = -MATRIX_HALF_HEIGHT;
bool AnimationRun = true;

#pragma region Constructors

AnimationWindow::AnimationWindow(bool isCPU)
{
	this->isCPUx = isCPU;

	NormalFishFactory normalFishFactory = NormalFishFactory();
	FastFishFactory fastFishFactory = FastFishFactory();
	SmallFishFactory smallFishFactory = SmallFishFactory();
	BigFishFactory bigFishFactory = BigFishFactory();


	for (int i = 0; i < FISH_COUNT; i++)
	{
		
		int fishKinf = 0;
		if (i == 0) fishKinf = 0;
		else if (i == 1) fishKinf = 33;
		else if (i == 2) fishKinf = 66;
		else if (i == 3) fishKinf = 99;
		else fishKinf = rand() % 100;

		if (fishKinf < 33) // Normal fish
		{
			FishShol[i] = normalFishFactory.Create();
		}
		else if (fishKinf < 66) // Fast fish
		{
			FishShol[i] = fastFishFactory.Create();
		}
		else if (fishKinf < 99) // Small fihs
		{
			FishShol[i] = smallFishFactory.Create();
		}
		else // Big fish
		{
			FishShol[i] = bigFishFactory.Create();
		}
	}
}

#pragma endregion

AnimationWindow::~AnimationWindow()
{

}

void AnimationWindow::Run()
{
	animationWindow = this;

	if (FISH_COUNT > 15000)
	{
		printf("The count of fish can not be higher than: 15000\n");
		return;
	}

	if (!animationWindow->IsCPU())
	{
		InitParallelSteering();
	}

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
	
	AnimationRun = true;

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
		int frame = ((int)(1000 / (elapsed.count() * 1000)));
		char title[150] = "Shoal of fish ";
		strcat(title, (" | FPS [ Max: " + std::to_string(frame) + "; Actual: " + std::to_string(std::min(30, frame)) + " ] | Calcualtion time : " + std::to_string(elapsed.count()) + "s ").c_str());
		glutSetWindowTitle(title);

	}
	else
	{
		auto start = std::chrono::high_resolution_clock::now();
		std::string timing = ParallelSteering(animationWindow->FishShol, mouseX, mouseY);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		int frame = ((int)(1000 / (elapsed.count() * 1000)));
		char title[150] = "Shoal of fish ";
		strcat(title, (" | FPS [ Max: " + std::to_string(frame) + "; Actual: " + std::to_string(std::min(30, frame)) + " ]").c_str());
		strcat(title, timing.c_str());
		glutSetWindowTitle(title);
	}
}
void CloseWindow()
{
	FinalizeParallelSteering();
	AnimationRun = false;
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

