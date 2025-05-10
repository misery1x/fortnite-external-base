#include "console.hpp"
#include "utility/color_lib.hpp"

void console_class::writeline_option(std::string text, int option, bool new_line)
{
	std::cout << rang::fg::white << "[" << rang::style::reset;
	std::cout << rang::fg::cyan << option << rang::style::reset;
	std::cout << rang::fg::white << "] " << rang::style::reset;

	if (new_line)
	{
		std::cout << text << std::endl;
	}
	else
	{
		std::cout << text;
	}
}

void console_class::writeline(std::string text, bool new_line)
{
	std::cout << rang::fg::white << "[" << rang::style::reset;
	std::cout << rang::fg::cyan << "+" << rang::style::reset;
	std::cout << rang::fg::white << "] " << rang::style::reset;

	if (new_line)
	{
		std::cout << text << std::endl;
	}
	else
	{
		std::cout << text;
	}
}

void console_class::enter_key()
{
	std::cout << rang::fg::white << "[" << rang::style::reset;
	std::cout << rang::fg::cyan << "+" << rang::style::reset;
	std::cout << rang::fg::white << "] License: " << rang::style::reset;
}

void console_class::writeline_loader(std::string text)
{
	std::cout << rang::fg::white << "[" << rang::style::reset;
	std::cout << rang::fg::blue << "Stackswise" << rang::style::reset;
	std::cout << rang::fg::white << "] " << rang::style::reset;

	std::cout << text << std::endl;
}

void console_class::cleanline()
{
	std::cout << "" << std::endl;
}

void console_class::setconsoletitle(std::string title)
{
	SetConsoleTitleA(title.c_str());
}

void console_class::clearconsole()
{
	system("cls");
}

void console_class::sleep(DWORD milliseconds)
{
	Sleep(milliseconds);
}

void console_class::beep(DWORD dwFreq, DWORD dwDuration)
{
	Beep(dwFreq, dwDuration);
}

void console_class::progress_bar(int progress, int total, int width)
{
    float percentage = static_cast<float>(progress) / total;
    int filled = static_cast<int>(width * percentage);
    int empty = width - filled;
    
    std::cout << rang::fg::green;
    for (int i = 0; i < filled; i++)
    {
        std::cout << "-";
    }
    std::cout << rang::fg::gray;
    for (int i = 0; i < empty; i++)
    {
        std::cout << "-";
    }
    std::cout << rang::style::reset << "\r";
    std::cout.flush();
}

void console_class::exit_process(UINT uExitCode)
{
	ExitProcess(uExitCode);
}