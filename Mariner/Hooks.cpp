#include "pch.h"
#include "Hooks.h"
#include "ada.h"
#include "Util.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>

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

void(__thiscall* Hooks::pfnDoHttpRewrite)(void*, void*, void*, void*, void*, void*, bool, void*);
void __fastcall Hooks::DoHttpRewrite(void* _this, void*, void* a2, void* a3, void* a4, void* a5, void* a6, bool a7, void* a8)
{
	std::cout << "[DoHttpRewrite]\n";

	std::string* url1 = reinterpret_cast<std::string*>((int)_this);
	std::string* url2 = reinterpret_cast<std::string*>((int)_this + 32);

	std::cout << "\turl1: " << *url1 << "\n";
	std::cout << "\turl2: " << *url2 << "\n";

	size_t pos = url2->find("roblox.com");
	if (pos != std::string::npos) {
		url2->replace(pos, 10, "xtcy.dev");
		std::cout << "\treplaced: " << *url2 << "\n";
	}

	std::cout << "\n";

	Hooks::pfnDoHttpRewrite(_this, a2, a3, a4, a5, a6, a7, a8);
}

BOOL _cdecl Hooks::DoTrustCheck(const char* url)
{
	std::cout << "[TrustCheck]: " << url << "\n";

	ada::result<ada::url_aggregator> _url = ada::parse<ada::url_aggregator>(url);
	if (!_url) {
		std::cout << "Failed to parse url: " << url << "\n";
		return FALSE;
	}

	// Check host against allowed hosts

	if (std::find(std::begin(Util::AllowedHosts), std::end(Util::AllowedHosts), _url->get_host()) != std::end(Util::AllowedHosts)) 
	{
		std::cout << _url->get_host() << " passed TrustCheck" << "\n";
		return TRUE;
	}

	return FALSE;
}