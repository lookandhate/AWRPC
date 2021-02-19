#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <chrono>

#include "Discord/Discord.h"

#include "Helpers/Memutils.h"
#include "Helpers/logger.h"
#include "loguru/loguru.cpp"

using namespace std::chrono_literals;

// Defines how often check is game running and for map updates
#ifdef _DEBUG
#define INTERVAL_CHECK_GAME_IS_RUNNING 10000ms
#define INTERVAL_UPDATE_MAP_INFO 100ms

std::string CURRENT_VERSION = std::string("1.2.0 debug build");

#else
#define INTERVAL_CHECK_GAME_IS_RUNNING 30000ms
#define INTERVAL_UPDATE_MAP_INFO 10000ms

std::string CURRENT_VERSION = std::string("1.2.0 release build");
#endif // _DEBUG




enum class Localization
{
	EEng = 0, ERu
};

void wait_for_game_to_launch()
{
	if (!CGameMemory::isGameRunning("armoredwarfare.exe"))
	{
		std::cout << "Game isn't running\n";
		LOG_F(WARNING, "Game is not running on program launch");
		while (!CGameMemory::isGameRunning("armoredwarfare.exe"))
		{
			std::this_thread::sleep_for(2000ms);
		}
	}
}


int main(int argc, char* argv[])
{
	// Initialize the logger
	loguru::init(argc, argv);
	loguru::add_file("awrpc.log", loguru::Truncate, loguru::Verbosity_INFO);
	LOG_F(INFO, "Launching app, version %s, supported game version is %s", CURRENT_VERSION.c_str(), SUPPORTED_GAME_VERSION);


	// TODO Cleanup main function and separate it to several functtions
	SetConsoleOutputCP(1251);

	// Make possible to change Localization using command args
	Localization lang = Localization::EEng;

	wait_for_game_to_launch();

	system("cls");
	std::cout << "Game is running\n";


	/// Create DiscordSDK object and fill strings with temp data
	LOG_F(INFO, "Initializing discord client...");
	Discord* DiscordSDK = new Discord;
	LOG_F(INFO, "Discord client initialized at 0x%p", DiscordSDK);

	char buffer[150] = "Nothing";
	char buffer_for_old[150] = "Nothing";
	char nickname_buffer[150] = "NicknamePlaceholder";
	// Represents do we need to send new data to discord on this iteration
	bool bNeedToBeUpdated = true;

	std::string level = "Nothing";

	CGameMemory GameMemory = CGameMemory();

	std::cout << "Press Enter to begin" << std::endl;
	std::cin.get();

	LOG_F(INFO, "Initializing DiscordSDK");
	std::time_t timestamp = std::time(nullptr);
	DiscordSDK->Initialize();
	
	system("cls");
	std::cout << "Working....\n";
	while ((GameMemory.isGameRunning("armoredwarfare.exe")))
	{

#ifdef _DEBUG
		system("cls");
#endif // _DEBUG
		LOG_F(INFO, "Reading current map....");
		bool bMapRead = GameMemory.GetCurrentMap(&buffer);
		if (!bMapRead)
			LOG_F(ERROR, "Could not read current map");
		else
			LOG_F(INFO, "Map has been read successfully");
		
		LOG_F(INFO, "Reading nickname....");
		bool bNicknameRead = GameMemory.GetPlayerNickname(&nickname_buffer);
		if (!bNicknameRead)
			LOG_F(ERROR, "Could not read nickname");
		else
			LOG_F(INFO, "Nickname has been read successfully");

		level = std::string(buffer);
		std::string old_level = std::string(buffer_for_old);
		
		if(level != old_level)
		{
			LOG_F(INFO, "Updating discord info because old level is not the same as new one (%s != %s)", old_level.c_str(), level.c_str());

			strcpy_s(buffer_for_old, buffer);
			old_level = level;
			timestamp = std::time(nullptr);
			bNeedToBeUpdated = true;

		}
		if (bNeedToBeUpdated)
		{
			size_t slash_index = level.find('/');
			level = level.substr(0, slash_index);

			std::string localizedlevel = levelLocalization[level].m_eng;
			LOG_F(INFO, "Read %s from memory, localized name is %s", level.c_str(), localizedlevel.c_str());


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
					DiscordSDK->Update(
						HangarSitting.c_str(),
						std::string("Running game version " + std::string(SUPPORTED_GAME_VERSION)).c_str(),
						"small_logo_black",
						(int64_t)timestamp
					);
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
			
			bNeedToBeUpdated = false;
		}
#ifdef _DEBUG
		std::cout << "Is Map read: " << bMapRead << " Level: " << level << std::endl << " Localized Level(ENG): " << levelLocalization[level].m_eng << std::endl << " Localized Level(RUS): " << levelLocalization[level].m_rus << std::endl;
#endif // _DEBUG
		LOG_F(INFO, "Sleeping for %d ms", INTERVAL_UPDATE_MAP_INFO);
		LOG_F(INFO, "===============================================================");
		std::this_thread::sleep_for(INTERVAL_UPDATE_MAP_INFO);
	}
	system("cls");


	LOG_F(INFO, "Game is not running...");
	LOG_F(INFO, "Unloading DiscordSDK");
	DiscordSDK->Unload();
	delete DiscordSDK;
	std::cout << "Game is not running\nPress any key to close the program";

	std::cin.get();
}