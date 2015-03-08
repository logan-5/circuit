#pragma once
#include <vector>
#include <forward_list>
#include <memory>
#include "Tile.h"

class Map : public std::enable_shared_from_this<Map>
{
	friend class CmapParser;
	typedef std::vector<std::vector<std::shared_ptr<Tile>>> map_t;
public:

	std::forward_list<std::weak_ptr<Drawable>>	tileList( );
												Map( int tileSize, std::string fileName, int xOffset = 0, int yOffset =					0 );
												Map( int sizeX, int sizeY, std::string fileName, int xOffset = 0, int					yOffset = 0 );
												~Map( );
	std::vector<std::shared_ptr<Drawable>>		initFromFile( std::string fileName );
	std::shared_ptr<Tile>						operator()( int x, int y ) { return _map[y][x]; }
	int											sizeX()	{ return _sizeX; }
	int											sizeY() { return _sizeY; }
	int											tileSize() { return _tileSize; }
	int											xOffset() { return _xOffset; }
	int											yOffset() { return _yOffset; }
	std::string									displayString() { return _displayString; }

private:	
	map_t										_map;
	int											_tileSize, _xOffset, _yOffset, _sizeX, _sizeY;
	std::string									_displayString;

	Map( );
};

