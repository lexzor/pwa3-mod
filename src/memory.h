#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <MinHook.h>

#include "utils/logger.h"
#include "utils/singleton.h"

struct DetourData;

constexpr uintptr_t IDA_IMAGE_BASE = 0x10000000;

class DLLMemory : public Singleton<DLLMemory>
{
public:
	HMODULE m_ModuleHandle = NULL;
	std::vector<uintptr_t> m_HookRegistry = {};

public:
	void Uninitialize();
	bool InitializeMinHook(MH_STATUS& status);
	bool LoadOriginalDLL(const char* path);
	const HMODULE& GetModule() const;
	bool RegisterDetour(uintptr_t address, void* detour, void* original);
	bool RegisterDetour(const DetourData& data);

	template<typename T = uintptr_t>
	T RVA(uintptr_t va_address)
	{
		uintptr_t RVA = (uintptr_t)m_ModuleHandle - IDA_IMAGE_BASE + va_address;

		if constexpr (std::is_pointer_v<T>)
			return reinterpret_cast<T>(RVA);
		else
			return static_cast<T>(RVA);
	}
};