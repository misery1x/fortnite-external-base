#pragma once

#include <Windows.h>
#include <float.h>

#include "../../sdk/vector.hpp"

class AIMBOT_ENGINE
{
public:
	float TargetDistance = FLT_MAX;
	uintptr_t TargetEntity = NULL;
	uintptr_t DesyncTargetEntity = NULL;

	void AimbotThread();
	void MoveMouseToEntity(FVector2D Index);
}; inline AIMBOT_ENGINE* AimbotEngine = new AIMBOT_ENGINE();