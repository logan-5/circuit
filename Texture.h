#pragma once
#include <SDL.h>
#include "Window.h"
#include "Color.h"
#include <string>

namespace SDLWrapper
{

class Texture
{
	friend struct	TextureData;
	friend class	Window;
	friend class	Representation;
	friend class	RenderObject;
public:
					Texture( const std::string& bmp, const SDLWrapper::Window* window, Color* transparent = nullptr );
					Texture( const char* bmp, const SDLWrapper::Window* window, Color* transparent = nullptr );
					Texture( const Texture& other );
					~Texture();
	void			modulate( const Color& mod );
	void			resetModulation();
private:
	SDL_Texture*	_texture;
	const Window*	_window;
	Color			_transparent;
	const char*		_fileName;

	void			init( const char* bmp, const SDLWrapper::Window* window, Color* transparent );
};

}

