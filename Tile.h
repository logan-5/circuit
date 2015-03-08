#pragma once
#include "Drawable.h"

class Tile : public Drawable
{
public:
	enum tiletype_e { TILE_EMPTY, TILE_NORMAL, TILE_DESTINATION };
	Tile( int size, int x, int y, tiletype_e type = TILE_NORMAL, bool blocked = false );
	~Tile();
	tiletype_e tiletype() {	return _tiletype; }
	bool isBlocked() { return _blocked; }
	void block() { _blocked = true; }
	void unblock() { if( _tiletype!=TILE_EMPTY ) _blocked = false; }

protected:
	int _size, x, y;
	bool _blocked;
	tiletype_e _tiletype;
	virtual void initRepresentation() = 0;
};