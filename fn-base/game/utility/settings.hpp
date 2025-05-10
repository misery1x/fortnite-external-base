#pragma once

#include <windows.h>

class GLOBALS_SETTINGS
{
public:
	HWND WindowHandle;

	int ScreenWidth = 1920;
	int ScreenHeight = 1080;

	bool GUIWindow = true;
};

class AIMBOT_SETTINGS
{
public:
	bool Enable = false;
	bool DrawFov = false;
	int FieldOfView = 150;

	int SmoothingX = 6;
	int SmoothingY = 6;

	int Keybind = MK_RBUTTON;
};

class VISUALS_SETTINGS
{
public:
	bool Enable = false;
	bool Box = false;
	bool Skeleton = false;
	bool DebugBones = false;
	
	int RenderDistance = 269;
};

class MISC_SETTINGS
{
public:
	bool Watermark = true;
};

inline GLOBALS_SETTINGS* Globals = new GLOBALS_SETTINGS();
inline AIMBOT_SETTINGS* Aimbot = new AIMBOT_SETTINGS();
inline VISUALS_SETTINGS* Visuals = new VISUALS_SETTINGS();
inline MISC_SETTINGS* Misc = new MISC_SETTINGS();