#include "Texture.h"
using namespace SDLWrapper;

Texture::Texture( const char* bmp, const SDLWrapper::Window* window, Color* transparent ) : _window( window ), _fileName( bmp ) {
	init( bmp, window, transparent );
}

Texture::Texture( const std::string& bmp, const SDLWrapper::Window* window, Color* transparent ) : _window( window ), _fileName( bmp.c_str() ) {
	init( bmp.c_str(), window, transparent );
}

Texture::Texture( const Texture& other ) : _window( other._window ), _transparent( other._transparent ), _fileName( other._fileName ) {
	init( other._fileName, _window, &_transparent );
}

void Texture::init( const char* bmp, const SDLWrapper::Window* window, Color* transparent ) {
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( bmp );
	if ( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", bmp, SDL_GetError() );
	} else {
		//Create texture from surface pixels
		_texture = SDL_CreateTextureFromSurface( window->_renderer, loadedSurface );
		if ( _texture == NULL ) {
			printf( "Unable to create texture from %s! SDL Error: %s\n", bmp, SDL_GetError() );
		}

		if ( transparent != nullptr ) {
			// Color key image
			SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, transparent->red, transparent->green, transparent->blue ) );
			_transparent = Color(*transparent);
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
}

Texture::~Texture() {
	SDL_DestroyTexture( _texture );
}

void Texture::modulate( const Color& mod ) {
	SDL_SetTextureColorMod( _texture, mod.red, mod.green, mod.blue );
}

void Texture::resetModulation() {
	SDL_SetTextureColorMod( _texture, 0, 0, 0 );
}