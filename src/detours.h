#pragma once

#include <vector>
#include "sdk/GameLogic.h"

// Mod data detours 

struct DetourData
{
    DetourData(const char* n, uintptr_t v, void* h, void** o)
        : detour_name(n), va_address(v), hook_function(h), original_function(o) {
    }

    const char* detour_name;
    uintptr_t   va_address;
    void* hook_function;
    void** original_function;
};

extern const std::vector<DetourData> g_Detours;

// GameAPI
using GameAPIGenericType = void(__cdecl*)(GameAPI* game_api);

extern GameAPIGenericType oSetGameAPIObject;	// SetGameAPIObject (called when game starts and when game shutdown having GameAPI param as a nullptr)
extern GameAPIGenericType oGameAPIShutDown;		// GameAPI::Shutdown (called on game shutdown)

void __cdecl hSetGameAPIObject	(GameAPI* game_api);
void __cdecl hGameAPIShutdown	(GameAPI* game_api);

// Player::Chat(Player*, const char*)
using PlayerChatType = void(__fastcall*)(Player* _this, void* /*ignored*/, const char* text);
extern PlayerChatType oPlayerChat;

void __fastcall hPlayerChat(Player* player, void*, const char* text);

// World::AddLocalPlayer
using WorldAddLocalPlayerType = void(__thiscall*)(World* _this, Player* player, ILocalPlayer* local_player);
extern WorldAddLocalPlayerType oWorldAddLocalPlayer;

void __fastcall hWorldAddLocalPlayer(World* _this, void*, Player* player, ILocalPlayer* local_player);

// World::Tick
using WorldTickType = void(__thiscall*)(World* _this, float deltaTime);
extern WorldTickType oWorldTick;

void __fastcall hWorldTick(World* _this, void* /*ignored*/, float deltaTime);