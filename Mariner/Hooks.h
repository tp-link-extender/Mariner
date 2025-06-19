#ifndef HOOKS_H
#define HOOKS_H

#include <string>
#include <Windows.h>
#include <map>

typedef std::map<const char*, std::string*> stringTable_t;

typedef struct {
	LPCWSTR module;
	const char* name;
	DWORD addrStudio;
	DWORD addrPlayer;
	LPVOID addrFunc;
	LPVOID detour;
	LPVOID* ret;
} Hook;

class Hooks
{
public:
	static const DWORD TrustCheckStudio;
	static const DWORD DoHttpReqStudio;
	static const DWORD TrustCheckPlayer;
	static const DWORD DoHttpReqPlayer;
	static const DWORD ModuleBase;
	static std::string GetClientName();
	static BOOL _cdecl DoTrustCheck(const char* url);
	static void(__thiscall* pfnDoHttpRewrite)(void*, void*, void*, void*, void*, void*, bool, void*);
	static void __fastcall DoHttpRewrite(void* _this, void*, void* a2, void* a3, void* a4, void* a5, void* a6, bool a7, void* a8);
	static void DoStringReplace(const char** text);
	static void(__thiscall* pfnQString__ctor)(void*, const char*);
	static void __fastcall QString__ctor(void* _this, void*, const char* text);
	static void* (__cdecl* pfnQCoreApplication__translate)(const char*, const char*, const char*, char, int);
	static void* __cdecl QCoreApplication__translate(const char* a1, const char* context, const char* sourceText, char encoding, int n);
	static BOOL(WINAPI* CreateDirectoryA_fp)(LPCSTR, LPSECURITY_ATTRIBUTES);
	static BOOL WINAPI CreateDirectoryA_hook(LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	static void* (__cdecl* sub_7128A0_fp)(std::string*, bool, bool, char*);
	static void* __cdecl sub_7128A0_hook(std::string* a1, bool a2, bool a3, char* a4);
	static void* (__cdecl* sub_404260_fp)(void* a1, void* a2, const char* a3);
	static void* __cdecl sub_404260_hook(void* a1, void* a2, const char* a3);
};

#endif