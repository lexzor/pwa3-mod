#include <windows.h>
#include <iostream>
#include "sdk/GameLogic.h"
#include "memory.h"
#include "MinHook.h"
#include "game.h"

using SetGameAPIObjectType = void(__cdecl*)(GameAPI*);
SetGameAPIObjectType oSetGameAPIObject = nullptr;

void __cdecl hSetGameAPIObject(GameAPI* game_api)
{
    if (oSetGameAPIObject)
        oSetGameAPIObject(game_api);
}

using GameAPIShutdownType = void(__cdecl*)(GameAPI*);
SetGameAPIObjectType oGameAPIShutDown = nullptr;

void __cdecl hGameAPIShutdown(GameAPI* game_api)
{
    DLLMemory::get()->Uninitialize();

    if (oGameAPIShutDown)
        oGameAPIShutDown(game_api);
}

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
    // Configure logger
    Logger::SetOptions({
        .ColorPrefix = "!",
        .InfoPrefix = "",
        .WarningPrefix = "!y[WARNING]!d ",
        .ErrorPrefix = "!r[ERROR]!d ",
    });

    // Create console for debugging
    CreateDebugConsole();

    // Load original dll
    if (!DLLMemory::get()->LoadOriginalDLL("gamelogic_original.dll"))
    {
        Logger::Error("Failed to load gamelogic_original.dll. The game mods won't work. Please visit!p https://github.com/lexzor/pwa3-mod!d and follow installation steps.");
        return 0;
    }

    // Initialize 
    MH_STATUS status = MH_ERROR_DISABLED;
    if (!DLLMemory::get()->InitializeMinHook(status))
    {
        Logger::Error("Failed to initialize MinHook. Status: {}", MH_StatusToString(status));
        return -1;
    }

    DLLMemory::get()->RegisterDetour(0x10020C90, &hSetGameAPIObject, &oSetGameAPIObject);
    DLLMemory::get()->RegisterDetour(0x1001D9D0, &hGameAPIShutdown, &oGameAPIShutDown);

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