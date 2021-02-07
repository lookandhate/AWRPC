#pragma once
#include "discord_register.h"
#include "discord_rpc.h"
#include "Windows.h"
#include "../Helper.hpp"


class Discord
{
public:
	void Initialize();
<<<<<<< HEAD
	void Update(const char* state, const char* details, const char* mapName, MapType map = MapType::EHangar);
	char* GameModeEnumToCharCoverName(MapType maptype);
=======
	void Update(const char* state, const char* details, const char* mapName, int64_t startTimeStamp);
>>>>>>> feauture/time
	void Unload();
};

