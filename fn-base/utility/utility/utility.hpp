#pragma once

// Windows
#include <string>
#include <vector>
#include <fstream>

class utility_class
{
public:
	// Loader
	std::string get_windows_version( );
	std::string get_full_build_date( );

	// Driver
	bool WriteBytesToFile(const std::string& filePath, const std::vector<unsigned char>& data);
}; inline utility_class utility;