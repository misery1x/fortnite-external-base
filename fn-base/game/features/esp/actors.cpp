#include "actors.hpp"

// Windows
#include <thread>
#include <chrono>
#include <iostream>

// Custom
#include "../../utility/offsets.hpp"
#include "../../utility/cache.hpp"
#include "../../utility/settings.hpp"
#include "../../sdk/camera.hpp"
#include "../../../vendor/kernel/communications.hpp"
#include "../../sdk/Matrix.hpp"
#include "../../sdk/sdk.hpp"
#include "../../drawing/drawing.hpp"
#include "../../../utility/console/console.hpp"
#include "../aimbot/aimbot.hpp"

// ImGui
#include "../../../vendor/imgui/imgui.h"

void actors_class::actor_cache()
{
	TempEntityList.clear();

	cache.UWorld = ReadMemory<uintptr_t>(BaseAddress + offsets::UWorld);
	cache.GameInstance = ReadMemory<uintptr_t>(cache.UWorld + offsets::OwningGameInstance);
	cache.LocalPlayer = ReadMemory<uintptr_t>(ReadMemory<uintptr_t>(cache.GameInstance + offsets::LocalPlayers));
	cache.PlayerController = ReadMemory<uintptr_t>(cache.LocalPlayer + offsets::PlayerController);
	cache.AcknowledgedPawn = ReadMemory<uintptr_t>(cache.PlayerController + offsets::AcknowledgedPawn);
	cache.Mesh = ReadMemory<uintptr_t>(cache.AcknowledgedPawn + offsets::Mesh);
	cache.PlayerState = ReadMemory<uintptr_t>(cache.AcknowledgedPawn + offsets::PlayerState);
	cache.RootComponent = ReadMemory<uintptr_t>(cache.AcknowledgedPawn + offsets::RootComponent);
	cache.GameState = ReadMemory<uintptr_t>(cache.UWorld + offsets::GameState);
	cache.PlayerArray = ReadMemory<uintptr_t>(cache.GameState + offsets::PlayerArray);
	cache.PlayerArraySize = ReadMemory<int>(cache.GameState + (offsets::PlayerArray + sizeof(uintptr_t)));
	actors.write_pointers();

	for (int i = 0; i < cache.PlayerArraySize; ++i)
	{
		auto PlayerArray = ReadMemory<uintptr_t>(cache.PlayerArray + (i * sizeof(uintptr_t)));
		auto CurrentActor = ReadMemory<uintptr_t>(PlayerArray + offsets::PawnPrivate);
		auto SkeletonMesh = ReadMemory<uintptr_t>(CurrentActor + offsets::Mesh);

		// Local player skip
		if (CurrentActor == cache.AcknowledgedPawn) continue;

		// Skip if bone = 0
		auto BaseBone = bones.GetBoneLocation(SkeletonMesh, 110);
		if (BaseBone.x == 0 && BaseBone.y == 0 && BaseBone.z == 0) continue;

		Entity_class CachedActors{};
		CachedActors.Entity = CurrentActor;
		CachedActors.SkeletalMesh = ReadMemory<uintptr_t>(CurrentActor + offsets::Mesh);
		CachedActors.RootComponent = ReadMemory<uintptr_t>(CurrentActor + offsets::RootComponent);
		CachedActors.PlayerState = ReadMemory<uintptr_t>(CurrentActor + offsets::PlayerState);

		TempEntityList.push_back(CachedActors);
	}

	EntityList.clear();
	EntityList = TempEntityList;
}

void actors_class::draw_esp()
{
	ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

	for (auto& Cached : EntityList)
	{
		const FVector HeadBone = bones.GetBoneLocation(Cached.SkeletalMesh, 110);
		const FVector BaseBone = bones.GetBoneLocation(Cached.SkeletalMesh, 0);

		const FVector2D Head = sdk.ProjectWorldToScreen(HeadBone);
		const FVector2D Root = sdk.ProjectWorldToScreen(BaseBone);

		const FVector2D HeadBox = sdk.ProjectWorldToScreen(FVector(HeadBone.x, HeadBone.y, HeadBone.z + 16));

		const float distance = camera_postion.location.Distance(BaseBone) / 100;
		if (distance > Visuals->RenderDistance && cache.AcknowledgedPawn) continue;

		// Aimbot
		if (Aimbot->Enable)
		{
			double dx = Head.x - (Globals->ScreenWidth / 2);
			double dy = Head.y - (Globals->ScreenHeight / 2);
			float dist = sqrtf(dx * dx + dy * dy);
			if (dist < Aimbot->FieldOfView)
			{
				if (dist < AimbotEngine->TargetDistance)
				{
					AimbotEngine->TargetDistance = dist;
					AimbotEngine->TargetEntity = Cached.Entity;
				}
			}
		}

		// Box
		const float BoxHeight = std::abs(Head.y - Root.y);
		const float BoxWidth = BoxHeight * 0.30;

		if (Visuals->Enable)
		{
			if (Visuals->Box)
			{
				drawing.Draw2DBox(BoxWidth, Root, HeadBox, ImColor(255, 255, 255));
			}

			if (Visuals->Skeleton)
			{
				FVector2D Bone1 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 66));
				FVector2D Bone2 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 9));
				FVector2D Bone3 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 38));
				FVector2D Bone4 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 10));
				FVector2D Bone5 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 39));
				FVector2D Bone6 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 62));
				FVector2D Bone7 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 33));
				FVector2D Bone8 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 7));
				FVector2D Bone9 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 3));
				FVector2D Bone10 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 71));
				FVector2D Bone11 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 78));
				FVector2D Bone12 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 79));
				FVector2D Bone13 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 72));
				FVector2D Bone14 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 75));
				FVector2D Bone15 = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, 82));

				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone1.x, Bone1.y),   ImVec2(Bone2.x, Bone2.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone1.x, Bone1.y),   ImVec2(Bone3.x, Bone3.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone2.x, Bone2.y),   ImVec2(Bone4.x, Bone4.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone3.x, Bone3.y),   ImVec2(Bone5.x, Bone5.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone5.x, Bone5.y),   ImVec2(Bone6.x, Bone6.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone4.x, Bone4.y),   ImVec2(Bone7.x, Bone7.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone1.x, Bone1.y),   ImVec2(Bone8.x, Bone8.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone8.x, Bone8.y),   ImVec2(Bone9.x, Bone9.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone9.x, Bone9.y),   ImVec2(Bone10.x, Bone10.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone9.x, Bone9.y),   ImVec2(Bone11.x, Bone11.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone10.x, Bone10.y), ImVec2(Bone13.x, Bone13.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone11.x, Bone11.y), ImVec2(Bone12.x, Bone12.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone13.x, Bone13.y), ImVec2(Bone14.x, Bone14.y), ImColor(255, 255, 255), 1.0f);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Bone12.x, Bone12.y), ImVec2(Bone15.x, Bone15.y), ImColor(255, 255, 255), 1.0f);
			}

			// This was so i can make the skeleton.
			if (Visuals->DebugBones)
			{
				for (int BoneID = 1; BoneID <= 200; BoneID++)
				{
					FVector2D Bone = sdk.ProjectWorldToScreen(bones.GetBoneLocation(Cached.SkeletalMesh, BoneID));
					if (Bone.x > 0 && Bone.y > 0)
					{
						char BoneText[8];
						sprintf_s(BoneText, "%d", BoneID);
						ImGui::GetBackgroundDrawList()->AddText(ImVec2(Bone.x, Bone.y), ImColor(255, 255, 255), BoneText);
					}
				}
			}
		}
	}
}

void actors_class::write_pointers()
{
	// This is for debugging.
	console.WriteLinePointer("UWorld", cache.UWorld);
	console.WriteLinePointer("GameInstance", cache.GameInstance);
	console.WriteLinePointer("LocalPlayer", cache.LocalPlayer);
	console.WriteLinePointer("PlayerController", cache.PlayerController);
	console.WriteLinePointer("AcknowledgedPawn", cache.AcknowledgedPawn);
	console.WriteLinePointer("Mesh", cache.Mesh);
	console.WriteLinePointer("PlayerState", cache.PlayerState);
	console.WriteLinePointer("RootComponent", cache.RootComponent);
	console.WriteLinePointer("GameState", cache.GameState);
	console.WriteLinePointer("PlayerArray", cache.PlayerArray);
	console.WriteLinePointer("PlayerArraySize", cache.PlayerArraySize);

	system ("cls");
}