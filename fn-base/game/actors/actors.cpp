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
#include "../../utility/console/console.hpp"

// ImGui
#include "../../vendor/imgui/imgui.h"

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
	// this is for debugging. actors.write_pointers();

	for (int i = 0; i < cache.PlayerArraySize; ++i)
	{
		auto PlayerArray = ReadMemory<uintptr_t>(cache.PlayerArray + (i * sizeof(uintptr_t)));
		auto CurrentActor = ReadMemory<uintptr_t>(PlayerArray + offsets::PawnPrivate);

		// Local player skip
		if (CurrentActor == cache.AcknowledgedPawn) continue;

		auto SkeletonMesh = ReadMemory<uintptr_t>(CurrentActor + offsets::Mesh);
		if (!SkeletonMesh) continue;

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