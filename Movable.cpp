#include "Movable.h"
#include "DestinationTile.h"
#include <cmath>
#define INT(x) static_cast<int>(round(x))

Movable::Movable( std::weak_ptr<Map> map, int gridX, int gridY, movertype_e _type, bool prototype ) : _map( map ), _gridX( gridX ), _gridY( gridY ), _type( _type ), _updating( false ), _updatetype( UPDATE_NONE ), _moveSteps( 10 ), _rotateSteps( 10 ) , _moveCounter( 0 ), _rotateCounter( 0 ) { 
	if ( !prototype ) {
		( *_map.lock() )( _gridX, _gridY )->block();
	}
}

bool Movable::turnToFace( int angle = 0 ) {
	if ( !_updating ) {
		if ( _angle == angle ) { return false; }
		_updating = true;
		_updatetype = UPDATE_ROTATE;
		_rotateStepValue = INT( fmin( abs( _angle - angle ), abs( _angle - ( angle + 360 ) ) ) ) / _rotateSteps;
		_oldAngle = _angle;
		_angle = angle;
	}

	int newAngle;
	if ( _rotateCounter >= _rotateSteps ) {
		_updating = false;
		newAngle = _angle;
		_updatetype = UPDATE_NONE;
		_rotateCounter = 0;
	} else {
		//int rotAngle = ( _angle - _oldAngle + 360 )%360;
		//int rotAngle = fmin( abs( _angle - _oldAngle ), abs( ( _oldAngle + 360 ) - _angle ) );
		int rotAngle = _angle - _oldAngle;
		if ( rotAngle >= 90 ) {
			newAngle = _oldAngle + _rotateStepValue*( ++_rotateCounter );
		} else {
			newAngle = _oldAngle - _rotateStepValue*( ++_rotateCounter );
		}
	}
	updateTarget();
	updateRepresentationWithAngle( newAngle );
	return true;
}

bool Movable::moveForward() {
	auto map = _map.lock();

	if ( !_updating ) {
		if ( _targetX < 0 || _targetX >= map->sizeX() || _targetY < 0 || _targetY >= map->sizeY() ) return false;

		_updating = true;
		_updatetype = UPDATE_MOVE;
		_moveStepValue = map->tileSize() / _moveSteps;
		auto target = ( *map )( _targetX, _targetY );
		if ( target->isBlocked() ) { _updating = false;  return false; }
		auto oldTile = ( *map )( _gridX, _gridY );
		oldTile->unblock();
		if ( DestinationTile* d = dynamic_cast<DestinationTile*>( oldTile.get() ) ) {
			d->empty();
		}
		_oldX = _gridX;
		_oldY = _gridY;
		_gridX = _targetX;
		_gridY = _targetY;
		target->block();
		updateTarget();
	}

	int newX, newY;
	if ( _moveCounter >= _moveSteps ) {
		_updating = false;
		newX = _gridX*map->tileSize();
		newY = _gridY*map->tileSize();
		_moveCounter = 0;
		_updatetype = UPDATE_NONE;
		if ( DestinationTile* d = dynamic_cast<DestinationTile*>( ( *map )( _gridX, _gridY ).get() ) ) {
			d->occupyWith( this );
		}
	} else {
		newX = _oldX*map->tileSize() + INT( cos( _angle*M_PI / 180 )*_moveStepValue*( ++_moveCounter ) );
		newY = _oldY*map->tileSize() - INT( sin( _angle*M_PI / 180 )*_moveStepValue*( _moveCounter ) );
	}
	updateRepresentationWithXY( newX, newY );
	return true;
}

void Movable::updateTarget() {
	auto map = _map.lock();

	int tileX = 0, tileY = 0;
	switch ( ( _angle / 90 ) * 90 ) {
	case 0:
		tileX = _gridX + 1, tileY = _gridY;
		break;
	case 90:
		tileX = _gridX, tileY = _gridY - 1;
		break;
	case 180:
		tileX = _gridX - 1, tileY = _gridY;
		break;
	case 270:
		tileX = _gridX, tileY = _gridY + 1;
		break;
	}

	_targetX = tileX, _targetY = tileY;
}

void Movable::updateRepresentation() {
	int tileSize = _map.lock()->tileSize();
	_representation->x = _gridX*tileSize;
	_representation->y = _gridY*tileSize;
	_representation->angle = _angle;
}

void Movable::updateRepresentationWithXY( int newX, int newY ) {
	int tileSize = _map.lock()->tileSize();
	_representation->x = newX;
	_representation->y = newY;
	_representation->angle = _angle;
}

void Movable::updateRepresentationWithAngle( int angle ) {
	int tileSize = _map.lock()->tileSize();
	_representation->x = _gridX*tileSize;
	_representation->y = _gridY*tileSize;
	_representation->angle = angle;
}

bool Movable::update() {
	if ( _updating ) {
		switch ( _updatetype ) {
		case UPDATE_NONE:
			_updating = false;
			break;
		case UPDATE_MOVE:
			moveForward();
			return true;
			break;
		case UPDATE_ROTATE:
			turnToFace();
			return true;
			break;
		}
	}
	return false;
}