#pragma once

#include <windows.h>
#include <iostream>
#include <cstdio>
#include "utils/logger.h"
#include "memory.h"
#include "detours.h"

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

    // Enable ANSI colors
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

    Logger::Info("!gSuccesfully created debug console");
}

static void RegisterDetours()
{
    for (const DetourData& dt_data : g_Detours)
    {
        if (!DLLMemory::get()->RegisterDetour(dt_data.va_address, dt_data.hook_function, dt_data.original_function, true))
        {
            Logger::Error("Failed to register detour at address {:p} for function {}", dt_data.va_address, dt_data.detour_name);
        }
        else
        {
            Logger::Info("!g[DETOURS]!d Registered detour for function!y {}!d.", dt_data.detour_name);
        }
    }
}