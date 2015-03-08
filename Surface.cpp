#include "Surface.h"

SDLWrapper::Surface::Surface( const std::string& bmp, const SDL_PixelFormat& format ) {
	this->_surface = NULL; 
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( bmp.c_str() );
	if ( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL Error: %s\n", bmp.c_str(), SDL_GetError() );
	}
	else {
		//Convert surface to screen format
		this->_surface = SDL_ConvertSurface( loadedSurface, &format, NULL );
		if ( this->_surface == NULL ) {
			printf( "Unable to optimize image %s! SDL Error: %s\n", bmp.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
}

SDLWrapper::Surface::Surface( const char* bmp, const SDL_PixelFormat& format ) {
	this->_surface = NULL;
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( bmp );
	if ( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL Error: %s\n", bmp, SDL_GetError() );
	} else {
		//Convert surface to screen format
		this->_surface = SDL_ConvertSurface( loadedSurface, &format, NULL );
		if ( this->_surface == NULL ) {
			printf( "Unable to optimize image %s! SDL Error: %s\n", bmp, SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
}

SDLWrapper::Surface::~Surface() {
	SDL_FreeSurface( this->_surface );
	this->_surface = nullptr;
}