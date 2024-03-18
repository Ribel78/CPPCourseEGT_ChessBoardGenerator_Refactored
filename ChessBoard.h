#pragma once

#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Timer.h"

struct ChessBoardDescriptions {
    std::string Custom;
    std::string FEN;
};

class ChessBoard
{
public:
    ChessBoard();
    ~ChessBoard();

    //extract chess glyphs from a font as textures
    void prepChessPieceTextures();

    //check the current state of the simulation
    bool isSimulating() const;

    //set the current state of the simulation
    void setSimulating(const bool state);

    void setPiecesToRemove(int amount);

    void shufflePieces(const bool shuff,
                       std::string& custDescription,
                       std::string& fenDescription);

    //sets boardDescription from the queue back (last simulation) used in overlay display
    void setBoardDescriptionFromQueueBack();

    // std::queue<std::string>& getMutableCustomDescriptionQueue();

    // std::queue<std::string>& getMutableFENDescriptionQueue();

    std::queue<ChessBoardDescriptions>& getMutableDescriptionsQueue();

    //sets the index of the currently clicked chess board square or -1
    void setChessPieceIdx(int idx);

    //get a rectangle from the chess board for drawing
    SDL_Rect* getChessBoardSquareRect(int idx) const;

    //returns string from Time object to be passed for drawing
    std::string getSimulationSummary() const;

    //initializes the chess board squares according to the given chess board size
	void initBoard();

    //draws colored chess board on the screen
	void drawBoard();

    //draws color overlay on top of the chessboard - for displaying allowed moves 
	void drawBoardOverlay();

    //draws the chess pieces according to given board description
    void drawPieces();

private:

    //parse result from shufflePieces to valid FEN notation for dispaly
    void parseFEN(const char chess_set[65], char FEN[71]);

    //check if board is currently being simulated
    bool m_simulating;

	//size of the chess board in pixels 
    int m_chessBoardSize;

    int m_piecesToRemove;

    //store rectangles for each chess board square
    SDL_Rect* m_chessBoardSquare[64];

    //store textures of the 12 unique chess pieces
    SDL_Texture* m_chessPieces[12];

    //rectangles for chess board labels
    SDL_Rect* m_chessBoardLabelsV[8];

    SDL_Rect* m_chessBoardLabelsH[8];

    //colors of the black and white squares and the overlay color
    SDL_Color m_chessBoardColor[3];


	//Used in drawing the Board Overlay
    //the index of the clicked square, -1 if board overlay is off
    int m_chessPieceIdx;

    //current chess board description
    std::string m_boardDescription;

    //NEW struct queue

    std::queue<ChessBoardDescriptions> m_CB_Descriptions;

	//Simulation time stats are retrieved from this object.
    Timer m_timer;
};
