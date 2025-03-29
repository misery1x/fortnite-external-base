#pragma once

#include <windows.h>

namespace offsets
{
	// Cache
	inline uintptr_t UWorld = 0x1771C988;
	inline uintptr_t OwningGameInstance = 0x208;
	inline uintptr_t LocalPlayers = 0x38;
	inline uintptr_t PlayerController = 0x30;
	inline uintptr_t AcknowledgedPawn = 0x350;
	inline uintptr_t Mesh = 0x328;
	inline uintptr_t PlayerState = 0x2c8;
	inline uintptr_t TeamIndex = 0x1259;
	inline uintptr_t RootComponent = 0x1B0;
	inline uintptr_t GameState = 0x190;
	inline uintptr_t PlayerArray = 0x2C0;
	inline uintptr_t PawnPrivate = 0x320;

	// Getbone
	inline uintptr_t BoneArray = 0x5c8;
	inline uintptr_t BoneCache = 0x5D8;
	inline uintptr_t ComponentToWorld = 0x1e0;
}