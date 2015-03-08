#include "NormalTile.h"
#include "Representation.h"

NormalTile::NormalTile( int size, int x, int y ) : Tile( size, x, y, TILE_NORMAL, false ) {
	this->initRepresentation();
}


NormalTile::~NormalTile() {
}

void NormalTile::initRepresentation() {
	_representation = std::make_shared<SDLWrapper::Representation>( normalTexture, x, y, _size, _size );
}