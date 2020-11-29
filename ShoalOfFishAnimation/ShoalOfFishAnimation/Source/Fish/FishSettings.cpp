#include "FishSettings.h"

FishSettings::FishSettings()
{
	color_R = 1;
	color_G = 1;
	color_B = 1;

	speed = 2;
	size = 1;

	grouping = true;
	independence = false;
}

void FishSettings::operator=(FishSettings settings)
{
	color_R = settings.color_R;
	color_G = settings.color_G;
	color_B = settings.color_B;

	speed = settings.speed;
	size = settings.size;

	grouping = settings.grouping;
	independence = settings.independence;

}