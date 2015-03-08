#pragma once
#include <memory>
#include "Map.h"
#include "Drawable.h"
#include "Movable.h"
#include "Selectable.h"

class Dependent;

class Modulator : public Movable, public Selectable
{
	friend class Map;
public:
	static SDLWrapper::Texture	textureTemplate;
								Modulator( std::weak_ptr<Map> map, int gridX = 0, int gridY = 0, std::string code = "",	 bool prototype = false );
								~Modulator();

	void						addDependent( Dependent* d );
	void						turnDependents();
	bool						moveForward();
	bool						turnToFace( int angle );
	bool						select() override;
	void						deselect() override;
	std::string					code() { return _code; }
private:
	std::string					_code; // used by Map	
	std::vector<Dependent*>		_dependents;

	void						initRepresentation();
	void						adjustRepresentationIfSelected() override;
};

