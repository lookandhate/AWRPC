#pragma once
#include "Windows.h"
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>

namespace MemUtils
{
	constexpr uintptr_t MAP_NAME_MEMORY_ADDRESS = 0x333758B;
	constexpr uintptr_t NICKNAME_MEMORY_ADDRESS = 0x3130A5C;
	bool IsGameRunning(const char* const executableName);
	DWORD GetPID(const TCHAR* const executableName);
	uintptr_t GetBaseAddr(DWORD procID, const char* modName);
	bool GetGameHandle(HANDLE& gamehandle, DWORD procID);
	bool read_memory(HANDLE handle, uintptr_t address, LPVOID buffer, SIZE_T size);
}

bool GetCurrentMap(HANDLE& gameHandle, LPVOID buffer, uintptr_t BaseAddress);
bool GetPlayerNickname(HANDLE& gameHandle, LPVOID buffer, uintptr_t BaseAddress);