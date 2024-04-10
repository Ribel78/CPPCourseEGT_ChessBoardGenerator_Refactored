//main.cpp

#include "Game.h"
#include "Constants.h"

using namespace Constants;

Game* game = nullptr;


int main(int argc, char* argv[])
{
    game = new Game();
    game->Init("Chess Board Generator",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                DIM_WINDOW_WIDTH, DIM_WINDOW_HEIGHT, 0);

    while (game->IsRunning())
    {
        game->HandleEvents();

        game -> Draw();

        game->Update();
    }
    game->Clean();

	return 0;
}

