#pragma once

#include "Fish.h"
#include "../Constant/Constant.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class NormalFishFactory
{
public:
	NormalFishFactory()
	{
		srand(time(NULL));
	}
	Fish Create()
	{
		float x = MATRIX_HALF_WIDTH / 2 - (float)(rand() % (int)(2 * MATRIX_HALF_WIDTH / 2));
		float y = MATRIX_HALF_HEIGHT / 2 - (float)(rand() % (int)(2 * MATRIX_HALF_HEIGHT / 2));
		return Fish(Point(x, y), rand() % 360, FishSettings());
	}
};

class FastFishFactory
{
public:
	FastFishFactory()
	{
		srand(time(NULL));
	}
	Fish Create()
	{
		float x = MATRIX_HALF_WIDTH / 2 - (float)(rand() % (int)(2 * MATRIX_HALF_WIDTH / 2));
		float y = MATRIX_HALF_HEIGHT / 2 - (float)(rand() % (int)(2 * MATRIX_HALF_HEIGHT / 2));
		FishSettings settings = FishSettings();
		settings.color_R = 1;
		settings.color_G = 0;
		settings.color_B = 0;

		settings.speed = 20;
		return Fish(Point(x, y), rand() % 360, settings);
	}
};

class SmallFishFactory
{
public:
	SmallFishFactory()
	{
		srand(time(NULL));
	}
	Fish Create()
	{
		float x = MATRIX_HALF_WIDTH / 2 - (float)(rand() % (int)(2 * MATRIX_HALF_WIDTH / 2));
		float y = MATRIX_HALF_HEIGHT / 2 - (float)(rand() % (int)(2 * MATRIX_HALF_HEIGHT / 2));
		FishSettings settings = FishSettings();
		settings.color_R = 1;
		settings.color_G = 1;
		settings.color_B = 0;

		settings.size = 0.5;
		return Fish(Point(x, y), rand() % 360, settings);
	}
};

class BigFishFactory
{
public:
	BigFishFactory()
	{
		srand(time(NULL));
	}
	Fish Create()
	{
		float x = MATRIX_HALF_WIDTH / 2 - (float)(rand() % (int)(2 * MATRIX_HALF_WIDTH / 2));
		float y = MATRIX_HALF_HEIGHT / 2 - (float)(rand() % (int)(2 * MATRIX_HALF_HEIGHT / 2));
		FishSettings settings = FishSettings();
		settings.color_R = 0;
		settings.color_G = 0.6;
		settings.color_B = 0;

		settings.speed = 0.5;
		settings.size = 3;

		settings.grouping = false;
		settings.independence = true;
		return Fish(Point(x, y), rand() % 360, settings);
	}
};

