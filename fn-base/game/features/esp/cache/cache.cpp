#include "cache.hpp"

void actor_cache_class::actor_cache()
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

		// add skip if player head is out of bounds or not, you will crash when you teleport to battlebus

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