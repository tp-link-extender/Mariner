#include "pch.h"
#include "Hooks.h"

#include <Windows.h>
#include <stdio.h>
#include <iostream>

__declspec(dllexport) void ImportMariner() {}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        FILE* stream;
        AllocConsole();
        freopen_s(&stream, "CONOUT$", "w", stdout);

        std::cout << "Mariner Hook\n";

        if (MH_Initialize() != MH_OK) {
            std::cout << "MH_Initialize() failed\n";
            return FALSE;
        }

        // Detect client type

        BOOL isStudio;

        if (Hooks::GetClientName() == "RobloxStudioBeta.dll.exe")
        {
            std::cout << "Client type: Studio\n";
            isStudio = TRUE;
        }
        else if (Hooks::GetClientName() == "RobloxPlayerBeta.dll.exe")
        {
            std::cout << "Client type: Player\n";
            isStudio = FALSE;
        }
        else 
        {
            std::cout << "Could not detect client type\n";
            return FALSE;
        }

    }

    return TRUE;
}

