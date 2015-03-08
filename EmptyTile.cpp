#include "EmptyTile.h"


EmptyTile::EmptyTile( int size, int x, int y ) : Tile( size, x, y, TILE_EMPTY, true ) {
	this->initRepresentation();
}

EmptyTile::~EmptyTile() {
}

void EmptyTile::initRepresentation() {
	this->_representation = std::make_shared<SDLWrapper::Representation>( emptyTexture, x, y, _size, _size );
}