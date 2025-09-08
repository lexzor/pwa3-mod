#include "memory.h"

bool DLLMemory::LoadOriginalDLL(const char* path)
{
	m_ModuleHandle = LoadLibraryA(path);
	
	return m_ModuleHandle != NULL;
}

const uintptr_t DLLMemory::RVA(uintptr_t address) const
{
	return (uintptr_t) m_ModuleHandle + address;
}
