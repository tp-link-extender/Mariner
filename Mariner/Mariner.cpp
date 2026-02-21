#include "pch.h"
#include "Hooks.h"

#include <Windows.h>
#include <stdio.h>
#include <iostream>

__declspec(dllexport) void ImportMariner() {}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
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

        if (Hooks::GetClientName() == "MercuryStudioBeta.exe")
        {
            std::cout << "Client type: Studio\n";
            isStudio = TRUE;
        }
        else if (Hooks::GetClientName() == "MercuryPlayerBeta.exe")
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
              { NULL,           "HTTP Rewrite",                Hooks::DoHttpReqStudio,  Hooks::DoHttpReqPlayer,  NULL,             Hooks::DoHttpRewrite,               reinterpret_cast<LPVOID*>(&Hooks::pfnDoHttpRewrite) },
              { NULL,           "Trust Check",                 Hooks::TrustCheckStudio, Hooks::TrustCheckPlayer, NULL,             Hooks::DoTrustCheck,                NULL },
              { NULL,           "URL Check",                   0x19B700,                0x123EA0,                NULL,             Hooks::DoUrlCheck,                  NULL },
              { NULL,           "SSL Pinning",                 0x262B50,                NULL,                    NULL,             Hooks::sub_626B50_hook,             NULL },
           // { NULL,           "ProtectedString",             0x326770,                NULL,                    NULL,             Hooks::sub_726770_hook,             reinterpret_cast<LPVOID*>(&Hooks::sub_726770_fp) },
           // { L"qtcore4.dll", "QString::QString",            0x1777C0,                NULL,                    NULL,             Hooks::QString__ctor,               reinterpret_cast<LPVOID*>(&Hooks::pfnQString__ctor) },
           // { L"qtcore4.dll", "QCoreApplication::translate", 0x119530,                NULL,                    NULL,             Hooks::QCoreApplication__translate, reinterpret_cast<LPVOID*>(&Hooks::pfnQCoreApplication__translate) },
           // { NULL,           "CreateDirectoryA",            NULL,                    NULL,                    CreateDirectoryA, Hooks::CreateDirectoryA_hook,       reinterpret_cast<LPVOID*>(&Hooks::CreateDirectoryA_fp) },
           // { NULL,           "sub_7128A0",                  0x3128A0,                0x2FE010,                NULL,             Hooks::sub_7128A0_hook,             reinterpret_cast<LPVOID*>(&Hooks::sub_7128A0_fp) },
           // { NULL,           "sub_404260",                  0x004260,                NULL,                    NULL,             Hooks::sub_404260_hook,             reinterpret_cast<LPVOID*>(&Hooks::sub_404260_fp) },
        };

        for (int i = 0; i < sizeof(hooks) / sizeof(hooks[0]); i++)
        {
            LPVOID targetAddress = hooks[i].addrFunc;

            if (targetAddress == NULL)
            {
                targetAddress = reinterpret_cast<LPVOID>((DWORD)GetModuleHandle(hooks[i].module) + (isStudio ? hooks[i].addrStudio : hooks[i].addrPlayer));
            }

            if (MH_CreateHook(targetAddress, hooks[i].detour, hooks[i].ret) == MH_OK && MH_EnableHook(targetAddress) == MH_OK)
            {
                printf("Hooked %s\n", hooks[i].name);
            }
            else
            {
                printf("Failed to hook %s\n", hooks[i].name);
            }
        }

        CreateMutexA(NULL, TRUE, "RobloxCrashDumpUploaderMutex");
    }

    return TRUE;
}

