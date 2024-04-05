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

    void setPiecesToRemove(int amount);
    auto getPiecesToRemove() -> int;

    void runChessBoardSimulator(const bool isSimulation,
                       std::string& custDescription,
                       std::string& fenDescription);

    void generateRandomChessBoard(int removePcs,
                                  std::string& custDescription,
                                  std::string& fenDescription);

    void setBoardDescriptionFromQueueBack();
    void setBoardDescriptionFromVector();

    auto getMutableCBDescriptionsVecSeek() -> int&;

    auto getMutableDescriptionsQueue() -> std::queue<ChessBoardDescriptions>&;

    auto getMutableDescriptionsVector() -> std::vector<ChessBoardDescriptions>&;

    auto getCurrentDescription() const -> ChessBoardDescriptions;
    void setAndPushCurrentDescription(std::string cd,
                               std::string fen,
                               std::string sim_time,
                               std::string n_pieces);

    void setChessPieceIdx(int idx);

    auto getRectChessBoardTile(int idx) const -> SDL_Rect*;

    auto getSimulationSummary() const -> std::string;
    void updateStatistics();
    void resetSimulationStatistics();

    void initBoardRects();

    void drawBoard();
	void drawBoardOverlay();
    void drawPieces();

    void viewDescriptionNext();
    void viewDescriptionPrevious();
    void copyFENtoClipboard();

    void readDescriptionFile(std::fstream& dataStream);
    void openDescriptionFileForWriting(std::fstream& dataStream);
    void setCurrentBoardDescriptionSrc();


private:

    //shifflePieces helper functions
    void shuffleChessSet(char (&char_array)[65]);
    auto isIllegalBishops(char (&char_array)[65]) -> bool;
    void removeIllegalPawnsAndKings(char (&char_array)[65], int& mutable_int);
    bool isIllegalPawn(char (&char_array)[65]);
    auto isExtraPiecesToRemove(char (&char_array)[65], int& mutable_int) -> bool;
    void addKingsToBoard(char (&char_arr)[65]);

    std::string parseFEN(const char chess_set[65]);

    bool m_simulating = false; //simulating mode
    bool m_viewing = false; //viewing mode

	//size of the chess board in pixels 
    int m_chessBoardSize = DIM_CB_SIZE;
    int m_piecesToRemove = DIM_CP_TO_REMOVE;
    int m_chessPieceIdx = -1;

    int m_numberOfSimulations{};
    double m_totalSimulationTime{};
    double m_averageSimulationTime{};

    SDL_Rect* m_rectPtrChessBoardTile[64];   
    SDL_Rect* m_rectPtrBoardLabelsV[8];
    SDL_Rect* m_rectPtrBoardLabelsH[8];


    SDL_Rect m_rectFloatingChessTile;

    SDL_Color m_colorChessBoard[3] = {COL_CB_LIGHT, COL_CB_DARK, COL_CB_HIGHLIGHT};

    std::queue<ChessBoardDescriptions> m_cbDescriptions;

    std::vector<ChessBoardDescriptions> m_cbDescriptionsVec;
    int m_cbDescriptionsVecSeek {};

    ChessBoardDescriptions m_currentCBDescription;

    Timer m_timer;

};
