#include "Map.h"
#include "EmptyTile.h"
#include "NormalTile.h"
#include "DestinationTile.h"
#include "CmapParser.h"
#include <sstream>
#include <fstream>
#include <algorithm>

Map::Map( int tileSize, std::string fileName, int xOffset, int yOffset ) : _tileSize( tileSize ), _xOffset( xOffset ), _yOffset( yOffset ), _displayString( "" ) {
	//this->initFromFile( fileName );
}

Map::Map( int sizeX, int sizeY, std::string fileName, int xOffset, int yOffset ) : _xOffset( xOffset ), _yOffset( yOffset ), _displayString( "" ) {
	// calculate the tile size dynamically, so open the file to count
	std::ifstream stream( fileName );
	int rowN = 0, columnN = 0;
	if ( !stream.is_open() ) {
		printf( "Failed to open CMAP file." );
	} else {		
		std::string dummy;
		while ( dummy != "end_decls" ) std::getline( stream, dummy ); // skip "header" rows

		while ( std::getline( stream, dummy ) ) {
			if ( columnN == 0 ) {
				std::string otherdummy;
				std::stringstream line( dummy );
				while ( std::getline( line, otherdummy, ' ' ) ) { ++columnN; }
			}
			++rowN;
		}
		stream.close();
		_tileSize = std::min( sizeX / columnN, sizeY / rowN );
		//this->initFromFile( fileName );
	}
}

Map::~Map() { }

std::forward_list<std::weak_ptr<Drawable>> Map::tileList() {
	std::forward_list<std::weak_ptr<Drawable>> list;
	for ( auto i = _map.begin(); i != _map.end(); ++i ) {
		for ( auto j = ( *i ).begin(); j < ( *i ).end(); ++j )
			list.push_front( *j );
	}
	return list;
}

std::vector<std::shared_ptr<Drawable>> Map::initFromFile( std::string fileName ) {
	_sizeX = 0, _sizeY = 0;
	std::vector<std::shared_ptr<Drawable>> objects;
	std::ifstream stream( fileName );
	if ( !stream.is_open() ) {
		printf( "Failed to open CMAP file." );
	} else {
		std::string row = "";
		int rowN = 0, columnN = 0;
		CmapParser parser( fileName, shared_from_this() );
		_displayString = parser.displayString;
		while ( row != "end_decls" ) std::getline( stream, row );

		while ( std::getline( stream, row ) ) {
			std::string tile;
			std::istringstream rowStream( row );

			this->_map.push_back( std::vector<std::shared_ptr<Tile>>() );
			while ( std::getline( rowStream, tile, ' ' ) ) {				
				if ( tile == "0" ) {
					_map[rowN].push_back( std::make_shared<EmptyTile>( _tileSize, _xOffset + columnN*_tileSize, _yOffset + rowN*_tileSize ) );
					++columnN;
				} else if ( tile == "2" ) {
					_map[rowN].push_back( std::make_shared<DestinationTile>( _tileSize, _xOffset + columnN*_tileSize, _yOffset + rowN*_tileSize ) );
					++columnN;
				} else {

					auto type = parser.types.find( tile );

					if ( type != parser.types.end() ) {
						std::shared_ptr<Drawable> obj;// = std::shared_ptr<Drawable>( ( *( *type ).second.get() ) );
						switch ( type->second->drawtype() ) {
						case Drawable::DRAWABLE_TILE: 
						{
							Tile* tile = static_cast<Tile*>( ( *type ).second.get() ); // google static_cast vs dynamic_cast
							// nested switch(), anyone?
							switch ( tile->tiletype() ) { // welcome to hell
								// create different instances depending on type
								// e.g. EmptyTile, DestinationTile
							case Tile::TILE_EMPTY:
								_map[rowN].push_back( std::make_shared<EmptyTile>( _tileSize, _xOffset + columnN*_tileSize, _yOffset + rowN*_tileSize ) );
								break;
							case Tile::TILE_NORMAL:
								_map[rowN].push_back( std::make_shared<NormalTile>( _tileSize, _xOffset + columnN*_tileSize, _yOffset + rowN*_tileSize ) );
								break;
							case Tile::TILE_DESTINATION:
								_map[rowN].push_back( std::make_shared<DestinationTile>( _tileSize, _xOffset + columnN*_tileSize, _yOffset + rowN*_tileSize ) );
								break;
							default:
								break;
							}
							// obj = std::make_shared<Tile>( ( *( *type ).second.get() ) ); // see above
							break;
						}
						case Drawable::DRAWABLE_DEPENDENT: 
						{
							_map[rowN].push_back( std::make_shared<NormalTile>( _tileSize, _xOffset + columnN*_tileSize, _yOffset + rowN*_tileSize ) );
							Dependent* dep = dynamic_cast<Dependent*>( ( *type ).second.get() );
							
							obj = std::make_shared<Dependent>( shared_from_this(), columnN, rowN, dep->relationAngle(), dep->dependenttype(), dep->_code );
							goto add_object;
							break;
						}
						case Drawable::DRAWABLE_MODULATOR: 
						{
							_map[rowN].push_back( std::make_shared<NormalTile>( _tileSize, _xOffset + columnN*_tileSize, _yOffset + rowN*_tileSize ) );
							Modulator* mod = dynamic_cast<Modulator*>( ( *type ).second.get() );
							obj = std::make_shared<Modulator>( shared_from_this(), columnN, rowN, mod->_code );
							goto add_object;
							break;
						}
						default: 
						{
							_map[rowN].push_back( std::make_shared<NormalTile>( _tileSize, _xOffset + columnN*_tileSize, _yOffset + rowN*_tileSize ) );
						add_object:
							objects.push_back( obj );
							break; 
						}
						}
						
					} else {
						// add a normal tile
						_map[rowN].push_back( std::make_shared<NormalTile>( _tileSize, _xOffset + columnN*_tileSize, _yOffset + rowN*_tileSize ) );
					}
					++columnN;
				}
				_sizeX = _sizeX > columnN ? _sizeX : columnN;
			}
			++rowN, columnN = 0, ++_sizeY;
		}
		stream.close();
	}
	return objects;
}