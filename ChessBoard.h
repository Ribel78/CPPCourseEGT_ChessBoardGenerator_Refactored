#pragma once

#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Timer.h"

struct ChessBoardDescriptions {
    std::string Custom {};
    std::string FEN {};
    std::string simulationTime {};
    std::string chess_pieces {};

    friend auto operator >>(std::istream& input_stream, ChessBoardDescriptions& rhs) -> std::istream&
    {
        input_stream    >> rhs.Custom
                        >> rhs.FEN
                        >> rhs.simulationTime
                        >> rhs.chess_pieces;

        return input_stream;
    }
};

class ChessBoard
{
public:
    ChessBoard();
    ~ChessBoard();

    auto isSimulating() const -> bool;
    auto isViewing() const -> bool;

    void setSimulating(const bool state);
    void setViewing(const bool state);

    void setButtonViewerTexID(std::string texture_id);
    void setButtonSimulatorTexID(std::string texture_id);
    void setButtonStartTexID(std::string texture_id);
    void setButtonStopTexID(std::string texture_id);

    void setPiecesToRemove(int amount);
    auto getPiecesToRemove() -> int;

    void shufflePieces(const bool shuff,
                       std::string& custDescription,
                       std::string& fenDescription);

    void setBoardDescriptionFromQueueBack();
    void setBoardDescriptionFromVector();

    auto getMutable_CB_Descriptions_Vec_Seek() -> int&;

    auto getMutableDescriptionsQueue() -> std::queue<ChessBoardDescriptions>&;

    auto getMutableDescriptionsVector() -> std::vector<ChessBoardDescriptions>&;

    auto getCurrentDescription() const -> ChessBoardDescriptions;
    void setCurrentDescription(std::string cd,
                               std::string fen,
                               std::string sim_time,
                               std::string n_pieces);

    void setChessPieceIdx(int idx);

    auto getRectChessBoardTile(int idx) const -> SDL_Rect*;
    auto getRectTextFEN() -> SDL_Rect*;
    auto getRectButtonViewer() -> SDL_Rect*;
    auto getRectButtonSimulator() -> SDL_Rect*;
    auto getRectSliderKnob() -> SDL_Rect*;
    auto getRectWindow() -> SDL_Rect*;

    auto getSimulationSummary() const -> std::string;
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

    //shifflePieces helper functions
    void simRandomizeChessSet(char (&char_array)[65]);
    auto isIllegalBishops(char (&char_array)[65]) -> bool;
    void removeIllegalPawnsAndKings(char (&char_array)[65], int& mutable_int);
    auto isExtraPiecesToRemove(char (&char_array)[65], int& mutable_int) -> bool;
    void addKingsToBoard(char (&char_arr)[65]);

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
