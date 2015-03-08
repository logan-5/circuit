#pragma once
#include <SDL.h>
#include "Texture.h"

namespace SDLWrapper
{
struct TextureData {
public:
	TextureData( int x = 0, int y = 0, int sizeX = 0, int sizeY = 0, Texture* texture = nullptr, double angle = 0 ) : x( x ), y( y ), sizeX( sizeX ), sizeY( sizeY ), angle( angle ) { 
		if ( texture != nullptr )
			_texture = texture; 
	};
	int x, y, sizeX, sizeY;
	double angle;
protected:
	Texture* _texture;
};
}