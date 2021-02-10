#include "Helpers/Memutils.h"
#include "Helpers/logger.h"
bool MemUtils::IsGameRunning(const char* const executableName)
{
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!Process32First(snapshot, &procEntry))
	{
		CloseHandle(snapshot);
		return false;
	}

	do
	{
		if (!_tcsicmp(procEntry.szExeFile, executableName))
		{
			CloseHandle(snapshot);
			return true;
		}
	} while (Process32Next(snapshot, &procEntry));
	CloseHandle(snapshot);
	return false;
}

DWORD MemUtils::GetPID(const TCHAR* const executableName)
{
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!Process32First(snapshot, &procEntry))
	{
		CloseHandle(snapshot);
		return 0;
	}

	do
	{
		if (!_tcsicmp(procEntry.szExeFile, executableName))
		{
			CloseHandle(snapshot);
			return procEntry.th32ProcessID;
		}
	} while (Process32Next(snapshot, &procEntry));
	CloseHandle(snapshot);
	return 0;
}

uintptr_t MemUtils::GetBaseAddr(DWORD procID, const char* modName)
{
	DWORD baseAddr = -1;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procID);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		std::cout << "INVALID HANDLE\n";
		return 0;
	}
	MODULEENTRY32 ModEntry;
	// Set dw_size to size of struct
	ModEntry.dwSize = sizeof(MODULEENTRY32);
	// Call Module32 first
	if (Module32First(hSnapshot, &ModEntry))
	{
		if (strcmp(modName, ModEntry.szModule))
		{
			CloseHandle(hSnapshot);
			//std::cout << "Name: " << ModEntry.szModule << "BaseAddr:" << ModEntry.modBaseAddr << "Catched";
			return (uintptr_t)ModEntry.modBaseAddr;
		}

	}
	else
	{
		// If the Process32First call failed, it means that there is no
		// process running in the first place, we can return directly.
		CloseHandle(hSnapshot);
		return 0;
	}
	// If we are here it means that the Module32First call returned TRUE, but the first module
	// wasn't the module that we were searching for. Now we can loop through the modules
	// using Module32Next
	while (Module32Next(hSnapshot, &ModEntry))
	{
		// We do the same check we did for Module32First
		if (strcmp(modName, ModEntry.szModule))
		{
			// If we are here it means that the module has been found, we can add the module to the vector
			// But first of all we have to close the snapshot handle!
			CloseHandle(hSnapshot);
			std::cout << "Name: " << ModEntry.szModule << "BaseAddr:" << ModEntry.modBaseAddr << "Catched";
			return (uintptr_t)ModEntry.modBaseAddr;
		}
	}
	// Continue loop while the Module32Next call returns TRUE meaning that there are still modules to check

	// If we are here it means that the module has not been found or that there are no modules to scan for anymore.
	// We can close the snapshot handle and return false.
	CloseHandle(hSnapshot);
	return 0;

}

bool MemUtils::GetGameHandle(HANDLE& gamehandle, DWORD procID)
{
	gamehandle = OpenProcess(PROCESS_VM_READ, false, procID);
	return true;
}

bool MemUtils::read_memory(HANDLE handle, uintptr_t address, LPVOID buffer, SIZE_T size)
{
	SIZE_T size_read;
	return !!::ReadProcessMemory(handle, LPCVOID(address), buffer, size, &size_read) && size_read > 0;
}


bool GetCurrentMap(HANDLE& gameHandle, LPVOID buffer, uintptr_t BaseAddress)
{
#ifdef _DEBUG
	std::cout << "Reading memory" << std::endl;
	std::cout << "Base address: " << BaseAddress << " Offset: " << MemUtils::MAP_NAME_MEMORY_ADDRESS;
	std::cout << " Base + Offset = " << BaseAddress + MemUtils::MAP_NAME_MEMORY_ADDRESS << std::endl;
#endif //_DEBUG

	SIZE_T size_read;
	return !!::ReadProcessMemory(gameHandle, LPCVOID(BaseAddress + MemUtils::MAP_NAME_MEMORY_ADDRESS), buffer, 20, &size_read) && size_read > 0;

}

bool GetPlayerNickname(HANDLE& gameHandle, LPVOID buffer, uintptr_t BaseAddress)
{
#ifdef _DEBUG
	std::cout << "Reading memory" << std::endl;
	std::cout << "Base address: " << BaseAddress << " Offset: " << MemUtils::MAP_NAME_MEMORY_ADDRESS;
	std::cout << " Base + Offset = " << BaseAddress + MemUtils::MAP_NAME_MEMORY_ADDRESS << std::endl;
#endif //_DEBUG

	SIZE_T size_read;
	return !!::ReadProcessMemory(gameHandle, LPCVOID(BaseAddress + MemUtils::NICKNAME_MEMORY_ADDRESS), buffer, 20, &size_read) && size_read > 0;

}


CGameMemory::CGameMemory()
{
	LOG_F(INFO, "Initializing CGameMemory at %p", this);

	LOG_F(INFO, "Getting game PID...");
	m_dwGamePID = GetPID("armoredwarfare.exe");
	LOG_F(INFO, "Game PID is %d", m_dwGamePID);

	LOG_F(INFO, "Getting game Base address....");
	m_ptrGameBaseAddress = GetBaseAddr(m_dwGamePID, "armoredwarfare.exe");
	LOG_F(INFO, "Game Base address is 0x%d", m_ptrGameBaseAddress);
	
	LOG_F(INFO, "Getting game handle....");
	bool bHandleStatus = GetGameHandle(m_hGameHabndle, m_dwGamePID);
	LOG_F(INFO, "Handle Status: %d", bHandleStatus);

}

DWORD CGameMemory::GetPID(const TCHAR* const executableName)
{
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!Process32First(snapshot, &procEntry))
	{
		CloseHandle(snapshot);
		return 0;
	}

	do
	{
		if (!_tcsicmp(procEntry.szExeFile, executableName))
		{
			CloseHandle(snapshot);
			return procEntry.th32ProcessID;
		}
	} while (Process32Next(snapshot, &procEntry));
	CloseHandle(snapshot);
	return 0;
}

uintptr_t CGameMemory::GetBaseAddr(DWORD procID, const char* modName)
{
	DWORD baseAddr = -1;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procID);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		std::cout << "INVALID HANDLE\n";
		return 0;
	}
	MODULEENTRY32 ModEntry;
	// Set dw_size to size of struct
	ModEntry.dwSize = sizeof(MODULEENTRY32);
	// Call Module32 first
	if (Module32First(hSnapshot, &ModEntry))
	{
		if (strcmp(modName, ModEntry.szModule))
		{
			CloseHandle(hSnapshot);
			//std::cout << "Name: " << ModEntry.szModule << "BaseAddr:" << ModEntry.modBaseAddr << "Catched";
			return (uintptr_t)ModEntry.modBaseAddr;
		}

	}
	else
	{
		// If the Process32First call failed, it means that there is no
		// process running in the first place, we can return directly.
		CloseHandle(hSnapshot);
		return 0;
	}
	// If we are here it means that the Module32First call returned TRUE, but the first module
	// wasn't the module that we were searching for. Now we can loop through the modules
	// using Module32Next
	while (Module32Next(hSnapshot, &ModEntry))
	{
		// We do the same check we did for Module32First
		if (strcmp(modName, ModEntry.szModule))
		{
			// If we are here it means that the module has been found, we can add the module to the vector
			// But first of all we have to close the snapshot handle!
			CloseHandle(hSnapshot);
			std::cout << "Name: " << ModEntry.szModule << "BaseAddr:" << ModEntry.modBaseAddr << "Catched";
			return (uintptr_t)ModEntry.modBaseAddr;
		}
	}
	// Continue loop while the Module32Next call returns TRUE meaning that there are still modules to check

	// If we are here it means that the module has not been found or that there are no modules to scan for anymore.
	// We can close the snapshot handle and return false.
	CloseHandle(hSnapshot);
	return 0;
}


bool CGameMemory::GetGameHandle(HANDLE& handle, DWORD procID)
{
	handle = OpenProcess(PROCESS_VM_READ, false, procID);
	return true;
}

bool CGameMemory::GetCurrentMap(LPVOID buffer)
{
	SIZE_T sizeRead;
	return !!::ReadProcessMemory(m_hGameHabndle, LPCVOID(m_ptrGameBaseAddress + m_MAP_NAME_MEMORY_ADDRESS), buffer, 20, &sizeRead) && sizeRead > 0;
}

bool CGameMemory::GetPlayerNickname(LPVOID buffer)
{
	SIZE_T sizeRead;
	return !!::ReadProcessMemory(m_hGameHabndle, LPCVOID(m_ptrGameBaseAddress + m_NICKNAME_MEMORY_ADDRESS), buffer, 20, &sizeRead) && sizeRead > 0;
}

bool CGameMemory::isGameRunning(const char* const executableName)
{
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!Process32First(snapshot, &procEntry))
	{
		CloseHandle(snapshot);
		return false;
	}

	do
	{
		if (!_tcsicmp(procEntry.szExeFile, executableName))
		{
			CloseHandle(snapshot);
			return true;
		}
	} while (Process32Next(snapshot, &procEntry));
	CloseHandle(snapshot);
	return false;
}