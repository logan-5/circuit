#pragma once
#include "Tile.h"
class NormalTile : public Tile
{
public:
	static SDLWrapper::Texture normalTexture;
	NormalTile( int size, int x, int y );
	~NormalTile();
private:
	void initRepresentation() override;
};

