#pragma once
#include <iostream>

#include "Windows.h"
#include <tlhelp32.h>
#include <tchar.h>

#include "Helpers/logger.h"

class CGameMemory
{
private:
	HANDLE m_hGameHabndle;
	const uintptr_t m_MAP_NAME_MEMORY_ADDRESS = 0x3350A0B;
	const uintptr_t m_NICKNAME_MEMORY_ADDRESS = 0x3149A5C;
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