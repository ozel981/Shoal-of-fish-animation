#pragma once

struct FishSettings
{
public:
	float color_R = 1;
	float color_G = 1;
	float color_B = 1;

	float speed = 2;
	float size = 1;

	bool grouping = true;
	bool independence = false;

	FishSettings();
	void operator =(FishSettings settings);
};
