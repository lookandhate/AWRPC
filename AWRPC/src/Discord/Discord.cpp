#include "Discord.h"
const char* MADE_BY = "Available at github.com/lookandhate/AWRPC";
void Discord::Initialize()
{
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(DiscordEventHandlers));
	Discord_Initialize("745649693144645766", &Handle, 1, NULL);

}

void Discord::Update(const char* state, const char* details, const char* mapName, int64_t startTimeStamp, MapType maptype)

{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	auto map_cover = GameModeEnumToCharCoverName(maptype);
	discordPresence.state = state;
	discordPresence.details = details;
	discordPresence.largeImageKey = mapName;
	discordPresence.largeImageText = MADE_BY;
	discordPresence.startTimestamp = startTimeStamp;
	discordPresence.smallImageKey = map_cover.c_str();
	Discord_UpdatePresence(&discordPresence);
}

std::string Discord::GameModeEnumToCharCoverName(MapType maptype)
{
	std::string map_cover;
	switch (maptype)
	{
	case MapType::EPvP:

		map_cover = std::string("pvp-purple");
		return map_cover;
	case MapType::EGlops:
		map_cover = std::string("glops");
		return map_cover;
	case MapType::EPvE:
		map_cover = std::string("pve");
		return map_cover;
	case MapType::EHangar:
		map_cover = std::string("cus_alabino_polygon");
		return map_cover;
	case MapType::EPvPGlops:
		map_cover = std::string("pvp-purple");
		return map_cover;

	default:
		map_cover = std::string("small_logo_white");
		return map_cover;
	}
	return map_cover;
}

void Discord::Unload()
{
	Discord_Shutdown();
}