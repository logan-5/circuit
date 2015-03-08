#pragma once
#include <memory>
#include "Map.h"
#include "Drawable.h"
#include "Movable.h"
#include "Selectable.h"
#include "Tile.h"

class Modulator;

class Dependent : public Movable, public Selectable
{
	friend class Map;
	friend class Modulator;
public:
	enum						dependenttype_e { ALL };
	static SDLWrapper::Texture	textureTemplate;
								Dependent( std::weak_ptr<Map> map, int gridX = 0, int gridY = 0, int relationAngle = 0,	 dependenttype_e type_e = ALL, std::string code = "", bool prototype = false );
								~Dependent();
	int							relationAngle() { return _relationAngle; }
	dependenttype_e				dependenttype() { return _type; }
	std::string					code() { return _code; }
	bool						select() override { return false; } // no

private:
	std::string					_code; // used by Map
	int							_relationAngle;	// what is this frigger's angle compared to its modulator
	dependenttype_e				_type; // what types of destination tiles will accept this frigger (not implemented)

	void						initRepresentation( );
};

