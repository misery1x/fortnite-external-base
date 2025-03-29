#pragma once

#include <windows.h>

class globals_class
{
public:
	HWND WindowHandle;

	int ScreenWidth = 1920;
	int ScreenHeight = 1080;

	bool gui_window = true;
}; inline globals_class globals;

class visuals_class
{
public:
	bool box = false;
	
	int render_distance = 269;
}; inline visuals_class visuals;

class misc_class
{
public:
	bool show_fps = false;
}; inline misc_class misc;