#pragma once
#include <unordered_map>
#include "sdk/GameLogic.h"

enum class EventType
{
	OnGameStart,
};

struct OnGameStartEventData
{
	GameAPI* game_api;
};