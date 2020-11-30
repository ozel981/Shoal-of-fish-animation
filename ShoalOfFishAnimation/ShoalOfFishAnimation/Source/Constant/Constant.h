#pragma once
#include <algorithm>

#pragma region Matrix

#define MATRIX_HALF_WIDTH 1000.0
#define MATRIX_HALF_HEIGHT 1000.0

#pragma endregion

#pragma region Fish

#define FISH_COUNT 5000
#define FISH_SIDE_THICKNESS 6.0
#define FISH_FRON_LENGTH 16.0
#define FISH_BACK_LENGTH 4.0
#define FISH_VIEW_RANGE 100.0
#define FISH_COLISION_RANGE 14.0
#define MOUSE_FEAR_DISTANCE 100.0

#pragma endregion

#pragma region Angles

#define M_PI 3.14159265358979323846
#define DegreeToRadians(x) (x*(M_PI/180.0))
#define RadiansToDegree(x) (x*(180.0/M_PI))

#pragma endregion

#pragma region Floats

#define FLOAT_EPSILON std::numeric_limits<float>::epsilon()

#pragma endregion

#pragma region Window

#define WIDTH 700.0
#define HEIGHT 500.0

#pragma endregion


