#include "Dependent.h"

Dependent::Dependent( std::weak_ptr<Map> map, int gridX, int gridY, int relationAngle, dependenttype_e type, std::string code, bool prototype ) : Drawable( ENTITY_LAYER, DRAWABLE_DEPENDENT ), Movable( map, gridX, gridY, MOVER_GENERIC, prototype ), _relationAngle( relationAngle ), _type( type ), _code( code ), Selectable(textureTemplate) {
	_angle = 90;
	initRepresentation();
}


Dependent::~Dependent() {
}

void Dependent::initRepresentation() {
	auto map = _map.lock();
	int tileSize = map->tileSize();
	_representation = std::make_shared<SDLWrapper::Representation>( _texture, _gridX*tileSize, _gridY*tileSize, tileSize, tileSize );
}