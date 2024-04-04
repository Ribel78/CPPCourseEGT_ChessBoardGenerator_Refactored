#pragma once

#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Timer.h"
#include <string>
#include "Constants.h"

using namespace Constants;

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

    void toggleSimulating();
    void toggleViewing();

    void setButtonViewerTexID(std::string texture_id);
    void setButtonSimulatorTexID(std::string texture_id);
    void setButtonStartTexID(std::string texture_id);
    void setButtonStopTexID(std::string texture_id);
    void resetAllButtonsTexID();

    void setPiecesToRemove(int amount);
    auto getPiecesToRemove() -> int;

    void shufflePieces(const bool shuff,
                       std::string& custDescription,
                       std::string& fenDescription);

    void setBoardDescriptionFromQueueBack();
    void setBoardDescriptionFromVector();

    auto getMutableCBDescriptionsVecSeek() -> int&;

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
    void updateStatistics();
    void resetSimulationStatistics();

    void initBoardRects();
    void drawWindowBackground();
    void drawTitle();
    void drawBoard();
    void drawStatistics();
    void drawSlider();
    void drawModeToggleButtons();
    void drawFENDescription();
	void drawBoardOverlay();
    void drawPieces();

    void viewDescriptionNext();
    void viewDescriptionPrevious();
    void copyFENtoClipboard();

    //TODO think of better names potentially a Button object
    bool isButtonClicked(const SDL_Rect* r, int xUp, int yUp) const;
    auto buttonFocus(const SDL_Rect* r) const -> bool;
    void setMouseDownCoords(int x, int y);
    void updateBtnTexturesOnFocus();
    //end TODO

    void readDescriptionFile(std::fstream& dataStream);
    void openDescriptionFileForWriting(std::fstream& dataStream);
    void setCurrentBoardDescriptionSrc();

    // TODO move as private
    int m_offsetX = -1;

private:

    //shifflePieces helper functions
    void simRandomizeChessSet(char (&char_array)[65]);
    auto isIllegalBishops(char (&char_array)[65]) -> bool;
    void removeIllegalPawnsAndKings(char (&char_array)[65], int& mutable_int);
    auto isExtraPiecesToRemove(char (&char_array)[65], int& mutable_int) -> bool;
    void addKingsToBoard(char (&char_arr)[65]);

    //parse result from shufflePieces to valid FEN notation for dispaly
    void parseFEN(const char chess_set[65], char FEN[71]);

    bool m_simulating = false; //simulating mode
    bool m_viewing = false; //viewing mode

	//size of the chess board in pixels 
    int m_chessBoardSize = DIM_CB_SIZE;

    int m_piecesToRemove = DIM_CP_TO_REMOVE;

    int m_chessPieceIdx = -1;

    int m_numberOfSimulations{};
    double m_totalSimulationTime{};
    double m_averageSimulationTime{};

    int m_mouseDownX{}, m_mouseDownY{};

    SDL_Rect* m_rectPtrChessBoardTile[64];   
    SDL_Rect* m_rectPtrBoardLabelsV[8];
    SDL_Rect* m_rectPtrBoardLabelsH[8];

    SDL_Rect m_rectTextFEN;
    SDL_Rect m_rectTextStats;
    SDL_Rect m_rectButtonSimulator;
    SDL_Rect m_rectButtonViewer;
    SDL_Rect m_rectTextTitle;
    SDL_Rect m_rectWindow;
    SDL_Rect m_rectSliderSlit;
    SDL_Rect m_rectSliderKnob;
    SDL_Rect m_rectFloatingChessTile;

    std::string m_buttonViewerTexID = ID_BTN_VIEWER_UP;
    std::string m_buttonSimulatorTexID = ID_BTN_SIMULATOR_UP;
    std::string m_buttonStartTexID = ID_BTN_START_UP;
    std::string m_buttonStopTexID = ID_BTN_STOP_UP;

    SDL_Color m_colorChessBoard[3] = {COL_CB_LIGHT, COL_CB_DARK, COL_CB_HIGHLIGHT};

    std::queue<ChessBoardDescriptions> m_cbDescriptions;

    std::vector<ChessBoardDescriptions> m_cbDescriptionsVec;
    int m_cbDescriptionsVecSeek {};

    ChessBoardDescriptions m_currentCBDescription;

    Timer m_timer;

};
