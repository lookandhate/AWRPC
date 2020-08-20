#include "Discord.h"

void Discord::Initialize()
{
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(DiscordEventHandlers));
	Discord_Initialize("745649693144645766", &Handle, 1, NULL);

}

void Discord::Update(const char* state, const char* details)
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = state;
	discordPresence.details = details;
	discordPresence.largeImageKey = "small_logo_black";
	Discord_UpdatePresence(&discordPresence);
}