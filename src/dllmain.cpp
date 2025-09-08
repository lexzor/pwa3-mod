#include <windows.h>
#include <iostream>
#include "sdk/GameLogic.h"

HMODULE g_hOriginal = nullptr;

static void CreateDebugConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
    printf("Created debug console\n");
}

static DWORD WINAPI InitThread(LPVOID)
{
    g_hOriginal = LoadLibraryA("gamelogic_original.dll");

    if (g_hOriginal == NULL)
    {
        MessageBoxA(NULL, "Failed to find gamelogic_original.dll", "Error", MB_OKCANCEL | MB_ICONERROR);
        return 0;
    }

    CreateDebugConsole();

    uintptr_t dllBase = (uintptr_t)g_hOriginal;

    uintptr_t worldAddr = dllBase + 0x97D7C;
    ClientWorld* world = nullptr;
    world = *(ClientWorld**)worldAddr;

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinst);
        CreateThread(nullptr, 0, InitThread, nullptr, 0, nullptr);
    }
    return TRUE;
}