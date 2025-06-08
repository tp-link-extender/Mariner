#include "pch.h"
#include "Hooks.h"

#include <string>
#include <iostream>
#include <map>

const DWORD Hooks::ModuleBase = (DWORD)GetModuleHandle(NULL);

// Studio addresses
const LPVOID Hooks::TrustCheckStudio = reinterpret_cast<LPVOID>(Hooks::ModuleBase + 0x2631E0);
const LPVOID Hooks::DoHttpReqStudio = reinterpret_cast<LPVOID>(Hooks::ModuleBase + 0x4C59B0);

// Player addresses
const LPVOID Hooks::TrustCheckPlayer = reinterpret_cast<LPVOID>(Hooks::ModuleBase + 0x231E70);
const LPVOID Hooks::DoHttpReqPlayer = reinterpret_cast<LPVOID>(Hooks::ModuleBase + 0x4511D0);

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

BOOL _cdecl Hooks::DoTrustCheck(const char* url)
{
	std::cout << "[TrustCheck]: " << url << "\n";
	return TRUE;
}