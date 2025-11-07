#pragma once

#include <windows.h>

namespace offsets
{
	// Cache
	inline uintptr_t UWorld = 0x16EA99B0;
	inline uintptr_t OwningGameInstance = 0x240;
	inline uintptr_t LocalPlayers = 0x38;
	inline uintptr_t PlayerController = 0x30;
	inline uintptr_t AcknowledgedPawn = 0x358;
	inline uintptr_t Mesh = 0x330;
	inline uintptr_t PlayerState = 0x2D0;
	inline uintptr_t TeamIndex = 0x11a9;
	inline uintptr_t RootComponent = 0x1b0;
	inline uintptr_t GameState = 0x1C8;
	inline uintptr_t PlayerArray = 0x2C8;
	inline uintptr_t PawnPrivate = 0x328;

	// Getbone
	inline uintptr_t BoneArray = 0x5E8;
	inline uintptr_t BoneCache = 0x5D8;
	inline uintptr_t ComponentToWorld = 0x1e0;
}
