#pragma once
#include <iostream>

#include "Windows.h"
#include <tlhelp32.h>
#include <tchar.h>

#include "Helpers/logger.h";

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


class CGameMemory
{
private:
	HANDLE m_hGameHabndle;
	const uintptr_t m_MAP_NAME_MEMORY_ADDRESS = 0x333758B;
	const uintptr_t m_NICKNAME_MEMORY_ADDRESS = 0x3130A5C;
	uintptr_t m_ptrGameBaseAddress;
	DWORD m_dwGamePID;


private:
	bool read_memory();


public:
	CGameMemory();
	bool GetCurrentMap(LPVOID buffer);
	bool GetPlayerNickname(LPVOID buffer);
	
	static DWORD GetPID(const TCHAR* const executableName);
	static uintptr_t GetBaseAddr(DWORD procID, const char* modName);
	static bool GetGameHandle(HANDLE& handle, DWORD procID);


	static bool isGameRunning(const char* const executableName);
	



};