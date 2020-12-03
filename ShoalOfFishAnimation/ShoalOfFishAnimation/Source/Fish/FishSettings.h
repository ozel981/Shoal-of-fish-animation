#pragma once

struct FishSettings
{
public:
	float color_R;
	float color_G;
	float color_B;

	float speed;
	float size;

	bool grouping = true;
	bool independence = false;

	FishSettings();
	void operator =(FishSettings settings);
};
