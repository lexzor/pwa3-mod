#include "commands_registry.h"
#include "utils/logger.h"
#include "memory.h"

void CommandsRegistry::RegisterCommand(const char* cmd, OnPlayerChatCallback callback)
{
	std::string cmdString = std::string(cmd);

	if (m_Registry.find(cmdString) != m_Registry.end())
	{
		Logger::Error("Command!p {}!d already exists!", cmdString);
		return;
	}

	m_Registry[cmdString].push_back(callback);

	Logger::Info("Registered command!p {}", cmdString);
}

bool CommandsRegistry::OnCommand(Player* player, const char* cmd)
{
	std::string cmdString = std::string(cmd);

	auto cmdIt = m_Registry.find(cmdString);

	if (cmdIt == m_Registry.end())
	{
		return false;
	}

	for (auto& callback : cmdIt->second)
	{
		callback();
	}

	return true;
}