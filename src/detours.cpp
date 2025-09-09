#include "detours.h"

std::vector<DetourData> g_Detours =
{
    DetourData("SetGameAPIObject",  0x10020C90, &hSetGameAPIObject,     &oSetGameAPIObject),
    DetourData("GameAPIShutdown",   0x1001D9D0, &hGameAPIShutdown,      &oGameAPIShutDown),
};

GameAPIGenericType oSetGameAPIObject   = nullptr;
GameAPIGenericType oGameAPIShutDown    = nullptr;

void __cdecl hSetGameAPIObject(GameAPI* game_api)
{
    if (oSetGameAPIObject)
        oSetGameAPIObject(game_api);

    EventsManager::get()->TriggerEvent<OnGameStateEventData>(EventType::OnGameStart, { .game_api = game_api });
}

void __cdecl hGameAPIShutdown(GameAPI* game_api)
{
    if (oGameAPIShutDown)
        oGameAPIShutDown(game_api);

    EventsManager::get()->TriggerEvent<OnGameStateEventData>(EventType::OnGameShutdown, { .game_api = game_api });
    DLLMemory::get()->Uninitialize();
}