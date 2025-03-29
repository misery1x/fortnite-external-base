// Windows
#include <iostream>

// Custom
#include "drawing.hpp"
#include "../sdk/vector.hpp"
#include "../../vendor/imgui/imgui.h"

void drawing_class::AddRect(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding, ImDrawFlags flags, float thickness)
{

}

void drawing_class::AddRectFilled(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding, ImDrawFlags flags)
{

}

void drawing_class::draw_box(float BoxWidth, FVector2D Root, FVector2D HeadBox, ImColor Color)
{
    // Draw acual box
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(HeadBox.x - BoxWidth / 2.0f, HeadBox.y), ImVec2(Root.x + BoxWidth / 2.0f, Root.y), Color, 0.0f, 0, 1.0f);
    
    // Filled
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(HeadBox.x - BoxWidth / 2.0f, HeadBox.y), ImVec2(Root.x + BoxWidth / 2.0f, Root.y), ImColor(51, 51, 51, 90), 1.0f);
}