#pragma once
#include "Map.h"
#include <memory>

class Movable : public virtual Drawable {
public:
	enum				movertype_e { MOVER_GENERIC, MOVER_MODULATOR };
						Movable( std::weak_ptr<Map> map, int gridX, int gridY, movertype_e _type = MOVER_GENERIC, bool prototype = false );
	virtual				~Movable() {};
	int					gridX() { return _gridX; }
	int					gridY() { return _gridY; }
	int					angle() { return _angle; }
	int					targetX() { return _targetX; }
	int					targetY() { return _targetY; }
	virtual bool		turnToFace( int angle );
	virtual bool		moveForward();
	virtual bool		update();

protected:
	std::weak_ptr<Map>	_map;
	int					_gridX, _gridY, _angle;
	int					_oldX, _oldY, _oldAngle;
	const int			_moveSteps, _rotateSteps;
	int					_moveStepValue, _moveCounter, _rotateStepValue, _rotateCounter;
	int					_targetX, _targetY;
	movertype_e			_type;
	bool				_updating;
	enum				updatetype_e { UPDATE_NONE, UPDATE_ROTATE, UPDATE_MOVE } _updatetype;

	void				updateRepresentation();
	void				updateRepresentationWithXY( int newX, int newY );
	void				updateRepresentationWithAngle( int angle );
	void				updateTarget();
};