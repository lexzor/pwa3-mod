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
	using PlayerChatTypeHook = void(__fastcall*)(Player* _this, void* /*ignored*/, const char* text);
	PlayerChatTypeHook oPlayerChatHook;

	std::unordered_map<std::string, std::vector<OnPlayerChatCallback>> m_Registry;

public:
	void Initialize();
	void RegisterCommand(const char* cmd, OnPlayerChatCallback callback);
	void OnCommand(Player* player, const char* cmd);
	static void __fastcall hPlayerChat(Player* player, void*, const char* text);
};