#include "CmapParser.h"

/*
	"Documentation" for CMAP 'file format':

	0 - empty tile
	1 - normal tile
	2 - destination tile [type: ALL]

	CMAP is space- and newline-delimited.  This parser is unforgiving to stray whitespace.
	CMAP is case-sensitive.

	to allow for potentially flexible and customizeable levels, you may 'declare' symbols at the top of the file
	that correspond to game objects.
	these declarations end with a line beginning with the keyword 'end_decls'
	any attempt to overwrite symbols 0, 1, or 2 will be ignored

	as of now, only these in-game objects are supported:
		Modulator, declares as such:
		[symbol] modulator [code]

		Dependent, declared as such:
		[symbol] dependent [type ( corresponding to Dependent::type_e )] [angle offset from modulator] [code]

		Tile, declared as such:
		[symbol] tile [type ( corresponding to Tile::type_e )] [blocked ( true/false )]

		Display string for the bottom of the screen while the level is in progress, declared as such:
		string [string to be displayed]

	You may then use these symbols in the map representation to refer to the specified object.
	NOTE: all Dependents are placed with a NORMAL tile under them.

	Example CMAP file:
		d dependent all 0
		e dependent all 90
		e2 dependent all 180
		% tile destination
		end_decls
		0 0 1 1 0 0
		1 1 d e 1 1
		1 1 % % 1 e
		0 0 % 1 0 0
*/

#include <sstream>
#include <fstream>

CmapParser::CmapParser( std::string filename, std::shared_ptr<Map> map ) : _map( map ) {
	std::ifstream stream( filename );
	if ( stream.is_open() ) {
		bool finished = false;
		std::string line;
		while ( std::getline( stream, line ) && !finished ) {
			std::stringstream words( line );
			std::string word;
			int wordcount = 0;
			std::string symbol;
			while ( std::getline( words, word, ' ' ) && !finished ) {
				if ( wordcount == 0 ) {
					if ( word == "end_decls" ) { 
						finished = true; break; 
					} else if ( word == "string" ) {
						std::getline( words, displayString );
						continue;
					} else if ( word == "0" || word == "1" || word == "2" ) {
						continue;
					} else {
						symbol = word;
					}
					
				} else if ( wordcount == 1 ) {
					if ( word == "modulator" ) {
						parseModulator( symbol, line );
					} else if ( word == "dependent" ) {
						parseDependent( symbol, line );
					} else if ( word == "tile" ) {
						parseTile( symbol, line );
					}
				} else if ( wordcount > 1 ) { break; }

				++wordcount;
			}
			wordcount = 0;
		}
		stream.close();
	}
}

CmapParser::~CmapParser() {
}

void CmapParser::parseModulator( std::string symbol, std::string line ) {
	std::stringstream words( line );
	std::string word;
	int wordcount = 0;
	while ( std::getline( words, word, ' ' ) ) {
		if ( wordcount == 2 ) {
			std::shared_ptr<Drawable> mod = std::make_shared<Modulator>( _map, 0, 0, word, true );
			types.insert( std::pair<std::string, std::shared_ptr<Drawable>>( symbol, mod ) );
			break;
		}
		++wordcount;
	}
}

void CmapParser::parseDependent( std::string symbol, std::string line ) {
	std::stringstream words( line );
	std::string word;
	int wordcount = 0;
	int angle;
	Dependent::dependenttype_e type = Dependent::ALL;
	while ( std::getline( words, word, ' ' ) ) {
		if ( wordcount == 2 ) {
			if ( word == "all" ) {
				type = Dependent::ALL;
			}
		} else if ( wordcount == 3 ) {
			angle = std::stoi( word );
		} else if ( wordcount == 4 ) {
			std::shared_ptr<Drawable> mod = std::make_shared<Dependent>( _map, 0, 0, angle, type, word, true );
			types.insert( std::pair<std::string, std::shared_ptr<Drawable>>( symbol, mod ) );
			break;
		}

		++wordcount;
	}
}

void CmapParser::parseTile( std::string symbol, std::string line ) {
	std::stringstream words( line );
	std::string word;
	int wordcount = 0;
	Tile::tiletype_e type;
	bool blocked = false;
	while ( std::getline( words, word, ' ' ) ) {
		if ( wordcount == 2 ) {
			if ( word == "normal" ) {
				type = Tile::TILE_NORMAL;
			} else if ( word == "empty" ) {
				type = Tile::TILE_EMPTY;
			} else if ( word == "destination" ) {
				type = Tile::TILE_DESTINATION;
			}
		} else if ( wordcount == 3 ) {
			if ( word == "true" ) { blocked = true; }
			std::shared_ptr<Drawable> mod;
			switch ( type ) {	
			default:
			case Tile::TILE_NORMAL:
				mod = std::make_shared<NormalTile>( 0, 0, 0 );
				break;
			case Tile::TILE_EMPTY:
				mod = std::make_shared<EmptyTile>( 0, 0, 0 );
				break;
			case Tile::TILE_DESTINATION:
				mod = std::make_shared<DestinationTile>( 0, 0, 0 );
				break;
			}
//			std::shared_ptr<Drawable> mod = std::make_shared<Tile>( 0, 0, 0, type, blocked );
			types.insert( std::pair<std::string, std::shared_ptr<Drawable>>( symbol, mod ) );
			break;
		}

		++wordcount;
	}
}