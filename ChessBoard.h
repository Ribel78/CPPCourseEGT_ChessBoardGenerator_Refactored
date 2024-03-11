#pragma once

#include <queue>
#include <algorithm>
#include <array>
#include <random>
#include <cstring>
#include <iostream>
#include "Timer.h"
#include "TextureFactory.h"
#include "Utilities.h"

class ChessBoard
{

public:
    ChessBoard();
    ~ChessBoard();

    //get renderer pointer from Game object and set it as this class member 
    void setRenderer(SDL_Renderer* renderer);

    //extract chess glyphs from a font as textures
    void prep_chess_piece_textures();

    //check the current state of the simulation
    bool isSimulating();

    //set the current state of the simulation 
    void setSimulating(bool state);

    void setPiecesToRemove(int amount);

    /*
    Generate random chess board with given amount of removed pieces
    Use setPiecesToRemove method to set amount of removed pieces.
    */
    void shufflePieces(bool shuff,
                       std::string &custDescription,
                       std::string &fenDescription);

    //parse result from shufflePieces to valid FEN notation for dispaly
    void parseFEN(char chess_set[65], char FEN[71]);

    /*
    Utility function to find the allowed (attack) squares of given chess piece
    Taking into account of the rules of chess pieces moves.
    Used in drawBoardOverlay and chess board generation
    */
    friend std::string attackSquares(std::string boardDescription,
                                    int x, int y,
                                    char piece);

    //sets boardDescription from the queue back (last simulation) used in overlay display
    void setBoardDescriptionFromQueueBack();

    //sets the index of the currently clicked chess board square or -1
    void setChessPieceIdx(int idx);

    //get a rectangle from the chess board for drawing
    SDL_Rect* getChessBoardSquareRect(int idx);

    //returns string from Time object to be passed for drawing
    std::string getSimulationSummary();

    //chess board drawing functions

    //Initializes the chess board squares according to the given chess board size
	void initBoard();

    //draws colored chess board on the screen
	void drawBoard();

    //draws color overlay on top of the chessboard - for displaying allowed moves 
	void drawBoardOverlay();

    //draws the chess pieces according to given board description
    void drawPieces();

    //store generated chessboard descriptions - in a queue.
    //seek trough the last 20 simulations using Down Arrow Key
	std::queue<std::string> queueCustomSetDescription;
	std::queue<std::string> queueFENSetDescription;

private:
    //needed for the drawing functions passed from the Game object
    SDL_Renderer* renderer = NULL;

    //check if board is currently being simulated
	bool simulating;

	//size of the chess board in pixels 
	int chess_board_size;

    int piecesToRemove;

    //store rectangles for each chess board square
    SDL_Rect* chess_board_square[64];

    //colors of the black and white squares and the overlay color
    SDL_Color chess_board_color[3];	

    //store textures of the 12 unique chess pieces
	SDL_Texture* chessPieces[12];

	//Used in drawing the Board Overlay

    //the index of the clicked square, -1 if board overlay is off
	int chessPieceIdx;

    //current chess board description
	std::string boardDescription;   

	//Simulation time stats are retrieved from this object.
	Timer timer;
};
