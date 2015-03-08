#pragma once
#include "Representation.h"

class Drawable {
public:
	enum										layer_e { MAP_LAYER, ENTITY_LAYER };
	enum										drawtype_e { DRAWABLE_TILE, DRAWABLE_MODULATOR, DRAWABLE_DEPENDENT };
												Drawable( layer_e layer = ENTITY_LAYER, drawtype_e type =								DRAWABLE_MODULATOR ) : _layer( layer ), _type( type ) {}
												~Drawable()	{}
	std::weak_ptr<SDLWrapper::Representation>	getRepresentation() { return _representation; }
	layer_e layer()								{ return _layer; }
	drawtype_e drawtype()						{ return _type; }

protected:
	std::shared_ptr<SDLWrapper::Representation>	_representation;
	layer_e										_layer;
	drawtype_e									_type;

	virtual void initRepresentation() = 0;
};