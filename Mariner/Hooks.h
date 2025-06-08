#ifndef HOOKS_H
#define HOOKS_H

#include <string>
#include <Windows.h>
#include <map>

typedef struct {
	const char* name;
	LPVOID addrStudio;
	LPVOID addrPlayer;
	LPVOID detour;
	LPVOID* ret;
} Hook;

class Hooks
{
public:
	static const LPVOID TrustCheckStudio;
	static const LPVOID DoHttpReqStudio;
	static const LPVOID TrustCheckPlayer;
	static const LPVOID DoHttpReqPlayer;
	static const DWORD ModuleBase;
	static std::string GetClientName();
	static BOOL _cdecl DoTrustCheck(const char* url);
	static void(__thiscall* pfnDoHttpRewrite)(void*, void*, void*, void*, void*, void*, bool, void*);
	static void __fastcall DoHttpRewrite(void* _this, void*, void* a2, void* a3, void* a4, void* a5, void* a6, bool a7, void* a8);
};

#endif