#include <string>
#include "RenderObject.h"
#include "Window.h"


SDLWrapper::RenderObject::RenderObject( const char* bmp, const SDLWrapper::Window& window, int x, int y, int sizeX, int sizeY, std::shared_ptr<Color> transparent ) : TextureData( x, y, sizeX, sizeY ) {
	this->init( bmp, window, x, y, sizeX, sizeY, transparent );
}

SDLWrapper::RenderObject::RenderObject( const std::string& bmp, const SDLWrapper::Window& window, int x, int y, int sizeX, int sizeY, std::shared_ptr<Color> transparent ) : TextureData( x, y, sizeX, sizeY ) {
	this->init( bmp.c_str(), window, x, y, sizeX, sizeY, transparent );
}

void SDLWrapper::RenderObject::init( const char* bmp, const SDLWrapper::Window& window, int x, int y, int sizeX, int sizeY, std::shared_ptr<Color> transparent ) {
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( bmp );
	if ( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", bmp, SDL_GetError() );
	} else {
		//Create texture from surface pixels
		_texture->_texture = SDL_CreateTextureFromSurface( window._renderer, loadedSurface );
		if ( _texture == NULL ) {
			printf( "Unable to create texture from %s! SDL Error: %s\n", bmp, SDL_GetError() );
		}

		if ( transparent != nullptr ) {
			// Color key image
			SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, transparent->red, transparent->green, transparent->blue ) );
		}

		if ( this->sizeX == 0 && this->sizeY == 0 ) {
			SDL_QueryTexture( _texture->_texture, nullptr, nullptr, &( this->sizeX ), &( this->sizeY ) );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
}

SDLWrapper::RenderObject::~RenderObject() {
	// SDL_DestroyTexture( _texture );
}
