#include "pch.h"
#include "Hooks.h"

#include <string>
#include <iostream>

// https://learn.microsoft.com/en-gb/windows/win32/api/winver/nf-winver-getfileversioninfoa?redirectedfrom=MSDN
std::string Hooks::GetClientName()
{
	char path[MAX_PATH];
	BOOL result = GetModuleFileNameA(NULL, path, MAX_PATH);

	if (!result) {
		std::cout << "Failed to get module file name\n";
		return FALSE;
	}

	const char* fileName = strrchr(path, '\\');
	if (fileName != NULL) {
		fileName++;
	}
	else {
		fileName = path;
	}

	std::string fileN(fileName);

	return fileN;
}