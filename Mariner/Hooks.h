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
};

#endif