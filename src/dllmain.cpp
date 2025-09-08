#include <windows.h>
#include <iostream>
#include "sdk/GameLogic.h"
#include "utils/logger.h"
#include "memory.h"

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

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode))
        {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }

        Logger::Info("!gDebug console has been created succesfully");
    }
    else printf("Failed to get console handle");
}

static DWORD WINAPI InitThread(LPVOID)
{
    Logger::SetOptions({
        .ColorPrefix = "!",
        .InfoPrefix = "",
        .WarningPrefix = "!y[WARNING]!d ",
        .ErrorPrefix = "!r[ERROR]!d ",
    });

    CreateDebugConsole();

    if (!DLLMemory::get()->LoadOriginalDLL("gamelogic_riginal.dll"))
    {
        Logger::Error("Failed to load gamelogic_original.dll. The game mods won't work. Please visit!p https://github.com/lexzor/pwa3-mod!d and follow installation steps.");
        return 0;
    }

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