#pragma once
#include <SDL.h>
#include <memory>
#include "Selectable.h"
#include "Movable.h"
#include "Modulator.h"
#include "Dependent.h"

class Game
{
	friend class Renderer;
public:
	Game( const int fps = 60, int screenSizeX = 800, int screenSizeY = 600, std::string mapsFile = "maps.txt" );
	Game( std::weak_ptr<Map> map, const int fps = 60, int screenSizeX = 800, int screenSizeY = 600 ); // for testing purposes only
	~Game();

	inline const int fps() const { return _fps; }

	void handleInput( const SDL_Event* e );
	void update();
private:
	const int _fps, _screenSizeX, _screenSizeY;
	bool _animating;

	std::vector<std::string> _mapFilePaths;
	std::vector<std::string>::iterator _currentMapPath;
	std::shared_ptr<Map> _currentMap;
	void loadMap( std::string fileName );
	void addObjectsFromMap( std::vector<std::shared_ptr<Drawable>> objects );

	Selectable* _selected;
	std::shared_ptr<Renderer> _renderer;

	std::forward_list<std::shared_ptr<Drawable>> _drawList;
};

