#pragma once
#include <SDL.h>
#include <string>
#include "TextureData.h"
#include "Window.h"
#include "Color.h"

namespace SDLWrapper
{
	class RenderObject : public TextureData
	{
		friend class Window;
	public:
		RenderObject( const std::string& bmp, const SDLWrapper::Window& window, int x = 0, int y = 0, int sizeX = 0, int sizeY = 0, std::shared_ptr<Color> transparent = nullptr );
		RenderObject( const char* bmp, const SDLWrapper::Window& window, int x = 0, int y = 0, int sizeX = 0, int sizeY = 0, std::shared_ptr<Color> transparent = nullptr );
		~RenderObject();
	private:
		void init( const char* bmp, const SDLWrapper::Window& window, int x = 0, int y = 0, int sizeX = 0, int sizeY = 0, std::shared_ptr<Color> transparent = nullptr );
	};
}

