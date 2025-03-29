#pragma once

#include <windows.h>
#include <stdint.h>
#include <float.h>

class cache_class
{
public:
	uintptr_t UWorld;
	uintptr_t GameInstance;
	uintptr_t GameState;
	uintptr_t LocalPlayer;
	uintptr_t PlayerController;
	uintptr_t AcknowledgedPawn;
	uintptr_t Mesh;
	uintptr_t PlayerState;
	uintptr_t RootComponent;
	uintptr_t PlayerArray;

	int PlayerArraySize;
}; inline cache_class cache;

class Entity_class
{
public:
	uintptr_t Entity;
	uintptr_t SkeletalMesh;
	uintptr_t RootComponent;
	uintptr_t PlayerState;
};

std::vector<Entity_class> EntityList;
std::vector<Entity_class> TempEntityList;