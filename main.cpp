//main.cpp
#include "Game.h"

#include <iostream>

Game* game = NULL;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

int main(int argc, char* argv[]) {

	game = new Game();
	game->init("Chess Board Generator",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	game->prep_textures();
	//game->prep_chess_piece_textures();
	game->cb.prep_chess_piece_textures();
	game->cb.initBoard();
	while (game->isRunning()) {
		game->handleEvents();
		
		//Static Parts
		game->drawStaticElements();
		game->cb.drawBoard();	
			
		//Dynamic parts	
		game->drawDynamicElements();
		game->cb.drawBoardOverlay();
		game->cb.drawPieces();
	
		//Update Data	
		game->update();
		
	}
	game->clean();
	return 0;
}

