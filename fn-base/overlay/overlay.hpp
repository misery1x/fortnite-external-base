#pragma once

class overlay_class
{
public:
	void initialize();
	bool initialize_imgui();
	bool render_loop();
	void draw_loop();
	void menu_loop();
}; inline overlay_class overlay;