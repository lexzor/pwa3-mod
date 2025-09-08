#pragma once

#include "utils/singleton.h"
#include <Windows.h>

class DLLMemory : public Singleton<DLLMemory>
{
public:
	HMODULE m_ModuleHandle = NULL;

public:
	bool LoadOriginalDLL(const char* path);
	const uintptr_t RVA(uintptr_t address) const;
};