#include <windows.h>
#include <iostream>
#include "sdk/GameLogic.h"
#include "memory.h"
#include "MinHook.h"
#include "dllmain.h"
#include "detours.h"
#include "commands_registry.h"
#include "events_manager.h"
#include "events.h"

class TestClass
{
private:
    ClientWorld* m_ClientWorld = nullptr;

public:
    TestClass()
    {
        CommandsRegistry::get()->RegisterCommand("/test", CMD_BIND(this, OnCommand));
        EventsManager::get()->RegisterEventCallback<OnGameStateEventData>(EventType::OnGameStart, EV_BIND(this, OnGameStart));
        EventsManager::get()->RegisterEventCallback<OnGameStateEventData>(EventType::OnGameShutdown, EV_BIND(this, OnGameShutdown));
        EventsManager::get()->RegisterEventCallback<OnAddLocalPlayerEventData>(EventType::OnAddLocalPlayer, EV_BIND(this, OnAddLocalPlayer));
        EventsManager::get()->RegisterEventCallback<OnWorldTickEventData>(EventType::OnWorldTick, EV_BIND(this, OnWorldTick));
    }

    void OnWorldTick(OnWorldTickEventData* data)
    {
        Logger::Info("World::Tick; deltaTime = {}", data->delta_time);

        static bool displayedMessage;

        if (!m_ClientWorld && !displayedMessage)
        {
            Logger::Error("m_ClientWorld does not exists!");
            displayedMessage = true;
            return;
        }

        IPlayer* iplayer = m_ClientWorld->m_activePlayer.m_object;
        Player* player = ((Player*)(iplayer));
        player->m_walkingSpeed = 99999.0f;
    }

    void OnAddLocalPlayer(OnAddLocalPlayerEventData* data)
    {
        Logger::Info("TestClass::OnAddLocalPlayer");

        m_ClientWorld = DLLMemory::get()->RVA<ClientWorld*>(0x10097D7C);
    }

    void OnCommand()
    {
        Logger::Info("TestClass::OnCommand");
    }

    void OnGameStart(OnGameStateEventData* data)
    {
        Logger::Info("TestClass::OnGameStart");
    }

    void OnGameShutdown(OnGameStateEventData* data)
    {
        Logger::Info("TestClass::OnGameShutdown");
    }
};

static DWORD WINAPI InitThread(LPVOID)
{
    // Configure logger
    Logger::SetOptions({
        .ColorPrefix = "!",
        .InfoPrefix = "",
        .WarningPrefix = "!y[WARNING]!d ",
        .ErrorPrefix = "!r[ERROR]!d ",
    });

    // Create console for debugging
    CreateDebugConsole();

    // Load original dll
    if (!DLLMemory::get()->LoadOriginalDLL("gamelogic_original.dll"))
    {
        Logger::Error("Failed to load gamelogic_original.dll. The game mods won't work. Please visit!p https://github.com/lexzor/pwa3-mod!d and follow installation steps.");
        return 0;
    }

    // Initialize 
    MH_STATUS status = MH_ERROR_DISABLED;
    if (!DLLMemory::get()->InitializeMinHook(status))
    {
        Logger::Error("Failed to initialize MinHook. Status: {}", MH_StatusToString(status));
        return -1;
    }

    RegisterDetours();

    TestClass test{};

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinst);
        CreateThread(nullptr, 0, InitThread, nullptr, 0, nullptr);
    }
    return TRUE;
}