#pragma once
#include "../Fish/Fish.h"
#include "../Constant/Constant.h"

void SteerSequential(Fish* fish, int count, float mouseX, float mouseY)
{
	printf("%fx%f\n", mouseX, mouseY);
	Vector directionVectors[FISH_COUNT];
	Vector groupingVectors[FISH_COUNT];
	for (int i = 0; i < FISH_COUNT; i++)
	{
		directionVectors[i] = fish[i].VectorToTheAverageHeadingOfLocalFlockmates(fish, FISH_COUNT);
		if (!(mouseX < (-MATRIX_HALF_WIDTH + 20) || mouseX >(MATRIX_HALF_WIDTH - 20) || mouseY > (MATRIX_HALF_HEIGHT - 20) || mouseY < (-MATRIX_HALF_HEIGHT + 20)))
		{
			directionVectors[i] += fish[i].AvoidMouse(mouseX, mouseY);
		}
		groupingVectors[i] = fish[i].VectorToAvoidCrowdingLocalFlockmates(fish, FISH_COUNT);
		groupingVectors[i] += fish[i].VectorToTheAveragePositionOfLocalFlockmates(fish, FISH_COUNT);
	}
	for (int i = 0; i < FISH_COUNT; i++)
	{
		fish[i].MoveBy(directionVectors[i]);
		fish[i].MoveBy(groupingVectors[i]);
		fish[i].Direction = directionVectors[i].Normalized();

	}
}