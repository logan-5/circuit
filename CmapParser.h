#pragma once
#include <string>
#include <map>
#include <memory>
#include "Drawable.h"
#include "Modulator.h"
#include "Tile.h"
#include "NormalTile.h"
#include "EmptyTile.h"
#include "DestinationTile.h"
#include "Dependent.h"
#include "Map.h"

class CmapParser
{
public:
	CmapParser( std::string filename, std::shared_ptr<Map> map );
	~CmapParser();
	std::map<std::string, std::shared_ptr<Drawable>> types;
	std::string displayString;
private:
	std::shared_ptr<Map> _map;
	void parseModulator( std::string symbol, std::string line );
	void parseDependent( std::string symbol, std::string line );
	void parseTile( std::string symbol, std::string line );
};