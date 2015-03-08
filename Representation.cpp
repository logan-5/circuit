#include "Representation.h"
using namespace SDLWrapper;

/*
====================

Constructor

	Initialize all variables and call parent constructor.  If size is not given, 'ask' the actual bmp what size it is and use that.

====================
*/
Representation::Representation( SDLWrapper::Texture& texture, int x, int y, int sizeX, int sizeY, double angle ) : TextureData( x, y, sizeX, sizeY, &texture, angle ) {
	if ( this->sizeX == 0 && this->sizeY == 0 ) {
		SDL_QueryTexture( _texture->_texture, nullptr, nullptr, &( this->sizeX ), &( this->sizeY ) );
	}
}

Representation::~Representation() {
}

/*
====================

modulate( const Color& mod )

	Call SDL function to modulate the current representation's color, e.g. upon selection.

====================
*/
void Representation::modulate( const Color& mod ) {
	SDL_SetTextureColorMod( _texture->_texture, mod.red, mod.green, mod.blue );
}

/*
====================

resetModulation()

	Undo any color modulation effects.  Used e.g. upon deselection.

====================
*/
void Representation::resetModulation() {
	SDL_SetTextureColorMod( _texture->_texture, 255, 255, 255 );
}