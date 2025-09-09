#include "commands_registry.h"
#include "utils/logger.h"
#include "memory.h"

void CommandsRegistry::Initialize()
{
	DLLMemory::get()->RegisterDetour(0x100551A0, &CommandsRegistry::hPlayerChat, &oPlayerChatHook);
}

void CommandsRegistry::RegisterCommand(const char* cmd, OnPlayerChatCallback callback)
{
	if (m_Registry.find(std::string(cmd)) != m_Registry.end())
	{
		Logger::Error("Command!p {}!d already exists!", cmd);
		return;
	}

	m_Registry[cmd].push_back(callback);

	Logger::Info("Registered command!p {}", cmd);
}

void CommandsRegistry::OnCommand(Player* player, const char* cmd)
{
	auto cmdIt = m_Registry.find(std::string(cmd));

	if (cmdIt == m_Registry.end())
	{
		if (oPlayerChatHook)
		{
			oPlayerChatHook(player, nullptr, cmd);
		}
		return;
	}

	for (auto& callback : cmdIt->second)
	{
		callback();
	}
}

void __fastcall CommandsRegistry::hPlayerChat(Player* player, void*, const char* text)
{
	CommandsRegistry::get()->OnCommand(player, text);
}