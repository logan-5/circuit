#include "Window.h"
#include "RenderObject.h"
#include "Representation.h"
#include <stdio.h>

SDLWrapper::Window::Window( const char* title, const int screenWidth, const int screenHeight ) {
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	} else {
		//Create window
		_window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN );
		if ( _window == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		} else {
			//Create renderer for window
			_renderer = SDL_CreateRenderer( _window, -1, SDL_RENDERER_ACCELERATED );
			if ( _renderer == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			} else {
				//Initialize renderer color
				SDL_SetRenderDrawColor( _renderer, 0xFF, 0xFF, 0xFF, 0xFF );
			}
		}
	}
}

SDLWrapper::Window::~Window() {	
	//Destroy window    
	SDL_DestroyRenderer( _renderer );	   
	//Destroy window
	SDL_DestroyWindow( _window );

	//Quit SDL subsystems
	SDL_Quit();
}

void SDLWrapper::Window::clear() {
	SDL_RenderClear( _renderer );
}

void SDLWrapper::Window::draw( std::weak_ptr<SDLWrapper::RenderObject> obj ) {
	if ( obj.expired() ) return;

	std::shared_ptr<SDLWrapper::RenderObject> objPtr = obj.lock();
	//Render texture to screen
	SDL_Rect destR;
	destR.x = objPtr->x;
	destR.y = objPtr->y;
	destR.w = objPtr->sizeX;
	destR.h = objPtr->sizeY;
	double angle = 360 - objPtr->angle;
	SDL_RenderCopyEx( this->_renderer, objPtr->_texture->_texture, NULL, &destR, angle, NULL, SDL_FLIP_NONE );
}

void SDLWrapper::Window::draw( std::weak_ptr<SDLWrapper::Representation> obj ) {
	if ( obj.expired() ) return;

	std::shared_ptr<SDLWrapper::Representation> objPtr = obj.lock();
	//Render texture to screen
	SDL_Rect destR;
	destR.x = objPtr->x;
	destR.y = objPtr->y;
	destR.w = objPtr->sizeX;
	destR.h = objPtr->sizeY;
	double angle = 360 - objPtr->angle;
	SDL_RenderCopyEx( this->_renderer, objPtr->_texture->_texture, NULL, &destR, angle, NULL, SDL_FLIP_NONE );
}

void SDLWrapper::Window::update() {
	SDL_RenderPresent( _renderer );
}