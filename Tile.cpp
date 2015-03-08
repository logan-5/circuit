#include "Tile.h"

Tile::Tile( int size, int x, int y, tiletype_e type, bool blocked ) : Drawable( MAP_LAYER, DRAWABLE_TILE ), _size( size ), x( x ), y( y ), _tiletype( type ), _blocked( blocked ) {
}

Tile::~Tile() {}