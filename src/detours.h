#pragma once
#include "memory.h"
#include "sdk/GameLogic.h"

using GameAPIMainType = void(__cdecl*)(GameAPI*);

GameAPIMainType oSetGameAPIObject = nullptr;
GameAPIMainType oGameAPIShutDown = nullptr;

void __cdecl hSetGameAPIObject(GameAPI* game_api)
{
    if (oSetGameAPIObject)
        oSetGameAPIObject(game_api);
}

void __cdecl hGameAPIShutdown(GameAPI* game_api)
{
    DLLMemory::get()->Uninitialize();

    if (oGameAPIShutDown)
        oGameAPIShutDown(game_api);
}