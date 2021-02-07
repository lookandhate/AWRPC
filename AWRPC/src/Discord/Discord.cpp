#include "Discord.h"

void Discord::Initialize()
{
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(DiscordEventHandlers));
	Discord_Initialize("745649693144645766", &Handle, 1, NULL);

}

void Discord::Update(const char* state, const char* details, const char* mapName, MapType map)
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = state;
	discordPresence.details = details;
	discordPresence.largeImageKey = mapName;
	Discord_UpdatePresence(&discordPresence);
}

char* Discord::GameModeEnumToCharCoverName(MapType maptype)
{
	char* map_cover = new char;
	switch (maptype)
	{
	case MapType::EPvP:

		strcpy_s(map_cover, "pvp-purple");
		return map_cover;
	case MapType::EGlops:
		strcpy_s(map_cover, "glops");
		return map_cover;
	case MapType::EPvE:
		strcpy_s(map_cover, "pve");
		return map_cover;
	default:
		strcpy_s(map_cover, "small_logo_white");
		return map_cover;
	}
}

void Discord::Unload()
{
	Discord_Shutdown();
}