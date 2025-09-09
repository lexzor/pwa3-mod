#pragma once

#include <vector>
#include "memory.h"
#include "sdk/GameLogic.h"
#include "events_manager.h"

struct DetourData
{
	DetourData(const char* name, uintptr_t va_addr, void* hook_fn, void* org_fn) 
		: va_address(va_addr), hook_function(hook_fn), original_function(org_fn) {}

	const char* name;
	uintptr_t va_address;
	void* hook_function;
	void* original_function;
};

extern std::vector<DetourData> g_Detours;

using GameAPIGenericType = void(__cdecl*)(GameAPI*);

extern GameAPIGenericType oSetGameAPIObject;
extern GameAPIGenericType oGameAPIShutDown;

void __cdecl hSetGameAPIObject	(GameAPI* game_api);
void __cdecl hGameAPIShutdown	(GameAPI* game_api);