// Windows
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

// Custom
#include "engine.hpp"
#include "../utility/console/console.hpp"
#include "../utility/utility/utility.hpp"
// Auth

#include "../vendor/kernel/communications.hpp"
#include "../overlay/overlay.hpp"
#include "../game/actors/actors.hpp"

// Driver Bytes
#include "../utility/bytes/driver.hpp"
#include "../utility/bytes/mapper.hpp"

void engine_class::start( )
{
	std::string windows_version = utility.get_windows_version();
	const std::string build_date = utility.get_full_build_date();
	std::string loader_version = "Version: 1.0.0";

	std::string windows_text = "Windows Version: " + windows_version;
	std::string build_date_text = "Build Date: " + build_date;
  
	console.setconsoletitle("fn-base");

	console.writeline("Welcome to fn-base\n", true);
	console.sleep(1000);
	console.writeline(windows_text, true);
	console.writeline(build_date_text, true);
	console.writeline(loader_version, true);

    initialize();
}

void engine_class::load_driver( )
{
    // Yes this source is using shitty sofmains pasted driver, :sob:
    // Btw this is a very shit way of loading a driver, your downloading to disk, and anyone can grab your driver easily.
    // Also makes source very slow cause of the bytes.

    std::string driver_path = "C:\\Windows\\System32\\Tasks\\shitty_driver.sys";
    std::string mapper_path = "C:\\Windows\\System32\\Tasks\\shitty_mapper.exe";

    utility.WriteBytesToFile(driver_path, cheat_driver_bytes);
    utility.WriteBytesToFile(mapper_path, cheat_mapper_bytes);

    system("C:\\Windows\\System32\\Tasks\\shitty_mapper.exe C:\\Windows\\System32\\Tasks\\shitty_driver.sys");
}

void engine_class::initialize( )
{
    console.setconsoletitle("fn-base - loader");

    if (!driver.FindDriverHandle())
    {
        console.writeline("Driver not found!", true);
        console.beep(325, 1000);
        console.sleep(2000);
        load_driver();
        console.writeline("Loaded driver success! - FULL DETECTED", true);
        console.sleep(2000);
    }

    console.writeline("Waiting for FortniteClient-Win64-Shipping.exe", true);
    HWND FortniteWindow = NULL;
    while (FortniteWindow == NULL)
    {
        FortniteWindow = FindWindowA(0, "Fortnite  ");
    }
    console.writeline("FortniteClient-Win64-Shipping.exe Found!", true);

    driver.process_id = driver.FindProcess(L"FortniteClient-Win64-Shipping.exe");
    memory.base_address = driver.FindImage();
    driver.DecryptCr3();

    std::thread([&]()
    {
        while (true)
        {
            actors.actor_cache();
        }
    }).detach();

    overlay.initialize();
}