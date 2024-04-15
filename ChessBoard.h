#pragma once

#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Timer.h"
#include <string>
#include "Constants.h"

using namespace Constants;

struct ChessBoardDescriptions {
    std::string m_customDescription {};
    std::string m_fenDescription {};
    std::string m_simulationTime {};
    std::string m_chessPieces {};

    friend auto operator >>(std::istream& inputStream, ChessBoardDescriptions& rhs) -> std::istream&
    {
        inputStream >> rhs.m_customDescription
                    >> rhs.m_fenDescription
                    >> rhs.m_simulationTime
                    >> rhs.m_chessPieces;

        return inputStream;
    }
};

class ChessBoard
{
public:
    ChessBoard();
    ~ChessBoard();

    auto IsSimulating() const -> bool;
    auto IsViewing() const -> bool;

    void SetSimulating(const bool state);
    void SetViewing(const bool state);

    void ToggleSimulating();
    void ToggleViewing();

    void SetPiecesToRemove(int amount);
    auto GetPiecesToRemove() -> int;

    void RunChessBoardSimulator(const bool isSimulation,
                       std::string& customDescription,
                       std::string& fenDescription);

    void GenerateRandomChessBoard(int removePcs,
                                  std::string& customDescription,
                                  std::string& fenDescription);

    void SetBoardDescriptionFromQueueBack();
    void SetBoardDescriptionFromVector();
    void SetBoardDescriptionFromEdit(std::string descr);

    auto GetMutableCBDescriptionsVecSeek() -> int&;

    auto GetMutableDescriptionsQueue() -> std::queue<ChessBoardDescriptions>&;

    auto GetMutableDescriptionsVector() -> std::vector<ChessBoardDescriptions>&;

    auto GetCurrentDescription() const -> ChessBoardDescriptions;
    void SetAndPushCurrentDescription(std::string cd,
                               std::string fen,
                               std::string simulationTime,
                               std::string numberOfPieces);

    void SetChessPieceIdx(int idx);
    auto GetChessPieceIdx() const -> int;

    auto GetRectChessBoardTile(int idx) const -> SDL_Rect*;

    auto GetSimulationSummary() const -> std::string;
    void UpdateStatistics();
    void ResetSimulationStatistics();

    void InitBoardRects();

    void DrawBoard();
    void DrawBoardOverlay();
    void DrawPieces();

    void ViewDescriptionNext();
    void ViewDescriptionPrevious();
    void CopyFENtoClipboard();

    void ReadDescriptionFile(std::fstream& dataStream);
    void OpenDescriptionFileForWriting(std::fstream& dataStream);
    void SetCurrentBoardDescriptionSrc();

    std::string ParseToFENDescription(const char chessSet[65]);

private:

    //shifflePieces helper functions
    void ShuffleChessSet(char (&chessSet)[65]);
    auto IsIllegalBishops(char (&chessSet)[65]) -> bool;
    void RemoveIllegalPawnsAndKings(char (&chessSet)[65], int& removePscCount);
    bool IsIllegalPawn(char (&chessSet)[65]);
    auto isExtraPiecesToRemove(char (&chessSet)[65], int& removePscCount) -> bool;
    void AddKingsToBoard(char (&chessSet)[65]);


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
