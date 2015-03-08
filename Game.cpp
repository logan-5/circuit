#include "Game.h"
#include <iostream>
#include <fstream>
#include "DestinationTile.h"

Game::Game( const int fps, int screenSizeX, int screenSizeY, std::string mapsFile ) : _fps( fps ), _screenSizeX( screenSizeX ), _screenSizeY( screenSizeY ), _selected( nullptr ), _animating( false ) {
	std::ifstream mapFile( mapsFile );
	if ( mapFile.is_open() ) {
		std::string path;
		while ( std::getline( mapFile, path ) )
			_mapFilePaths.push_back( path );
		_currentMapPath = _mapFilePaths.begin();
	}
	
	loadMap( *_currentMapPath );
}

Game::Game( std::weak_ptr<Map> map, const int fps, int screenSizeX, int screenSizeY ) : _fps( fps ), _screenSizeX( screenSizeX ), _screenSizeY( screenSizeY ), _selected( nullptr ), _animating( false ) {
	/*auto sel = std::make_shared<Modulator>( map, 2, 3 );
	auto dep1 = std::make_shared<Dependent>( map, 2, 1, 90 );
	auto dep2 = std::make_shared<Dependent>( map, 2, 5, 270 );
	sel->addDependent( dep1 );
	sel->addDependent( dep2 );
	_selected = sel;
	_renderer->add( _selected );
	_renderer->add( dep1 );
	_renderer->add( dep2 );*/
//	_currentMap = std::make_shared<Map>( screenSizeX, screenSizeY, "maps/test2.cmap" );
//	addObjectsFromMap();
}

Game::~Game() {
}

/*
====================

loadMap (std::string fileName )

	Set the current map and create & add all objects it tells us we need to make.

====================
*/
void Game::loadMap( std::string fileName ) {
	_selected = nullptr;
	_drawList.clear();
	_currentMap = std::make_shared<Map>( _screenSizeX, _screenSizeY, fileName );
	auto objects = _currentMap->initFromFile( fileName );
	addObjectsFromMap( objects );
	auto tiles = _currentMap->tileList();

	/*
	//for debugging
	std::vector<Tile*> test;
	for ( std::weak_ptr<Drawable> d : tiles )
		test.push_back( dynamic_cast<Tile*>( d.lock().get() ) );*/

	for ( std::weak_ptr<Drawable> d : tiles )
		_drawList.push_front(  d.lock()  );
}

void Game::addObjectsFromMap( std::vector<std::shared_ptr<Drawable>> objects ) {
	for ( auto& it : objects )
		_drawList.push_front( it );

	std::vector<std::vector<Modulator*>> mods;

	// go through all modulators finding their codes
	for ( auto it = objects.begin(); it != objects.end(); ++it ) {
		if ( ( *it )->drawtype() == Drawable::DRAWABLE_MODULATOR ) {
			Modulator* m = dynamic_cast<Modulator*>( it->get() );

			//_selected = ( std::shared_ptr<Selectable>( m ) );
			bool assigned = false;
			for ( auto& modit : mods ) {
				if ( ( m->code() == modit[0]->code() ) ) {
					modit.push_back( m );
					assigned = true;
				}
			}
			if ( !assigned ) {
				Modulator* m = dynamic_cast<Modulator*>( it->get() );
				std::vector<Modulator*> vec;
				vec.push_back( m );
				mods.push_back( vec );
			}
		}
	}

	for ( auto it = objects.begin(); it != objects.end(); ++it ) {
		if ( ( *it )->drawtype() == Drawable::DRAWABLE_DEPENDENT ) {
			for ( auto& modit : mods ) {
				Dependent* d = dynamic_cast<Dependent*>( it->get() );
				if ( ( d->code() == modit[0]->code() ) ) {
					for ( auto& moditit : modit ) {
						//( *moditit )->addDependent( std::shared_ptr<Dependent>( d ) );
						moditit->addDependent( d );
					}
				}
			}
		}
	}

	for ( auto it = objects.begin(); it != objects.end(); ++it ) {
		if ( ( *it )->drawtype() == Drawable::DRAWABLE_MODULATOR ) {
			Modulator* m = dynamic_cast<Modulator*>( it->get() );
			m->turnToFace( m->angle() );
			_animating = true;
		}
	}
}

void Game::handleInput( const SDL_Event* e ) {
	if ( e->type == SDL_KEYDOWN ) {
		if ( _selected && !_animating ) {
			Movable* _mover = dynamic_cast<Movable*>( _selected ); // as of now all Selectable*s are Movable*s
			// bool move = false;
			switch ( e->key.keysym.sym ) {
			case SDLK_w:
			case SDLK_UP:
				if ( !( _animating = _mover->turnToFace( 90 ) ) ) 
					_animating = _mover->moveForward();
				break;
			case SDLK_s:
			case SDLK_DOWN:
				if ( !( _animating = _mover->turnToFace( 270 ) ) )
					_animating = _mover->moveForward();
				break;
			case SDLK_a:
			case SDLK_LEFT:
				if ( !( _animating = _mover->turnToFace( 180 ) ) )
					_animating = _mover->moveForward();
				break;
			case SDLK_d:
			case SDLK_RIGHT:
				if ( !( _animating = _mover->turnToFace( 0 ) ) )
					_animating = _mover->moveForward();
				break;
			default:
				break;
			}
		}
	}
	if ( e->type == SDL_MOUSEBUTTONDOWN ) {
		for ( auto it = _drawList.begin(); it != _drawList.end(); ++it ) {
			if ( ( *it )->drawtype() == Drawable::DRAWABLE_MODULATOR ) {
				Modulator* m = dynamic_cast<Modulator*>( ( *it ).get() );
				int x = m->gridX() * _currentMap->tileSize() + _currentMap->xOffset();
				int y = m->gridY() * _currentMap->tileSize() + _currentMap->yOffset();

				if ( e->button.x >= x && e->button.x <= x + _currentMap->tileSize() &&
					e->button.y >= y && e->button.y <= y + _currentMap->tileSize() ) {
					if ( m->select() ) {
						if ( _selected != nullptr ) {
							_selected->deselect();
						}
						_selected = m;
						m->turnToFace( m->angle() );
						_animating = true;
					}
				}
			}
		}
	}
}

void Game::update() {
	if ( _animating ) {
		Movable* m;
		bool stillAnimating = false;
		for ( auto it = _drawList.begin(); it != _drawList.end(); ++it ) {
			m = dynamic_cast<Movable*>( it->get() );
			if ( m != nullptr ) {
				if ( m->update() ) {
					stillAnimating = true;
				}
			}
		}
		if ( _animating && !stillAnimating ) {
			bool won = true;
			for ( auto it = _drawList.begin(); it != _drawList.end(); ++it ) {
				if ( DestinationTile* d = dynamic_cast<DestinationTile*>( it->get() ) ) {
					if ( d->occupier() ) {
						Dependent* dep = dynamic_cast<Dependent*>( d->occupier() );
						if ( dep == nullptr ) 
							won = false;
					}
					else won = false;
				}
			}
			if ( won && ( ++_currentMapPath != _mapFilePaths.end() ) ) {
				loadMap( *( _currentMapPath ) );
			}			
		}
		_animating = stillAnimating;
	}
}