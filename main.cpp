//main.cpp
#include "Game.h"

//#include <iostream>

Game* game = NULL;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

int main(int argc, char* argv[])
{
	game = new Game();
	game->init("Chess Board Generator",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    while (game->isRunning())
    {
		game->handleEvents();

        game -> draw();

		game->update();
	}
	game->clean();
	return 0;
}

