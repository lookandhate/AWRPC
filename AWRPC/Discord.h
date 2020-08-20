#pragma once
#include "discord_register.h"
#include "discord_rpc.h"
#include "Windows.h"


class Discord
{
public:
	void Initialize();
	void Update(const char* state, const char* details, const char* mapName);
	void Unload();
};

