#pragma once
#include <unordered_map>
#include "sdk/GameLogic.h"

enum class EventType
{
	OnGameStart,
	OnGameShutdown
};

struct OnGameStateEventData
{
	GameAPI* game_api;
};