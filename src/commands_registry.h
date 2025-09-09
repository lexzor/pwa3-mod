#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include "utils/singleton.h"
#include <string>
#include "sdk/GameLogic.h"

#define CMD_BIND(obj, func) [obj](){ (obj)->func(); }

using OnPlayerChatCallback = std::function<void()>;

class CommandsRegistry : public Singleton<CommandsRegistry>
{
public:
		
private:
	std::unordered_map<std::string, std::vector<OnPlayerChatCallback>> m_Registry;

public:
	void Initialize();
	void RegisterCommand(const char* cmd, OnPlayerChatCallback callback);
	bool OnCommand(Player* player, const char* cmd);
};