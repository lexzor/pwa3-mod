#include <windows.h>
#include <Psapi.h>
#include "MinHook.h"
#include <iostream>

// Original function pointer
typedef void (__thiscall* tGameAPITick)(void* thisPtr, float deltaTime);
tGameAPITick oGameAPITick = nullptr;

void __fastcall hkGameAPITick(void* thisPtr, float deltaTime)
{
    oGameAPITick(thisPtr, deltaTime);
}

HMODULE g_hOriginal = nullptr;

void CreateDebugConsole();

DWORD WINAPI InitThread(LPVOID)
{
    g_hOriginal = LoadLibraryA("GameLogic_orig.dll");

    if (!g_hOriginal)
    {
        MessageBoxA(nullptr, "Failed to load GameLogic_orig.dll\n", "Error", MB_ICONERROR);
        return 0;
    }

    CreateDebugConsole();
    RegisterCallback("?Tick@GameAPI@@QAEXM@Z");

    //FARPROC addr = GetProcAddress(g_hOriginal, "?Tick@GameAPI@@QAEXM@Z");
    //if (!addr)
    //{
    //    printf("Failed to get GameAPI::Tick\n");
    //    return 0;
    //}

    //MH_STATUS status = MH_Initialize();
    //if (status != MH_OK)
    //{
    //    printf("Failed to initialize MinHook. Status:%s\n", MH_StatusToString(status));
    //    return 0;
    //}

    //if (MH_CreateHook(addr, &hkGameAPITick, reinterpret_cast<LPVOID*>(&oGameAPITick)) != MH_OK)
    //{
    //    printf("Failed to create hook\n");
    //    return 0;
    //}

    //MH_EnableHook(addr);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID) 
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinst);
        CreateThread(nullptr, 0, InitThread, nullptr, 0, nullptr);
    }
    return TRUE;
}

void CreateDebugConsole() {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);        // Redirect stdout to console
    freopen_s(&fp, "CONOUT$", "w", stderr);        // Redirect stderr to console
    freopen_s(&fp, "CONIN$", "r", stdin);          // Redirect stdin if needed
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
    printf("Created debug console\n");
}