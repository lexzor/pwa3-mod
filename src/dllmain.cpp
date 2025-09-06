#include <windows.h>
#include <iostream>

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

    CreateDebugConsole();

    return 0;
}

static BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinst);
        CreateThread(nullptr, 0, InitThread, nullptr, 0, nullptr);
    }
    return TRUE;
}