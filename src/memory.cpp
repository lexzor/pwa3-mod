#include "memory.h"
#include "detours.h"

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

bool DLLMemory::RegisterDetour(uintptr_t address, void* detour, void** original, const bool is_rva_address)
{
	if(is_rva_address)
		address = RVA(address);
    
	MH_STATUS status = MH_CreateHook((LPVOID)address, (LPVOID)detour, (LPVOID*)original);
    if (status != MH_OK)
    {
        Logger::Error("Failed to detour function with address {:p}. Status: {}", (void*)address, MH_StatusToString(status));
        return false;
    }

    status = MH_EnableHook((LPVOID)address);
    if (status != MH_OK)
    {
        Logger::Error("Failed to enable hook for {:p}. Status: {}", (void*)address, MH_StatusToString(status));
        return false;
    }

    m_HookRegistry.push_back(address);

    return true;
}