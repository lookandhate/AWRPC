#include "Discord.h"

void Discord::Initialize()
{
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(DiscordEventHandlers));
	Discord_Initialize("745649693144645766", &Handle, 1, NULL);

}

void Discord::Update()
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = "Playing Solo";
	discordPresence.details = "Competitive";
	discordPresence.startTimestamp = 1507665886;
	discordPresence.endTimestamp = 1507665886;
	discordPresence.largeImageText = "Numbani";
	discordPresence.smallImageText = "Rogue - Level 100";
	Discord_UpdatePresence(&discordPresence);
}