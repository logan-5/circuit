#pragma once
#include <string>
#include <SDL.h>
#include "Texture.h"
#include "TextureData.h"
#include "Color.h"

namespace SDLWrapper
{
class Representation : public TextureData
{
	friend class Window;
public:
	Representation( SDLWrapper::Texture& texture, int x = 0, int y = 0, int sizeX = 0, int sizeY = 0, double angle = 0 );
	~Representation();
	void modulate( const Color& mod );
	void resetModulation();
};
}
