
#include "Windows.h"
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>

namespace MemUtils {
	constexpr uintptr_t MAP_NAME_MEMORY_ADDRESS = 53356283;

	bool IsGameRunning(const TCHAR* const executableName)
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

	DWORD GetPID(const TCHAR* const executableName)
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

	uintptr_t GetBaseAddr(DWORD procID, const char* modName)
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

	bool GetGameHandle(HANDLE& gamehandle, DWORD procID)
	{
		gamehandle = OpenProcess(PROCESS_VM_READ, false, procID);
		return true;
	}

	bool read_memory(HANDLE handle, uintptr_t address, LPVOID buffer, SIZE_T size)
	{
		SIZE_T size_read;
		return !!::ReadProcessMemory(handle, LPCVOID(address), buffer, size, &size_read) && size_read > 0;
	}
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

int main()
{
	if (!MemUtils::IsGameRunning("armoredwarfare.exe"))
	{
		std::cout << "Game isn't running";
		return 1;
	}

	char buffer[20] = "Nothing";

	std::cout << "Game is running\n";

	DWORD PID = MemUtils::GetPID("armoredwarfare.exe");
	std::cout << "Game PID is: " << PID << std::endl;

	auto BaseAddress = MemUtils::GetBaseAddr(PID, "armoredwarfare.exe");
	std::cout << "Base address: " << BaseAddress << std::endl;

	std::cout << "Getting game handle.... ";
	HANDLE gamehandle;
	bool bHandleStatus = MemUtils::GetGameHandle(gamehandle, PID);
	std::cout << "Handle Status:" << bHandleStatus << std::endl;


	bool bMapRead = GetCurrentMap(gamehandle, &buffer, BaseAddress);
	std::cout << "Is Map read: " << bMapRead << buffer << std::endl;

	std::cin.get();
	while (true)
	{
		system("cls");
		bool bMapRead = GetCurrentMap(gamehandle, &buffer, BaseAddress);
		std::cout << "Is Map read: " << bMapRead << " Buffer: " <<  buffer << std::endl;
		Sleep(100);
	}
}