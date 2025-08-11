#pragma once

// Windows
#include <windows.h>
#include <string>
#include <iostream>

class console_class
{
public:
	// Writing
	void writeline_option(std::string text, int option, bool new_line);
	void writeline(std::string text, bool new_line);
	void writeline_loader(std::string text);
	void cleanline();
	void enter_key();

	// Utility
	void setconsoletitle(std::string title);
	void clearconsole();
	void sleep(DWORD milliseconds);
	void beep(DWORD dwFreq, DWORD dwDuration);
	void progress_bar(int progress, int total, int width = 50);
	void exit_process(UINT uExitCode);

	// Debugging
	template<typename T>
	void WriteLinePointer(std::string Pointer, T Value)
	{
		std::cout << "[fn-base] " << Pointer << ": 0x" << std::hex << Value << std::dec << std::endl;
	}
}; inline console_class console;