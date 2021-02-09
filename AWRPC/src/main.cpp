
#include <iostream>
#include <string>
#include <map>
#include <ctime>

#include "Windows.h"
#include <tlhelp32.h>
#include <tchar.h>

#include "Discord/Discord.h"
#include "Helpers/Helper.hpp"
#include "Helpers/Memutils.h"

// Defines how often check is game running and for map updates
#ifdef _DEBUG
#define INTERVAL_CHECK_GAME_IS_RUNNING 10000
#define INTERVAL_UPDATE_MAP_INFO 100

#else
#define INTERVAL_CHECK_GAME_IS_RUNNING 30000
#define INTERVAL_UPDATE_MAP_INFO 10000
#endif // _DEBUG


enum class Localization
{
	EEng = 0, ERu
};



std::map<std::string, MapLocalization> levelLocalization = {
	// Special
	{"cus_alabino_polygon" , MapLocalization("Alabino", "Алабино", MapType::EPvPGlops)},
	// Hangars
	{"gar_base" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_eeu_lean" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_geom_thumbnail" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_outdoor" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_season_02" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_season_03" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_season_04" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_season_05" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_season_06" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_china" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_china_spring_festival" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_china_unniversary_alabino_polygon" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_holiday_02" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_holiday_03" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_holiday04" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_holiday05" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_holiday06" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_holiday" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	{"gar_nevada" , MapLocalization("Hangar", "Ангар", MapType::EHangar)},
	
	// PvP and GLOPS maps 
	{"glo01_barrendivide" , MapLocalization("Barren Divide", "Ткварчели", MapType::EGlops)},
	{"glo05_ghostfield" , MapLocalization("Ghostfield", "Безмер", MapType::EPvPGlops)},
	{"glo06_narrows" , MapLocalization("Narrows", "Кошице", MapType::EPvPGlops)},
	{"glo07_roughneck" , MapLocalization("Roughneck", "Надым", MapType::EPvPGlops)},
	{"glo08_china" , MapLocalization("Baise", "Байсэ", MapType::EPvP)},
	{"glo09_grassyfields" , MapLocalization("Pleternica", "Плетерница", MapType::EPvP)},
	{"glo11_alpine" , MapLocalization("Grindelwald", "Гриндельвальд", MapType::EGlops)},
	{"glo12_eutown" , MapLocalization("Salzburg", "Зальцбург", MapType::EPvPGlops)},
	{"glo17_coruscant" , MapLocalization("Moscow", "Москва", MapType::EPvP)},
	{"pvp01_coldstrike" , MapLocalization("Cold Strike", "Гори", MapType::EPvP)},
	{"pvp02_riverpoint" , MapLocalization("River Point", "Мостар", MapType::EPvP)},
	{"pvp05_pipelines" , MapLocalization("Pipelines", "Эль-Ариш", MapType::EPvP)},
	{"pvp06_portstorm" , MapLocalization("Port Storm", "Умм-Каср", MapType::EPvP)},
	{"pvp07_coastalthreat" , MapLocalization("Coastal Threat", "Батуми", MapType::EPvP)},
	{"pvp09_reactor" , MapLocalization("Reactor", "Уризар", MapType::EPvP)},
	{"pvp13_desertcrossing" , MapLocalization("Desert Crossing", "Суэцкий канал", MapType::EGlops)},
	{"pvp14_lostisland" , MapLocalization("Lost Island", "Палау", MapType::EPvP)},
	{"pvp15_frontline" , MapLocalization("Front Line", "Виста", MapType::EPvP)},
	{"pvp18_canyon" , MapLocalization("Chemical Plant", "Аламо", MapType::EPvP)},
	{"pvp19_tropicalcoast" , MapLocalization("Tropical Coast", "Порт-Антонио", MapType::EPvP)},
	{"pvp20_panamacanal" , MapLocalization("Waterway", "Панамский канал", MapType::EPvP)},
	{"glo18_jakku" , MapLocalization("Al Dabbah", "Ткварчели", MapType::EPvP)},
};

void wait_for_game_to_launch()
{
	if (!MemUtils::IsGameRunning("armoredwarfare.exe"))
	{
		std::cout << "Game isn't running\n";
		while (!MemUtils::IsGameRunning("armoredwarfare.exe"))
		{
			Sleep(INTERVAL_CHECK_GAME_IS_RUNNING);
		}
	}
}

void rpc_loop()
{

}

int main()
{
	// TODO Cleanup main function and separate it to several functtions
	SetConsoleOutputCP(1251);

	// Make possible to change Localization using command args
	Localization lang = Localization::EEng;

	wait_for_game_to_launch();

	system("cls");
	std::cout << "Game is running\n";

	std::cout << "Initializing discord client...";
	/// Create DiscordSDK object and fill strings with temp data
	Discord* DiscordSDK = new Discord;

	char buffer[150] = "Nothing";
	char buffer_for_old[150] = "Nothing";
	char nickname_buffer[150] = "NicknamePlaceholder";
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

	std::time_t timestamp = std::time(nullptr);
	DiscordSDK->Initialize();
	
	system("cls");
	std::cout << "Working....\n";
	while ((MemUtils::IsGameRunning("armoredwarfare.exe")))
	{

#ifdef _DEBUG
		system("cls");
#endif // _DEBUG

		bool bMapRead = GetCurrentMap(gamehandle, &buffer, BaseAddress);
		bool bNicknameRead = GetPlayerNickname(gamehandle, &nickname_buffer, BaseAddress);

		level = std::string(buffer);
		std::string old_level = std::string(buffer_for_old);
		if(level != old_level)
		{
			strcpy_s(buffer_for_old, buffer);
			old_level = level;
			timestamp = std::time(nullptr);

		}

		size_t slash_index = level.find('/');
		level = level.substr(0, slash_index);

		std::string localizedlevel = levelLocalization[level].m_eng;
		
		switch (lang)
		{
		case Localization::ERu:
			std::string eulocalizedlevel = levelLocalization[level].m_rus;
			break;
		}

		// Check if we are in the hangar
		if (levelLocalization[level].m_mapType == MapType::EHangar)
		{

#ifdef _DEBUG
			std::cout << levelLocalization[level].m_eng << "is type " << (int)levelLocalization[level].m_mapType << std::endl;
#endif // _DEBUG

			if (lang == Localization::EEng)
			{
				///TODO REPLACE small_logo_black
				std::string HangarSitting = std::string(nickname_buffer) + std::string(" is sitting in hangar");
				DiscordSDK->Update(HangarSitting.c_str(), "Testing API", "small_logo_black", (int64_t)timestamp);
			}
			else
				DiscordSDK->Update("В ангаре", "Проверка API", "small_logo_black", (int64_t)timestamp);
		}
		// We are not in the hangar
		else
		{
			if (lang == Localization::EEng)
			{
				std::string PlayingOnMapString = "Map: " + levelLocalization[level].m_eng;
				DiscordSDK->Update(PlayingOnMapString.c_str(), "Playing", level.c_str(), (int64_t)timestamp, levelLocalization[level].m_mapType);
			}
			else
			{
				std::string PlayingOnMapString = "Карта: " + levelLocalization[level].m_rus;
				DiscordSDK->Update(PlayingOnMapString.c_str(), "В бою", level.c_str(), (int64_t)timestamp, levelLocalization[level].m_mapType);
			}
		}
#ifdef _DEBUG
		std::cout << "Is Map read: " << bMapRead << " Level: " << level << std::endl << " Localized Level(ENG): " << levelLocalization[level].m_eng << std::endl << " Localized Level(RUS): " << levelLocalization[level].m_rus << std::endl;

#endif // _DEBUG
		Sleep(INTERVAL_UPDATE_MAP_INFO);
	}
	system("cls");

	DiscordSDK->Unload();
	delete DiscordSDK;

	std::cout << "Game is not running\nPress any key to close the program";

	std::cin.get();
}