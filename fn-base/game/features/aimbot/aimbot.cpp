// Windows
#include <windows.h>
#include <stdint.h>

// Custom
#include "../../sdk/sdk.hpp"
#include "aimbot.hpp"
#include "../../utility/settings.hpp"
#include "../../utility/offsets.hpp"
#include "../../../vendor/kernel/communications.hpp"
#include "../../sdk/vector.hpp"

void AIMBOT_ENGINE::AimbotThread()
{
	if (AimbotEngine->TargetEntity && Aimbot->Enable)
	{
		auto ClosestMesh = ReadMemory<uint64_t>(TargetEntity + offsets::Mesh);
		sdk.GetCamera();
		FVector HitBox;
		HitBox = bones.GetBoneLocation(ClosestMesh, 110); // This is the bone you want to aim at, 110 for head, 66, for neck you can make a combo, and or anything you want.
		FVector2D HitboxScreen = sdk.ProjectWorldToScreen(HitBox);

		if (GetAsyncKeyState(Aimbot->Keybind) & 0x8000)
		{
			MoveMouseToEntity(HitboxScreen);
		}
	}
}

void AIMBOT_ENGINE::MoveMouseToEntity(FVector2D Index)
{
	const float AimSpeedX = Aimbot->SmoothingX;
	const float AimSpeedY = Aimbot->SmoothingY;
	const FVector2D ScreenCenter = { static_cast<double>(Globals->ScreenWidth) / 2, static_cast<double>(Globals->ScreenHeight) / 2 };

	FVector2D Target = { 0, 0 };

	if (Index.x != 0)
	{
		Target.x = (Index.x > ScreenCenter.x) ? -(ScreenCenter.x - Index.x) : (Index.x - ScreenCenter.x);
		Target.x /= AimSpeedX;
		Target.x = (Target.x + ScreenCenter.x > ScreenCenter.x * 2 || Target.x + ScreenCenter.x < 0) ? 0 : Target.x;
	}

	if (Index.y != 0)
	{
		Target.y = (Index.y > ScreenCenter.y) ? -(ScreenCenter.y - Index.y) : (Index.y - ScreenCenter.y);
		Target.y /= AimSpeedY;
		Target.y = (Target.y + ScreenCenter.y > ScreenCenter.y * 2 || Target.y + ScreenCenter.y < 0) ? 0 : Target.y;
	}

	SetCursorPos(Target.x, Target.y);
}