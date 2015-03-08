#pragma once
#include <forward_list>
#include <memory>
#include <vector>
#include "Drawable.h"
#include "Game.h"

class Renderer
{
public:
	Renderer( std::shared_ptr<SDLWrapper::Window> window, std::weak_ptr<Game> game );
	~Renderer();
	//void add( std::weak_ptr<Drawable> d );
	//void add( std::forward_list<std::weak_ptr<Drawable>> dList );
	//void remove( std::weak_ptr<Drawable> d );
	void drawFrame();

private:
	std::weak_ptr<Game> _game;
	std::shared_ptr<SDLWrapper::Window> _window;
	std::forward_list<std::weak_ptr<Drawable>> _drawList;

	//void cullList();
	void drawEntities( std::vector<std::shared_ptr<Drawable>> dList );
};

