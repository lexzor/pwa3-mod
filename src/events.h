#pragma once
#include <unordered_map>
#include "sdk/GameLogic.h"

enum class EventType
{
	OnGameStart,
	OnGameShutdown,
	OnAddLocalPlayer,
	OnWorldTick
};

struct OnGameStateEventData
{
	GameAPI* game_api;
};

struct OnAddLocalPlayerEventData
{
	World* world;
	Player* player;
	ILocalPlayer* local_player;
};

struct OnWorldTickEventData
{
	World* world;
	float delta_time;
};