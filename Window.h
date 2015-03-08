#pragma once

#include <SDL.h>
#include <memory>
#undef main

namespace SDLWrapper
{
	class Window {
		friend class RenderObject;
		friend class Representation;
		friend class Texture;
	public:
		Window( const char* title = "", const int screenWidth = 800, const int screenHeight = 600 );
		~Window();
		void clear();
		void draw( std::weak_ptr<SDLWrapper::RenderObject> tex );
		void draw( std::weak_ptr<SDLWrapper::Representation> tex );
		void update();
	private:
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		SDL_Texture* _currentTexture;
	};
}