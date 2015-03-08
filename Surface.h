#pragma once

#include <SDL.h>
#include <string>

namespace SDLWrapper 
{
	class Surface {
		friend class Window;
	public:
		Surface( const std::string& bmp, const SDL_PixelFormat& format );
		Surface( const char* bmp, const SDL_PixelFormat& format );
		~Surface();
	private:
		SDL_Surface* _surface;
	};
}