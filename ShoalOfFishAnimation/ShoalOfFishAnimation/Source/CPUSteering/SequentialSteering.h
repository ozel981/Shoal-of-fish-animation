#pragma once
#include "../Fish/Fish.h"
#include "../Constant/Costant.h"

void SteerSequential(Fish* fish, int count)
{
	Vector vectors[FISH_COUNT];
	Vector vectors2[FISH_COUNT];
	for (int i = 0; i < FISH_COUNT; i++)
	{
		vectors[i] = fish[i].VectorToTheAverageHeadingOfLocalFlockmates(fish, FISH_COUNT) * 2;
		vectors2[i] += fish[i].VectorToAvoidCrowdingLocalFlockmates(fish, FISH_COUNT)*0.05;
		vectors2[i] += fish[i].VectorToTheAveragePositionOfLocalFlockmates(fish, FISH_COUNT)*0.3;
	}
	for (int i = 0; i < FISH_COUNT; i++)
	{
		fish[i].Direction = vectors[i].Normalized();
		fish[i].Move();
		fish[i].MoveBy(vectors2[i]);
	}
}