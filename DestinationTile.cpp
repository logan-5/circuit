#include "DestinationTile.h"

DestinationTile::DestinationTile( int size, int x, int y, Dependent::dependenttype_e type ) : Tile( size, x, y, TILE_DESTINATION, false ), _type( type ), _occupied( false ) {
	this->initRepresentation();
}

DestinationTile::~DestinationTile() {
}

void DestinationTile::initRepresentation() {
	_representation = std::make_shared<SDLWrapper::Representation>( destinationTexture, x, y, _size, _size );
}