#pragma once

#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Timer.h"

struct ChessBoardDescriptions {
    std::string Custom {};
    std::string FEN {};
    std::string simulationTime {};

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

    void prepFonts();
    void prepStaticFontTextures();
    void prepStaticImageTextures();
    void prepChessPieceTextures();
    void prepBoardLabelsTextures();

    void setButtonViewerTexID(std::string texture_id);
    void setButtonSimulatorTexID(std::string texture_id);
    void setButtonStartTexID(std::string texture_id);
    void setButtonStopTexID(std::string texture_id);

    void setPiecesToRemove(int amount);
    int getPiecesToRemove();

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

    SDL_Rect* getRectChessBoardTile(int idx) const;
    SDL_Rect* getRectTextFEN();
    SDL_Rect* getRectButtonViewer();
    SDL_Rect* getRectButtonSimulator();
    SDL_Rect* getRectSliderKnob();
    SDL_Rect* getRectWindow();

    std::string getSimulationSummary() const;
    void resetSimulationSummary();

    void initBoardRects();
    void drawWindowBackground();
    void drawTitle();
    void drawBoard();
    void drawStatistics();
    void drawSlider(const int& offsetX);
    void drawModeToggleButtons();
    void drawFENDescription();
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

    SDL_Rect* m_RectPtrChessBoardTile[64];
    SDL_Rect* m_RectPtrFloatingChessTile;
    SDL_Rect* m_RectPtrBoardLabelsV[8];
    SDL_Rect* m_RectPtrBoardLabelsH[8];

    SDL_Rect m_RectTextFEN;
    SDL_Rect m_RectTextStats;
    SDL_Rect m_RectButtonSimulator;
    SDL_Rect m_RectButtonViewer;
    SDL_Rect m_RectTextTitle;
    SDL_Rect m_RectWindow;
    SDL_Rect m_Rect_SliderSlit;
    SDL_Rect m_Rect_SliderKnob;

    std::string m_ButtonViewerTexID {};
    std::string m_ButtonSimulatorTexID {};
    std::string m_ButtonStartTexID {};
    std::string m_buttonStopTexID {};

    //colors of the black and white squares and the overlay color
    SDL_Color m_ColorChessBoard[3];

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
