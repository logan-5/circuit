#include "Renderer.h"
#include <vector>
#include <functional>

Renderer::Renderer( std::shared_ptr<SDLWrapper::Window> window, std::weak_ptr<Game> game ) : _window( window ), _game( game ) {
}


Renderer::~Renderer() {
}
/*
void Renderer::add( std::weak_ptr<Drawable> d ) {
	_drawList.push_front( d.lock() );
}

void Renderer::add( std::forward_list<std::weak_ptr<Drawable>> dList ) {
	for ( std::weak_ptr<Drawable> d : dList )
		this->add( d.lock() );
}

bool match( std::weak_ptr<Drawable>  a, std::weak_ptr<Drawable>  b ) {	return a.lock() == b.lock(); }
void Renderer::remove( std::weak_ptr<Drawable> d ) {
	_drawList.remove_if( std::tr1::bind( match, d,
		std::tr1::placeholders::_1 ) ); // thank you sooo much stackoverflow
}

void Renderer::drawEntities( std::vector<std::shared_ptr<Drawable>> dList ) {
	for ( auto d : dList )
		_window->draw( d->getRepresentation() );
}

void Renderer::drawFrame() {
	_window->clear();
	this->cullList();
	std::vector<std::shared_ptr<Drawable>> mapObj, entityObj; // keep track of when to draw the object ( maps first )
	for ( std::weak_ptr<Drawable> d : _drawList ) {
		auto d1 = d.lock();
		switch ( d1->getLayer() ) {
		case Drawable::MAP_LAYER:
			mapObj.push_back( d1 );
			break;
		case Drawable::ENTITY_LAYER:
			entityObj.push_back( d1 );
			break;
		default:
			break;
		}
	}
	drawEntities( mapObj ), drawEntities( entityObj );
	_window->update();
}

bool expired( std::weak_ptr<Drawable> d ) { return d.expired(); }
void Renderer::cullList() {
	_drawList.remove_if( expired );
}*/

void Renderer::drawEntities( std::vector<std::shared_ptr<Drawable>> dList ) {
	for ( auto d : dList ) {
		_window->draw( d->getRepresentation() );
	}
}

void Renderer::drawFrame() {
	_window->clear();

	std::vector<std::shared_ptr<Drawable>> mapObj, entityObj; // keep track of when to draw the object ( maps first )
	for ( std::shared_ptr<Drawable> d : _game.lock()->_drawList ) {
		switch ( d->layer() ) {
		case Drawable::MAP_LAYER:
			mapObj.push_back( d );
			break;
		case Drawable::ENTITY_LAYER:
			entityObj.push_back( d );
			break;
		default:
			break;
		}
	}
	drawEntities( mapObj ), drawEntities( entityObj );
	std::string displayString = _game.lock()->_currentMap->displayString();
	_window->update();
}