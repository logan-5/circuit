#pragma once
#include "Tile.h"
class EmptyTile : public Tile
{
public:
	static SDLWrapper::Texture emptyTexture;
	EmptyTile( int size, int x, int y );
	~EmptyTile();
private:
	void initRepresentation() override;
};

