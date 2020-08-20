
#include <iostream>
#include <string>
#include <map>

#include "Windows.h"
#include <tlhelp32.h>
#include <tchar.h>
#include "../Discord.h"

enum MapType
{
	EHangar = 0, EPvP, EGlops, EPvPGlops, EPvE
};

struct MapLocalization
{
	const std::string m_eng;
	const std::string m_rus;
	MapType m_mapType = EHangar;
	
	MapLocalization() :
		m_rus("NULL"), m_eng("NULL"), m_mapType(EHangar){};
	
	MapLocalization(std::string eng, std::string rus, MapType maptype=EHangar) :
		m_rus(rus), m_eng(eng), m_mapType(maptype){};

};

std::map<std::string, MapLocalization> levelLocalization = {
	{"cus_alabino_polygon" , MapLocalization("Alabino", "Алабино")},
	{"gar_base" , MapLocalization("Hangar", "Ангар")},
	{"gar_eeu_lean" , MapLocalization("Hangar", "Ангар")},
	{"gar_geom_thumbnail" , MapLocalization("Hangar", "Ангар")},
	{"gar_outdoor" , MapLocalization("Hangar", "Ангар")},
	{"gar_season_02" , MapLocalization("Hangar", "Ангар")},
	{"gar_season_03" , MapLocalization("Hangar", "Ангар")},
	{"gar_season_04" , MapLocalization("Hangar", "Ангар")},
	{"gar_season_05" , MapLocalization("Hangar", "Ангар")},
	{"gar_china" , MapLocalization("Hangar", "Ангар")},
	{"gar_china_spring_festival" , MapLocalization("Hangar", "Ангар")},
	{"gar_china_unniversary_alabino_polygon" , MapLocalization("Hangar", "Ангар")},
	{"gar_holiday_02" , MapLocalization("Hangar", "Ангар")},
	{"gar_holiday_03" , MapLocalization("Hangar", "Ангар")},
	{"gar_holiday04" , MapLocalization("Hangar", "Ангар")},
	{"gar_holiday05" , MapLocalization("Hangar", "Ангар")},
	{"gar_holiday06" , MapLocalization("Hangar", "Ангар")},
	{"gar_holiday" , MapLocalization("Hangar", "Ангар")},
	{"gar_nevada" , MapLocalization("Hangar", "Ангар")},

	{"glo01_barrendivide" , MapLocalization("Barren Divide", "Ткварчели")},
	{"glo05_ghostfield" , MapLocalization("Ghostfield", "Безмер")},
	{"glo06_narrows" , MapLocalization("Narrows", "Кошице")},
	{"glo07_roughneck" , MapLocalization("Roughneck", "Надым")},
	{"glo08_china" , MapLocalization("Baise", "Байсэ")},
	{"glo09_grassyfields" , MapLocalization("Pleternica", "Плетерница")},
	{"glo11_alpine" , MapLocalization("Grindelwald", "Гриндельвальд")},
	{"glo17_coruscant" , MapLocalization("Moscow", "Москва")},
	{"pvp01_coldstrike" , MapLocalization("Cold Strike", "Гори")},
	{"pvp02_riverpoint" , MapLocalization("River Point", "Мостар")},
	{"pvp05_pipelines" , MapLocalization("Pipelines", "Эль-Ариш")},
	{"pvp06_portstorm" , MapLocalization("Port Storm", "Умм-Каср")},
	{"pvp07_coastalthreat" , MapLocalization("Coastal Threat", "Батуми")},
	{"pvp09_reactor" , MapLocalization("Reactor", "Уризар")},
	{"pvp13_desertcrossing" , MapLocalization("Desert Crossing", "Суэцкий канал")},
	{"pvp14_lostisland" , MapLocalization("Lost Island", "Палау")},
	{"pvp15_frontline" , MapLocalization("Front Line", "Виста")},
	{"pvp18_canyon" , MapLocalization("Chemical Plant", "Аламо")},
	{"pvp19_tropicalcoast" , MapLocalization("Tropical Coast", "Порт-Антонио")},
	{"pvp20_panamacanal" , MapLocalization("Waterway", "Панамский канал")},
};

namespace MemUtils {
	constexpr uintptr_t MAP_NAME_MEMORY_ADDRESS = 53356283;

	bool IsGameRunning(const char* const executableName)
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
		while (!MemUtils::IsGameRunning("armoredwarfare.exe"))
		{
			Sleep(10000);
		}
	}
	std::cout << "Game is running\n";
	
	std::cout << "Initializing discord client...";
	/// Create DiscordSDK object and fill strings with temp data
	Discord* DiscordSDK = new Discord;
	char buffer[20] = "Nothing";
	std::string level = "Nothing";

	/// Get Game PID
	DWORD PID = MemUtils::GetPID("armoredwarfare.exe");
	std::cout << "Game PID is: " << PID << std::endl;
	
	/// Get base address of armoredwarfare.exe module
	auto BaseAddress = MemUtils::GetBaseAddr(PID, "armoredwarfare.exe");
	std::cout << "Base address: " << BaseAddress << std::endl;


	/// Get game handle
	std::cout << "Getting game handle.... ";
	HANDLE gamehandle;
	bool bHandleStatus = MemUtils::GetGameHandle(gamehandle, PID);
	std::cout << "Handle Status:" << bHandleStatus << std::endl;

	std::cout << "Press Enter to begin" << std::endl;
	std::cin.get();
	DiscordSDK->Initialize();
	while ((!MemUtils::IsGameRunning("armoredwarfare.exe")))
	{
		system("cls");
		bool bMapRead = GetCurrentMap(gamehandle, &buffer, BaseAddress);
		
		level = std::string(buffer);
		int slash_index = level.find('/');
		level = level.substr(0, slash_index);
		std::string eulocalizedlevel = levelLocalization[level].m_eng;

		if (levelLocalization[level].m_eng == "Hangar")
		{
			DiscordSDK->Update("Chilling in hangar", "Testing API");
		}

		std::string PlayingOnMapString = "Map " + levelLocalization[level].m_eng;
		DiscordSDK->Update(PlayingOnMapString.c_str(), "Playing");

		std::cout << "Is Map read: " << bMapRead << " Level: " <<  level << std::endl << " Localized Level(ENG): " << levelLocalization[level].m_eng << std::endl << " Localized Level(RUS): " << levelLocalization[level].m_rus << std::endl;
		Sleep(100);
	}
	std::cout << "Game isnt running\nPress anykey to close the program";
	std::cin.get();
}