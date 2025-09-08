#include <windows.h>
#include <iostream>
#include "sdk/GameLogic.h"
#include "utils/logger.h"

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

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode))
        {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
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

    Logger::SetOptions({
        .ColorPrefix = "!",
        .InfoPrefix = "",
        .WarningPrefix = "!y[WARNING]!d ",
        .ErrorPrefix = "!r[ERROR]!d ",
    });

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