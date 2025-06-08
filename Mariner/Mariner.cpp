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

        // Begin detouring

        static const Hook hooks[] = {
            { "Trust Check", Hooks::TrustCheckStudio, Hooks::TrustCheckPlayer, Hooks::DoTrustCheck, NULL },
            { "HTTP Rewrite", Hooks::DoHttpReqStudio, Hooks::DoHttpReqPlayer, Hooks::DoHttpRewrite, reinterpret_cast<LPVOID*>(&Hooks::pfnDoHttpRewrite) }
        };

        for (int i = 0; i < sizeof(hooks) / sizeof(hooks[0]); i++)
        {
            if (MH_CreateHook(isStudio ? hooks[i].addrStudio : hooks[i].addrPlayer, hooks[i].detour, hooks[i].ret) == MH_OK && MH_EnableHook(isStudio ? hooks[i].addrStudio : hooks[i].addrPlayer) == MH_OK)
            {
                printf("Hooked %s\n", hooks[i].name);
            }
            else
            {
                printf("Failed to hook %s\n", hooks[i].name);
            }
        }
    }

    return TRUE;
}

