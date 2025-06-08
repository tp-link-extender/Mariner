#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <Windows.h>

class Util
{
public:
	static const std::vector<std::string> AllowedHosts;
	static const char Domain[];
	static const char Name[];
	static const char ClientSettings[];
};

#endif