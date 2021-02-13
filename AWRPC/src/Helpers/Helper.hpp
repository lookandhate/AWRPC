#pragma once
#include <string>

#ifndef MAPTYPE
#define MAPTYPE
enum class MapType
{
	EHangar = 0, EPvP, EGlops, EPvPGlops, EPvE
};
#endif //MAPTYPE

extern const char* MADE_BY_GH;
extern const char* SUPPORTED_GAME_VERSION;

#ifndef MAPLOCALIZATION
#define MAPLOCALIZATION
struct MapLocalization
{
	const std::string m_eng;
	const std::string m_rus;
	MapType m_mapType = MapType::EHangar;

	MapLocalization() :
		m_rus("NULL"), m_eng("NULL"), m_mapType(MapType::EHangar) {};
	/// TODO: Replace Default value with garage
	MapLocalization(std::string eng, std::string rus, MapType maptype = MapType::EPvPGlops) :
		m_rus(rus), m_eng(eng), m_mapType(maptype) {};

};
#endif //MAPLOCALIZATION


