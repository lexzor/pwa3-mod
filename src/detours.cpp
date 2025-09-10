#include "detours.h"
#include "commands_registry.h"
#include "memory.h"
#include "events_manager.h"

const std::vector<DetourData> g_Detours =
{
    { "SetGameAPIObject",           0x10020C90, (void*)&hSetGameAPIObject,      (void**)&oSetGameAPIObject      },
    { "GameAPIShutdown",            0x1001D9D0, (void*)&hGameAPIShutdown,       (void**)&oGameAPIShutDown       },
    { "Player::Chat",               0x100551A0, (void*)&hPlayerChat,            (void**)&oPlayerChat            },
    { "World::AddLocalPlayer",      0x10062C70, (void*)&hWorldAddLocalPlayer,   (void**)&oWorldAddLocalPlayer   }
};

// GameAPI(GameAPI*)

GameAPIGenericType oSetGameAPIObject   = nullptr;
GameAPIGenericType oGameAPIShutDown    = nullptr;

void __cdecl hSetGameAPIObject(GameAPI* game_api)
{
    EventsManager::get()->TriggerEvent<OnGameStateEventData>(EventType::OnGameStart, { .game_api = game_api });
    
    if (oSetGameAPIObject)
        oSetGameAPIObject(game_api);
}

void __cdecl hGameAPIShutdown(GameAPI* game_api)
{
    EventsManager::get()->TriggerEvent<OnGameStateEventData>(EventType::OnGameShutdown, { .game_api = game_api });
    
    if (oGameAPIShutDown)
        oGameAPIShutDown(game_api);

    DLLMemory::get()->Uninitialize();
}

// Player::Chat(Player*, const char*)
PlayerChatType oPlayerChat = nullptr;

void __fastcall hPlayerChat(Player* player, void* /*ignored*/, const char* text)
{
    if (!CommandsRegistry::get()->OnCommand(player, text))
    {
        if (oPlayerChat)
            oPlayerChat(player, nullptr, text);
    }
}

// World::AddLocalPlayer
WorldAddLocalPlayerType oWorldAddLocalPlayer = nullptr;

void __fastcall hWorldAddLocalPlayer(World* _this, void*, Player* player, ILocalPlayer* local_player)
{
    EventsManager::get()->TriggerEvent<OnAddLocalPlayerEventData>(EventType::OnAddLocalPlayer, {
        .world = _this,
        .player = player,
        .local_player = local_player
    });

    void** vtable = *(void***)_this;
    void* worldTickFn = vtable[1];  // World::Tick in vtable

    DLLMemory::get()->RegisterDetour((uintptr_t)worldTickFn, (void*)&hWorldTick, (void**)&oWorldTick, false);

    if (oWorldAddLocalPlayer)
        oWorldAddLocalPlayer(_this, player, local_player);
}

// World::Tick
using WorldTickType = void(__thiscall*)(World* _this, float deltaTime);
WorldTickType oWorldTick = nullptr;

void __fastcall hWorldTick(World* _this, void* /*ignored*/, float deltaTime)
{
    EventsManager::get()->TriggerEvent<OnWorldTickEventData>(EventType::OnWorldTick, {
        .world = _this,
        .delta_time = deltaTime
    });

    if (oWorldTick)
        oWorldTick(_this, deltaTime);
}