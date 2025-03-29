#pragma once

// Windows
#include <windows.h>
#include <string>

class console_class
{
public:
	// Writing
	void writeline_option(std::string text, int option, bool new_line);
	void writeline(std::string text, bool new_line);
	void writeline_loader(std::string text);
	void cleanline();
	
	// Utility
	void setconsoletitle(std::string title);

	void clearconsole();
	void sleep(DWORD milliseconds);
	void beep(DWORD dwFreq, DWORD dwDuration);
	void progress_bar(int progress, int total, int width = 50);
	void exit_process(UINT uExitCode);
}; inline console_class console;