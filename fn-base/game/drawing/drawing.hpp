#pragma once

// Windows

// Custom
#include "../sdk/vector.hpp"
#include "../../vendor/imgui/imgui.h"

class drawing_class
{
public:
	void AddRect(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding, ImDrawFlags flags, float thickness);
	void AddRectFilled(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding, ImDrawFlags flags);

	void draw_box(float BoxWidth, FVector2D Root, FVector2D HeadBox, ImColor Color);
}; inline drawing_class drawing;