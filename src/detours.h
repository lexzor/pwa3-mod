#pragma once
#include "memory.h"
#include "sdk/GameLogic.h"
#include "events_manager.h"

using GameAPIMainType = void(__cdecl*)(GameAPI*);

GameAPIMainType oSetGameAPIObject = nullptr;
GameAPIMainType oGameAPIShutDown = nullptr;

void __cdecl hSetGameAPIObject(GameAPI* game_api)
{
    if (oSetGameAPIObject)
        oSetGameAPIObject(game_api);

    EventsManager::get()->TriggerEvent<OnGameStartEventData>(EventType::OnGameStart, { .game_api = game_api });
}

void __cdecl hGameAPIShutdown(GameAPI* game_api)
{
    DLLMemory::get()->Uninitialize();

    if (oGameAPIShutDown)
        oGameAPIShutDown(game_api);
}