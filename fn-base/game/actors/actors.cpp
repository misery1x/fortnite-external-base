#include "actors.hpp"

// Windows
#include <thread>
#include <chrono>
#include <iostream>

// Custom
#include "../utility/offsets.hpp"
#include "../utility/cache.hpp"
#include "../utility/settings.hpp"
#include "../sdk/camera.hpp"
#include "../../vendor/kernel/communications.hpp"
#include "../sdk/Matrix.hpp"
#include "../sdk/sdk.hpp"
#include "../drawing/drawing.hpp"

// ImGui
#include "../../vendor/imgui/imgui.h"

void actors_class::actor_cache()
{
	TempEntityList.clear();

	cache.UWorld = memory.Read<uintptr_t>(memory.base_address + offsets::UWorld);
	cache.GameInstance = memory.Read<uintptr_t>(cache.UWorld + offsets::OwningGameInstance);
	cache.LocalPlayer = memory.Read<uintptr_t>(memory.Read<uintptr_t>(cache.GameInstance + offsets::LocalPlayers));
	cache.PlayerController = memory.Read<uintptr_t>(cache.LocalPlayer + offsets::PlayerController);
	cache.AcknowledgedPawn = memory.Read<uintptr_t>(cache.PlayerController + offsets::AcknowledgedPawn);
	cache.Mesh = memory.Read<uintptr_t>(cache.AcknowledgedPawn + offsets::Mesh);
	cache.PlayerState = memory.Read<uintptr_t>(cache.AcknowledgedPawn + offsets::PlayerState);
	cache.RootComponent = memory.Read<uintptr_t>(cache.AcknowledgedPawn + offsets::RootComponent);
	cache.GameState = memory.Read<uintptr_t>(cache.UWorld + offsets::GameState);
	cache.PlayerArray = memory.Read<uintptr_t>(cache.GameState + offsets::PlayerArray);
	cache.PlayerArraySize = memory.Read<int>(cache.GameState + (offsets::PlayerArray + sizeof(uintptr_t)));

	for (int i = 0; i < cache.PlayerArraySize; ++i)
	{
		auto PlayerArray = memory.Read<uintptr_t>(cache.PlayerArray + (i * sizeof(uintptr_t)));
		auto CurrentActor = memory.Read<uintptr_t>(PlayerArray + offsets::PawnPrivate);

		// Local player skip
		if (CurrentActor == cache.AcknowledgedPawn) continue;

		auto SkeletonMesh = memory.Read<uintptr_t>(CurrentActor + offsets::Mesh);
		if (!SkeletonMesh) continue;

		Entity_class CachedActors{};
		CachedActors.Entity = CurrentActor;
		CachedActors.SkeletalMesh = memory.Read<uintptr_t>(CurrentActor + offsets::Mesh);
		CachedActors.RootComponent = memory.Read<uintptr_t>(CurrentActor + offsets::RootComponent);
		CachedActors.PlayerState = memory.Read<uintptr_t>(CurrentActor + offsets::PlayerState);

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
		if (distance > visuals.render_distance && cache.AcknowledgedPawn) continue;

		// Box
		const float BoxHeight = std::abs(Head.y - Root.y);
		const float BoxWidth = BoxHeight * 0.30;

		if (visuals.box)
		{
			drawing.draw_box(BoxWidth, Root, HeadBox, ImColor(255, 255, 255));
		}
	}
}