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

bool DLLMemory::RegisterDetour(uintptr_t va_address, void* detour, void* original)
{
	Logger::Info("Registering detour {:p}", va_address);
	uintptr_t rva = RVA(va_address);

	MH_STATUS status = MH_CreateHook((LPVOID)rva,  (LPVOID)detour, (LPVOID*)original);

	if (status != MH_OK)
	{
		Logger::Error("Failed to detour function with address {:p} . Status: {}", va_address, MH_StatusToString(status));
		return false;
	}

	status = MH_EnableHook((LPVOID)rva);

	if (status != MH_OK)
	{
		Logger::Error("Failed to register hook for {:p} function. Status: {}", va_address, MH_StatusToString(status));
		return false;
	}

	m_HookRegistry.push_back(va_address);

	return true;
}

bool DLLMemory::RegisterDetour(const DetourData& data)
{
	uintptr_t rva = RVA(data.va_address);

	// Temporary storage for MinHook
	void* original_temp = nullptr;

	MH_STATUS status = MH_CreateHook(
		reinterpret_cast<LPVOID>(rva),
		data.hook_function,
		&original_temp   // Pass pointer to local variable
	);

	if (status != MH_OK)
	{
		Logger::Error("Failed to hook {:p} ({}). Status: {}", (void*)data.va_address, data.name, MH_StatusToString(status));
		return false;
	}

	status = MH_EnableHook(reinterpret_cast<LPVOID>(rva));
	if (status != MH_OK)
	{
		Logger::Error("Failed to enable hook {:p} ({}). Status: {}", (void*)data.va_address, data.name, MH_StatusToString(status));
		return false;
	}

	// Store the original function pointer inside the struct
	//data.original_function = original_temp;

	m_HookRegistry.push_back(data.va_address);
	return true;
}