#pragma once
#include "Tile.h"
#include "Dependent.h"

class DestinationTile : public Tile
{
public:
	static						SDLWrapper::Texture destinationTexture;
								DestinationTile( int size, int x, int y, Dependent::dependenttype_e type =	Dependent::ALL );
								~DestinationTile();
	Dependent::dependenttype_e	dependenttype() { return _type; }
	void						occupyWith( Drawable* d ) {	_occupier = d; _occupied = true;	}
	void						empty() { _occupier = nullptr; _occupied = false; }
	Drawable*					occupier() { return _occupied ? _occupier : nullptr; }

private:
	Dependent::dependenttype_e	_type;
	bool						_occupied;
	Drawable*					_occupier;

	void						initRepresentation( ) override;
};

