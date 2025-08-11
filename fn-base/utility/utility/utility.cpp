#pragma once

// Windows
#include <windows.h>
#include <iostream>
#include <string>
#include <map>
#include <fstream>

// Custom
#include "utility.hpp"

bool utility_class::WriteBytesToFile(const std::string& filePath, const std::vector<unsigned char>& data)
{
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile.is_open())
    {
        return false;
    }

    outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
    outFile.close();
    return true;
}

const std::map<std::string, std::string> month_map =
{
    {"Jan", "January"}, {"Feb", "February"}, {"Mar", "March"}, {"Apr", "April"},
    {"May", "May"}, {"Jun", "June"}, {"Jul", "July"}, {"Aug", "August"},
    {"Sep", "September"}, {"Oct", "October"}, {"Nov", "November"}, {"Dec", "December"}
};

std::string utility_class::get_full_build_date( )
{
    std::string date = __DATE__;
    std::string month_abbr = date.substr(0, 3);
    std::string day = date.substr(4, 2);
    std::string year = date.substr(7, 4);

    return month_map.at(month_abbr) + " " + day + ", " + year + " " + __TIME__;
}

std::string utility_class::get_windows_version( )
{
    typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOEXW);
    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    if (hMod)
    {
        RtlGetVersionPtr fn = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
        if (fn)
        {
            RTL_OSVERSIONINFOEXW osInfo = { 0 };
            osInfo.dwOSVersionInfoSize = sizeof(osInfo);
            if (fn(&osInfo) == 0)
            {
                return std::to_string(osInfo.dwMajorVersion) + "." +
                    std::to_string(osInfo.dwMinorVersion) + " (Build " +
                    std::to_string(osInfo.dwBuildNumber) + ")";
            }
        }
    }
    return "Unknown Windows Version";
}