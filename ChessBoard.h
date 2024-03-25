#pragma once

#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Timer.h"

struct ChessBoardDescriptions {
    std::string Custom {};
    std::string FEN {};
    std::string simulationTime {};

    // friend auto operator <<(std::ostream& output_stream, const ChessBoardDescriptions& rhs) -> std::ostream&
    // {
    //     output_stream << rhs.Custom << " "
    //                   << rhs.FEN << " "
    //                   << rhs.simulationTime << "\n";

    //     return output_stream;
    // }

    friend auto operator >>(std::istream& input_stream, ChessBoardDescriptions& rhs) -> std::istream&
    {
        input_stream    >> rhs.Custom
                        >> rhs.FEN
                        >> rhs.simulationTime;;

        return input_stream;
    }
};

class ChessBoard
{
public:
    ChessBoard();
    ~ChessBoard();

    //extract chess glyphs from a font as textures
    void prepChessPieceTextures();

    bool isSimulating() const;

    bool isViewing() const;

    void setSimulating(const bool state);

    void setViewing(const bool state);

    void setPiecesToRemove(int amount);

    void shufflePieces(const bool shuff,
                       std::string& custDescription,
                       std::string& fenDescription);

    //sets boardDescription from the queue back (last simulation) used in overlay display
    void setBoardDescriptionFromQueueBack();

    void setBoardDescriptionFromVector();
    int& getMutable_CB_Descriptions_Vec_Seek();

    std::queue<ChessBoardDescriptions>& getMutableDescriptionsQueue();

    std::vector<ChessBoardDescriptions>& getMutableDescriptionsVector();

    ChessBoardDescriptions getCurrentDescription() const;

    //sets the index of the currently clicked chess board square or -1
    void setChessPieceIdx(int idx);

    //get a rectangle from the chess board for drawing
    SDL_Rect* getChessBoardSquareRect(int idx) const;

    //returns string from Time object to be passed for drawing
    std::string getSimulationSummary() const;

    void resetSimulationSummary();

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

    bool m_simulating; //check if board is currently being simulated

    bool m_viewing; //check if board is in viewer mode

	//size of the chess board in pixels 
    int m_chessBoardSize;

    int m_piecesToRemove;

    //store textures of the 12 unique chess pieces
    SDL_Texture* m_chessPieces[12];

    SDL_Rect* m_chessBoardSquare[64];
    SDL_Rect* m_srcChessTileRect;
    SDL_Rect* m_chessBoardLabelsV[8];
    SDL_Rect* m_chessBoardLabelsH[8];

    //colors of the black and white squares and the overlay color
    SDL_Color m_chessBoardColor[3];

	//Used in drawing the Board Overlay
    //the index of the clicked square, -1 if board overlay is off
    int m_chessPieceIdx;

    std::queue<ChessBoardDescriptions> m_CB_Descriptions;

    std::vector<ChessBoardDescriptions> m_CB_Descriptions_Vec;
    int m_CB_Descriptions_Vec_Seek {};

    //current chess board description
    ChessBoardDescriptions m_current_CB_Description;

	//Simulation time stats are retrieved from this object.
    Timer m_timer;
};
