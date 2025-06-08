#include "pch.h"

#include <Windows.h>

__declspec(dllexport) void ImportMariner() {}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        MessageBoxA(0, (LPCSTR)"We shall blow New Jersey to smithereens!", (LPCSTR)"Chief Keef is calling", MB_ICONSTOP);
    }

    return TRUE;
}

