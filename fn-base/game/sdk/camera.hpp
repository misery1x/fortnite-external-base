#pragma once

// Windows

// Custom
#include "vector.hpp"

struct camera_position_s
{
	FVector location{};
	FVector rotation{};
	float fov{};
};
inline camera_position_s camera_postion{};