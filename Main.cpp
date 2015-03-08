#include <SDL.h>
#include "Window.h"
#include "Surface.h"
#include "RenderObject.h"
#include "Representation.h"
#include "Texture.h"
#include "Game.h"
#include "NormalTile.h"
#include "EmptyTile.h"
#include "DestinationTile.h"
#include "Renderer.h"
#include "Map.h"
#include "Modulator.h"
#include "Dependent.h"

using namespace SDLWrapper;

const int screenSizeX = 800;
const int screenSizeY = 700;

auto window = std::make_shared<SDLWrapper::Window>( "Circuit", screenSizeX, screenSizeY );
Texture NormalTile::normalTexture( "img/normal.bmp",window.get() );
Texture EmptyTile::emptyTexture( "img/empty.bmp", window.get() );
Texture DestinationTile::destinationTexture( "img/destination.bmp", window.get() );
Texture Modulator::textureTemplate( "img/modulator.bmp", window.get(), &Color( 0,0,0,0 ) );
Texture Dependent::textureTemplate( "img/dependent.bmp", window.get() );

int main() {				 	

	bool running = true; 
	//std::shared_ptr<Map> test = std::make_shared<Map>( screenSizeX, screenSizeY, "maps/test2.cmap" );
	//test->initFromFile( "maps/test2.cmap" );
	//std::shared_ptr<Modulator> m = std::make_shared<Modulator>( test, 9, 3 );
	auto game = std::make_shared<Game>( 60, screenSizeX, screenSizeY );
	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>( window, game );
	//Renderer renderer( window );
	SDL_Event e;
	
//	renderer->add( test->tileList() );


	while ( running ) {
		while ( SDL_PollEvent( &e ) != 0 ) {
			//User requests quit
			if ( e.type == SDL_QUIT ) {
				running = false;
			}
			else {
				game->handleInput( &e );
			}
		}

		game->update();
		renderer->drawFrame();

		SDL_Delay( 1000 / game->fps() );
	}
	return 0;
}