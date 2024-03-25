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

    bool isSimulating() const;

    bool isViewing() const;

    void setSimulating(const bool state);

    void setViewing(const bool state);

    //extract chess glyphs from a font as textures
    void prepChessPieceTextures();

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

    void setChessPieceIdx(int idx);

    SDL_Rect* getChessBoardSquareRect(int idx) const;
    SDL_Rect* getTextFENRect();
    SDL_Rect* getbuttonViewerRect();
    SDL_Rect* getbuttonSimulationRect();
    SDL_Rect* getWindowRect();

    std::string getSimulationSummary() const;

    void resetSimulationSummary();
	void initBoard();
    void drawTitle();
    void drawModeToggleButtons();
    void drawWindowBackground();
	void drawBoard();
    void drawFENDescription();
    void drawStatistics();
	void drawBoardOverlay();
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

    SDL_Rect m_textFENRect;
    SDL_Rect m_textTimeRect;
    SDL_Rect m_buttonSimulationRect;
    SDL_Rect m_buttonViewerRect;
    SDL_Rect m_textTitleRect;
    SDL_Rect m_windowRect;

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
