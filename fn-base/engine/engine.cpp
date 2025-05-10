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
#include "../game/features/esp/actors.hpp"

// Driver Bytes
#include "../utility/bytes/driver.hpp"
#include "../utility/bytes/mapper.hpp"

// Auth
#include "../vendor/keyauth/auth.hpp"

using namespace KeyAuth;

std::string name = (""); // REPLACE WITH YOUR KEYAUTH DETAILS.
std::string ownerid = ("");
std::string version = ("1.0");
std::string url = ("https://keyauth.win/api/1.3/");
std::string path = ("");

api KeyAuthApp(name, ownerid, version, url, path);

void engine_class::start_auth()
{
    KeyAuthApp.init();
    if (!KeyAuthApp.response.success)
    {
        std::cout << ("Status: ") << KeyAuthApp.response.message;
        Sleep(1500);
        exit(1);
    }

    std::string Key;
    console.enter_key();
    std::cin >> Key;
    KeyAuthApp.license(Key);

    if (KeyAuthApp.response.message.empty()) exit(11);
    if (!KeyAuthApp.response.success)
    {
        std::cout << ("Status: ") << KeyAuthApp.response.message;
        Sleep(1500);
        exit(1);
    }
}

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
    std::cout << "" << std::endl;

    start_auth();
    initialize();
}

void engine_class::load_driver( )
{
    std::string driver_path = "C:\\Windows\\System32\\Tasks\\shitty_driver.sys";
    std::string mapper_path = "C:\\Windows\\System32\\Tasks\\shitty_mapper.exe";

    utility.WriteBytesToFile(driver_path, cheat_driver_bytes);
    utility.WriteBytesToFile(mapper_path, cheat_mapper_bytes);

    system("C:\\Windows\\System32\\Tasks\\shitty_mapper.exe C:\\Windows\\System32\\Tasks\\shitty_driver.sys");
}

void engine_class::initialize( )
{
    console.setconsoletitle("fn-base - loader");

    if (!Driver::Init())
    {
        console.writeline("Driver not found!", true);
        console.beep(325, 1000);
        console.sleep(2000);
        load_driver();
        console.writeline("Loaded driver success!", true);
        console.sleep(2000);
    }

    console.writeline("Waiting for FortniteClient-Win64-Shipping.exe", true);
    HWND FortniteWindow = NULL;
    while (FortniteWindow == NULL)
    {
        FortniteWindow = FindWindowA(0, "Fortnite  ");
    }
    console.writeline("FortniteClient-Win64-Shipping.exe Found!", true);

    Driver::ProcessID = Driver::FindProcess(L"FortniteClient-Win64-Shipping.exe");
    BaseAddress = Driver::GetBase();
    Driver::CR3();

    std::thread([&]()
    {
        while (true)
        {
            actors.actor_cache();
        }
    }).detach();

    overlay.initialize();
}