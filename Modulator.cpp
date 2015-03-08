#include "Modulator.h"
#include "Dependent.h"

/*
====================

Constructor

	Initialize all variables and call all parent constructors necessary.  Initialize onscreen representation.

====================
*/
Modulator::Modulator( std::weak_ptr<Map> map, int gridX, int gridY, std::string code, bool prototype ) : Drawable( ENTITY_LAYER, DRAWABLE_MODULATOR ), Movable( map, gridX, gridY, MOVER_MODULATOR, prototype ), _code( code ), Selectable(textureTemplate) {
	_angle = 90;
	initRepresentation();
	adjustRepresentationIfSelected();
}

/*
====================

Destructor

	Not much to see.

====================
*/
Modulator::~Modulator() {
}

/*
====================

initRepresentation

	Set up the onscreen Representation object with the appropriate texture and size.

====================
*/
void Modulator::initRepresentation() {
	auto map = _map.lock();
	int tileSize = map->tileSize();
	this->_representation = std::make_shared<SDLWrapper::Representation>( _texture, _gridX*tileSize, _gridY*tileSize, tileSize, tileSize );
}

/*
====================

adjustRepresentationIfSelected()

	If selected, turn this modulator and all associated dependents the 'selected' color (defined in Selectable.h).
	If not selected, revert to normal color.

====================
*/
void Modulator::adjustRepresentationIfSelected() {
	if ( selected ) {
		_representation->modulate( _modColor );
		for ( auto it : _dependents ) {
			it->_representation->modulate( _modColor );
		}
	} else {
		_representation->resetModulation();
		for ( auto it : _dependents ) {
			it->_representation->resetModulation();
		}
	}
}

/*
====================

select()

	Tell this modulator it is selected and change its color to indicate as much (as well as all its dependents' colors).

====================
*/
bool Modulator::select() {
	adjustRepresentationIfSelected();
	return Selectable::select();
}

/*
====================

deselect()

	Tell this modulator it is no longer selected and revert its colors to normal, along with those of its dependents.

====================
*/
void Modulator::deselect() {
	Selectable::deselect();
	adjustRepresentationIfSelected();
}

/*
====================

addDependent( Dependent* d )

	Add a Dependent object to the list of this modulator's dependents, effectively giving ownership of the dependent to this modulator.

====================
*/
void Modulator::addDependent( Dependent* d ) {
	_dependents.push_back( d );
}

/*
====================

turnDependents()

	Iteratively turn each of this modulator's dependents so they face the appropriate direction, based on the modulator's current angle + the dependents' respective relation angles.

====================
*/
void Modulator::turnDependents() {
	for ( auto& it : _dependents ) {
		if ( it->angle() != ( _angle + it->relationAngle() ) % 360 ) {
			it->turnToFace( ( _angle + it->relationAngle() ) % 360 );
		}
	}
}

/*
====================

checkTargetClashes( Movable* ent, std::vector<Dependent*> dependents )

	Iteratively determine if this modulator and/or any of its dependents will attempt to move into the same square as another upon the next move operation.  This is not collision detection; rather, this function checks if two or more objects are 'facing' the same square and so will attempt to acquire it next move.  This is not allowed, so a 'false' result may terminate the move operation.

====================
*/
bool checkTargetClashes( Movable* ent, std::vector<Dependent*> dependents ) {
	for ( auto& it : dependents ) {
		if ( ent != it &&
			ent->targetX() == it->targetX() && ent->targetY() == it->targetY() ) {
			return true;
		}
	}
	return false;
}

/*
====================

moveForward()

	Start the animation process of moving one square in the current facing direction (animation continued in Movable::moveForward()).  Start the process of moving all relevant dependents as well, to likewise be continued in Movable::moveForward().

====================
*/
bool Modulator::moveForward() {
	//turnDependents();
	bool first = !_updating;
	std::vector<Dependent*> movers;
	if ( first ) {
		// check if anything's target tile matches mine, if so then I can't move ( nor can my dependents )
		if ( checkTargetClashes( this, _dependents ) ) { return false; }

		// if not, make a list of all dependents who are NOT trying to move into the same square as someone else
		// those will be the only ones we move
		for ( auto& it : _dependents ) {
			if ( !checkTargetClashes( it, _dependents ) ) {
				movers.push_back( it );
			}
		}
	}
	bool ret = Movable::moveForward();
	if ( ret && first ) {
		first = false;
		for ( auto& it : movers ) {
			it->moveForward();
		}
	}
	return ret;
}

/*
====================

turnToFace( int angle )

	Start the process of turning to face the target angle, along with turning all dependents.  Continued in Movable::turnToFace( int angle ).

====================
*/
bool Modulator::turnToFace( int angle ) {
	adjustRepresentationIfSelected();
	bool ret = Movable::turnToFace( angle );
	turnDependents();
	return ret;
}