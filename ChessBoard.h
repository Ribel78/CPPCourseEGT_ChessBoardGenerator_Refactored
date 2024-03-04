#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <queue>
#include <iostream>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>
#include <cstring>

#include "Timer.h"
#include "TextureFactory.h"
#include "Utilities.h"

class ChessBoard{


public:
    ChessBoard();
    ~ChessBoard();
    void setRenderer(SDL_Renderer*);
    void prep_chess_piece_textures();
    bool isSimulating(); //chess 
	void setSimulating(bool); //chess
    /*
    Used to generate random chess board with 75% (24 pieces)
    */
    void shufflePieces(bool, std::string &, std::string &);

    void parseFEN(char chess_set[65], char FEN[71]);

    /*
    Utility function to find the allowed (attack) squares of given chess piece
    Taking into account of the rules of chess pieces moves.
    Used in drawBoardOverlay and chess board generation
    */
    friend std::string attackSquares(	std::string boardDescription, 
										int x, int y, 
										char piece);

    void setBoardDescriptionFromQueueBack();
    void setChessPieceIdx(int);
    SDL_Rect* getChessBoardSquareRect(int);
    std::string getSimulationSummary();
    //Chess drawing functions
	void initBoard();
	void drawBoard();
	void drawBoardOverlay();
    void drawPieces();

	//Seek trough the last 20 simulations using Down Arrow
	std::queue<std::string> queueCustomSetDescription;
	std::queue<std::string> queueFENSetDescription;

private:

    SDL_Renderer* renderer = NULL;

	bool simulating;

	//Chess Related fields
	int chess_board_size;
    SDL_Rect* chess_board_square[64];
    SDL_Color chess_board_color[3];	
	SDL_Texture* chessPieces[12];

	//Used in drawing the Board Overlay
	int chessPieceIdx;
	std::string boardDescription;   

	//Simulation Time
	Timer timer;
};