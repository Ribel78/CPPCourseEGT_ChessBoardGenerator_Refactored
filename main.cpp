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

    game->prepTextures();

	//game->prep_chess_piece_textures();

    game->m_chessBoard.prepChessPieceTextures();

    game->m_chessBoard.initBoard();

    while (game->isRunning())
    {
		game->handleEvents();
		
		//Static Parts
		game->drawStaticElements();
        game->m_chessBoard.drawBoard();
			
		//Dynamic parts	
		game->drawDynamicElements();
        game->m_chessBoard.drawBoardOverlay();
        game->m_chessBoard.drawPieces();
	
		//Update Data	
		game->update();
	}
	game->clean();
	return 0;
}

