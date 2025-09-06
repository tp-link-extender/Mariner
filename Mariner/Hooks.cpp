#include "pch.h"
#include "Hooks.h"
#include "ada.h"
#include "Util.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>

// Studio addresses
const DWORD Hooks::TrustCheckStudio = 0x2631E0;
const DWORD Hooks::DoHttpReqStudio = 0x4C59B0;

// Player addresses
const DWORD Hooks::TrustCheckPlayer = 0x231E70;
const DWORD Hooks::DoHttpReqPlayer = 0x4511D0;

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
	if (pos != std::string::npos)
	{
		std::cout << "[DoHttpRewrite] Rewriting " << *url2 << "\n";
		url2->replace(pos, 10, Util::Domain);
	}

	pos = url2->find(".robloxlabs.com");
	if (pos != std::string::npos)
	{
		std::cout << "[DoHttpRewrite] Rewriting " << *url2 << "\n";
		url2->replace(pos, 16, "");
	}

	std::cout << "\n";

	Hooks::pfnDoHttpRewrite(_this, a2, a3, a4, a5, a6, a7, a8);
}

BOOL CheckURL(const char* url)
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

BOOL _cdecl Hooks::DoTrustCheck(const char* url)
{
	return CheckURL(url);
}

BOOL _cdecl Hooks::DoUrlCheck(const char* url)
{
	return CheckURL(url);
}

static stringTable_t g_stringTable;

void Hooks::DoStringReplace(const char** text)
{
	// might be better if we explicitly define strings to replace
	// instead of having automatic string replacement like this?

	if (strstr(*text, "ROBLOX") == NULL && strstr(*text, "Roblox") == NULL && strstr(*text, "roblox.com") == NULL)
		return;

	// exclude Qt identifiers like 'QT_ROBLOX_STUDIO' or 'aboutRobloxAction'
	if (strstr(*text, "action") != NULL || strstr(*text, "Action") != NULL || strchr(*text, '_') != NULL)
		return;

	// exclude Qt resource strings like ':/images/RobloxStudioSplash.png'
	if (strncmp(*text, ":/", 2) == 0)
		return;

	// exclude legalese
	if (strstr(*text, "Online Building Toy") != NULL)
		return;

	// exclude file browser specs like 'Roblox Place Files (*.rbxl)'
	if (strstr(*text, "*.rbxl") != NULL)
		return;

	std::cout << "[DoStringReplace] Rewriting " << *text << "\n";

	std::string* str;

	stringTable_t::const_iterator it = g_stringTable.find(*text);
	if (it != g_stringTable.end())
	{
		// std::cout << "[" << func << "] Table hit\n";
		str = it->second;
	}
	else
	{
		// std::cout << "[" << func << "] Table miss\n";

		// heap allocation using 'new'
		str = new std::string(*text);

		size_t pos;
		while ((pos = str->find("ROBLOX")) != std::string::npos)
			str->replace(pos, 6, Util::Name);

		while ((pos = str->find("Roblox")) != std::string::npos)
			str->replace(pos, 6, Util::Name);

		g_stringTable.insert(std::make_pair(*text, str));
	}

	*text = str->c_str();
}

void* (__cdecl* Hooks::sub_404260_fp)(void* a1, void* a2, const char* a3);
void* __cdecl Hooks::sub_404260_hook(void* a1, void* a2, const char* a3)
{
	Hooks::DoStringReplace(&a3);
	return Hooks::sub_404260_fp(a1, a2, a3);
}

BOOL(WINAPI* Hooks::CreateDirectoryA_fp)(LPCSTR, LPSECURITY_ATTRIBUTES);
BOOL WINAPI Hooks::CreateDirectoryA_hook(LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	std::string str(lpPathName);

	size_t pos = str.rfind("Roblox");

	if (pos != std::string::npos)
	{
		std::cout << "[CreateDirectoryA] Rewriting path " << lpPathName << "\n";
		str.replace(pos, 6, Util::Name);
		lpPathName = str.c_str();
	}

	return Hooks::CreateDirectoryA_fp(lpPathName, lpSecurityAttributes);
}

// Path rewriting
void* (__cdecl* Hooks::sub_7128A0_fp)(std::string*, bool, bool, char*);
void* __cdecl Hooks::sub_7128A0_hook(std::string* a1, bool a2, bool a3, char* a4)
{
	void* result = Hooks::sub_7128A0_fp(a1, a2, a3, a4);

	size_t pos = a1->rfind("Roblox");

	if (pos != std::string::npos)
	{
		std::cout << "[sub_7128A0] Rewriting path " << *a1 << "\n";
		a1->replace(pos, 6, Util::Name);
	}

	return result;
}

void* (__cdecl* Hooks::pfnQCoreApplication__translate)(const char*, const char*, const char*, char, int);
void* __cdecl Hooks::QCoreApplication__translate(const char* a1, const char* context, const char* sourceText, char encoding, int n)
{
	// all qt-specific contexts begin with 'Q' so we 
	// can safely ignore those
	if (strchr(context, 'Q') == NULL)
		Hooks::DoStringReplace(&sourceText);

	return Hooks::pfnQCoreApplication__translate(a1, context, sourceText, encoding, n);
}

void(__thiscall* Hooks::pfnQString__ctor)(void*, const char*);
void __fastcall Hooks::QString__ctor(void* _this, void*, const char* text)
{
	Hooks::DoStringReplace(&text);
	Hooks::pfnQString__ctor(_this, text);
}