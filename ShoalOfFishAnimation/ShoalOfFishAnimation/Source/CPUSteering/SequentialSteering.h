#pragma once
#include "../Fish/Fish.h"
#include "../Constant/Costant.h"

void SteerSequential(Fish* fish, int count)
{
	Vector directionVectors[FISH_COUNT];
	Vector groupingVectors[FISH_COUNT];
	for (int i = 0; i < FISH_COUNT; i++)
	{
		directionVectors[i] = fish[i].VectorToTheAverageHeadingOfLocalFlockmates(fish, FISH_COUNT);
		groupingVectors[i] += fish[i].VectorToAvoidCrowdingLocalFlockmates(fish, FISH_COUNT);
		groupingVectors[i] += fish[i].VectorToTheAveragePositionOfLocalFlockmates(fish, FISH_COUNT);
	}
	for (int i = 0; i < FISH_COUNT; i++)
	{
		fish[i].Direction = directionVectors[i].Normalized();
		fish[i].Move();
		fish[i].MoveBy(groupingVectors[i]);
	}
}