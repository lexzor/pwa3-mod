#include "memory.h"

bool DLLMemory::LoadOriginalDLL(const char* path)
{
	m_ModuleHandle = LoadLibraryA(path);
	
	return m_ModuleHandle != nullptr;
}

void DLLMemory::Uninitialize()
{
	for (const uintptr_t va_address : m_HookRegistry)
	{
		MH_DisableHook((LPVOID) va_address);
	}

	MH_Uninitialize();
}

bool DLLMemory::InitializeMinHook(MH_STATUS& status)
{
	status = MH_Initialize();

	if (status != MH_OK)
	{
		return false;
	}

	return true;
}

const HMODULE& DLLMemory::GetModule() const
{
	return m_ModuleHandle;
}

bool DLLMemory::RegisterDetour(const uintptr_t va_address, const void* detour, const void* original)
{
	uintptr_t rva = RVA(va_address);

	MH_STATUS status = MH_CreateHook((LPVOID)rva,  (LPVOID)detour, (LPVOID*)original);

	if (status != MH_OK)
	{
		Logger::Error("Failed to detour SetGameAPIObject function. Status: %s", MH_StatusToString(status));
		return false;
	}

	status = MH_EnableHook((LPVOID)rva);

	if (status != MH_OK)
	{
		Logger::Error("Failed to enable hook for SetGameAPIObject function. Status: %s", MH_StatusToString(status));
		return true;
	}

	m_HookRegistry.push_back(va_address);

	return false;
}
