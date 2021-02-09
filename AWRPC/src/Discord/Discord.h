#pragma once
#include "discord_register.h"
#include "discord_rpc.h"
#include "Windows.h"
#include "Helpers/Helper.hpp"
#include <string>


class Discord
{
public:
	void Initialize();
	void Update(const char* state, const char* details, const char* mapName, MapType map = MapType::EHangar);
	std::string GameModeEnumToCharCoverName(MapType maptype);
	void Update(const char* state, const char* details, const char* mapName, int64_t startTimeStamp, MapType map = MapType::EHangar);
	void Unload();
};

