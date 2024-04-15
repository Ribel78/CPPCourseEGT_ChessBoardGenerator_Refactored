//ChessBoard.cpp

#include <algorithm>
#include <random>
#include "ChessBoard.h"
#include "Constants.h"
#include "Utilities.h"
#include "TextureFactory.h"
#include <iostream>
#include <fstream>

using namespace Constants;

ChessBoard::ChessBoard()
{

    InitBoardRects();
    SetAndPushCurrentDescription(STR_CB_INIT_DESCR,
                          STR_CB_INIT_FEN,
                          "0.0",
                          std::to_string(32 - m_piecesToRemove));
}

ChessBoard::~ChessBoard()
{
    // delete chess board rectangles
    for (int i = 0; i < 64; i++)
    {
        delete m_rectPtrChessBoardTile[i];
    }

    // delete chess board labels
    for (int i = 0; i < 8; i++)
    {
        delete m_rectPtrBoardLabelsV[i];
        delete m_rectPtrBoardLabelsH[i];
    }
}

auto ChessBoard::IsSimulating() const -> bool
{
    return ChessBoard::m_simulating;
}

auto ChessBoard::IsViewing() const -> bool
{
    return ChessBoard::m_viewing;
}

void ChessBoard::SetViewing(const bool state)
{
    ChessBoard::m_viewing = state;
}

void ChessBoard::ToggleSimulating()
{
    SetSimulating(!IsSimulating());
}
void ChessBoard::ToggleViewing()
{
    SetViewing(!IsViewing());
}

void ChessBoard::SetSimulating(const bool state)
{
    ChessBoard::m_simulating = state;
}

void ChessBoard::SetPiecesToRemove(int amount)
{
    m_piecesToRemove = (amount < 0 || amount > 29) ? DIM_CP_TO_REMOVE : amount;
}

auto ChessBoard::GetPiecesToRemove() -> int
{
    return m_piecesToRemove;
}

/*
Update references to custom and m_fenDescription annotation description strings with
random chess board description
_shuff - bool to shuffle (1) or keep ordered chess set
_&custDescription - string reference to write the chess board description to
_&fenDescription - string reference to write the m_fenDescription notation of the chess board description
*/
void ChessBoard::RunChessBoardSimulator(const bool isSimulation,
                               std::string& customDescription,
                               std::string& fenDescription)
{
    if(isSimulation)
    {
        m_timer.MarkStart();

        GenerateRandomChessBoard(m_piecesToRemove, customDescription, fenDescription);

        m_timer.MarkEnd();

        UpdateStatistics();

        SetAndPushCurrentDescription(customDescription,
                              fenDescription,
                                     std::to_string(m_timer.GetDuration()),
                              std::to_string(32 - m_piecesToRemove));
    }
    else
    {
        if(!IsViewing()){
            customDescription = m_cbDescriptions.back().m_customDescription;
            fenDescription = m_cbDescriptions.back().m_fenDescription;
        }
        else
        {
            SetBoardDescriptionFromVector();
            customDescription = GetCurrentDescription().m_customDescription;
            fenDescription = GetCurrentDescription().m_fenDescription;
        }
    }
}

void ChessBoard::GenerateRandomChessBoard(int removePcs,
                                          std::string& customDescription,
                                          std::string& fenDescription)
{
    char chessSet[] = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";
    bool repeatSimulation = true;

    while(repeatSimulation == true)
    {
        ShuffleChessSet(chessSet);

        if(IsIllegalBishops(chessSet))
            continue;

        int piecesToRemove = removePcs;

        while(piecesToRemove >= 0)
        {
            RemoveIllegalPawnsAndKings(chessSet, piecesToRemove);

            if(!isExtraPiecesToRemove(chessSet, piecesToRemove))
                break;
        }

        if (piecesToRemove < 0 || IsIllegalPawn(chessSet))
        {
            continue;
        }

        AddKingsToBoard(chessSet);

        customDescription = chessSet;
        fenDescription = ParseToFENDescription(chessSet);

        repeatSimulation = false;
    }
}

/*
Parse custom chess board description to FEN notation
*/
std::string ChessBoard::ParseToFENDescription(const char chessSet[65])
{
    //char array to hold the positions including separators '/' (64+7)
    char FEN[71] {};
    char tempFEN[71];

    int j = 0;

    for (int i = 0; i < 64; i++)
    {
        tempFEN[j] = chessSet[i];
        if ((i + 1) % 8 == 0 && ((i + 1) > 0 && (i + 1) < 64))
        {
            j += 1;
            tempFEN[j] = '/';
        }
        j += 1;
    }

    int emptySpace = 0;

    j = 0;

    int count = 0;

    while (count < 71)
    {
        if (tempFEN[count] != '-' && emptySpace == 0)
        {

            FEN[j] = tempFEN[count];
            count += 1;
            j += 1;
        }
        else if (tempFEN[count] == '-')
        {
            emptySpace += 1;
            FEN[j] = ('0' + emptySpace);
            count += 1;
        }
        else
        {
            j += 1;
            emptySpace = 0;
        }
    }
    return std::string(FEN);
}

void ChessBoard::SetBoardDescriptionFromQueueBack()
{
    m_currentCBDescription.m_customDescription = m_cbDescriptions.back().m_customDescription;
    m_currentCBDescription.m_chessPieces = m_cbDescriptions.back().m_chessPieces;
}

void ChessBoard::SetBoardDescriptionFromEdit(std::string descr)
{
    m_currentCBDescription.m_customDescription = descr;
}

void ChessBoard::SetBoardDescriptionFromVector()
{
    int idx = m_cbDescriptionsVecSeek % m_cbDescriptionsVec.size();
    m_currentCBDescription.m_customDescription = m_cbDescriptionsVec.at(idx).m_customDescription;
    m_currentCBDescription.m_fenDescription = m_cbDescriptionsVec.at(idx).m_fenDescription;
    m_currentCBDescription.m_simulationTime = m_cbDescriptionsVec.at(idx).m_simulationTime;
    m_currentCBDescription.m_chessPieces = m_cbDescriptionsVec.at(idx).m_chessPieces;
}

auto ChessBoard::GetMutableCBDescriptionsVecSeek() -> int&
{
    return m_cbDescriptionsVecSeek;
}

auto ChessBoard::GetMutableDescriptionsQueue() -> std::queue<ChessBoardDescriptions>&
{
    return m_cbDescriptions;
}

auto ChessBoard::GetMutableDescriptionsVector() -> std::vector<ChessBoardDescriptions>&
{
    return m_cbDescriptionsVec;
}

auto ChessBoard::GetCurrentDescription() const -> ChessBoardDescriptions
{
    return m_currentCBDescription;
}

void ChessBoard::SetAndPushCurrentDescription(std::string cd,
                                       std::string fen,
                                       std::string simulationTime,
                                       std::string numberOfPieces)
{
    m_currentCBDescription = {cd, fen, simulationTime, numberOfPieces};

    m_cbDescriptions.push(m_currentCBDescription);

    if(m_cbDescriptions.size() == DIM_QUEUE_LAST_SIMULATIONS + 1)
    {
        m_cbDescriptions.pop();
    }
}

void ChessBoard::SetChessPieceIdx(int idx)
{
    m_chessPieceIdx = idx;
}

auto ChessBoard::GetChessPieceIdx() const -> int
{
    return m_chessPieceIdx;
}

auto ChessBoard::GetRectChessBoardTile(int idx) const -> SDL_Rect*
{
    return m_rectPtrChessBoardTile[idx];
}





auto ChessBoard::GetSimulationSummary() const -> std::string
{
    std::string timeStatsString {};
    if(!IsViewing())
    {
        timeStatsString.append("Chess pieces: ");
        timeStatsString.append(m_currentCBDescription.m_chessPieces);
        timeStatsString.append("\n");
        timeStatsString.append("Number of simulations: ");
        timeStatsString.append(std::to_string(m_numberOfSimulations));
        timeStatsString.append("\n");
        timeStatsString.append("Last Simulation Time: ");
        timeStatsString.append(std::to_string(m_timer.GetDuration()));
        timeStatsString.append(" ns\n");
        timeStatsString.append("Total Simulation Time: ");
        timeStatsString.append(std::to_string(m_totalSimulationTime));
        timeStatsString.append(" ns\n");
        timeStatsString.append("Average Simulation Time: ");
        timeStatsString.append(std::to_string(m_averageSimulationTime));
        timeStatsString.append(" ns");
    }
    else
    {
        timeStatsString.append("Board ");
        timeStatsString.append(std::to_string(m_cbDescriptionsVecSeek + 1));
        timeStatsString.append(" of ");
        timeStatsString.append(std::to_string(m_cbDescriptionsVec.size()));
        timeStatsString.append("\n");
        timeStatsString.append("Simulation Time: ");
        timeStatsString.append(m_currentCBDescription.m_simulationTime);
        timeStatsString.append("\n");
        timeStatsString.append("Number of pieces: ");
        timeStatsString.append(m_currentCBDescription.m_chessPieces);
        timeStatsString.append("\n\n");
    }

    return timeStatsString;
}

void ChessBoard::UpdateStatistics()
{
    m_numberOfSimulations += 1;
    m_totalSimulationTime += m_timer.GetDuration();
    m_averageSimulationTime = m_totalSimulationTime / m_numberOfSimulations;
}

void ChessBoard::ResetSimulationStatistics()
{
    m_numberOfSimulations = 0;
    m_totalSimulationTime = 0;
    m_averageSimulationTime = 0;
}

void ChessBoard::InitBoardRects()
{
    //init and position squares rects
    for (int i = 0; i < 64; i++)
    {
        m_rectPtrChessBoardTile[i] = new SDL_Rect{0, 0, 0, 0};
        m_rectPtrChessBoardTile[i]->x = (i % 8)*(m_chessBoardSize / 8);
        m_rectPtrChessBoardTile[i]->y = (i / 8)*(m_chessBoardSize / 8);
        m_rectPtrChessBoardTile[i]->w = m_rectPtrChessBoardTile[i]->h = m_chessBoardSize / 8;
    }
    //init label rects
    for (int i = 0; i < 8; i++)
    {
        m_rectPtrBoardLabelsV[i] = new SDL_Rect{0, 0, 0, 0};
        m_rectPtrBoardLabelsH[i] = new SDL_Rect{0, 0, 0, 0};

        m_rectPtrBoardLabelsV[i]->x = 0;
        m_rectPtrBoardLabelsV[i]->y = (i * DIM_CB_TILE_SIZE);
        m_rectPtrBoardLabelsV[i]->w = m_rectPtrBoardLabelsV[i]->h = DIM_CB_TILE_SIZE/4;

        m_rectPtrBoardLabelsH[i]->x = (i * DIM_CB_TILE_SIZE) + (DIM_CB_TILE_SIZE - DIM_CB_TILE_SIZE/4);
        m_rectPtrBoardLabelsH[i]->y = (DIM_CB_TILE_SIZE * 7) + (DIM_CB_TILE_SIZE - DIM_CB_TILE_SIZE/4);
        m_rectPtrBoardLabelsH[i]->w = m_rectPtrBoardLabelsH[i]->h = DIM_CB_TILE_SIZE/4;
    }

    //random tile for chess board texture variation
    m_rectFloatingChessTile = {0, 0, 64, 64};
}

void ChessBoard::DrawBoard()
{
    SDL_SetRenderDrawBlendMode(TextureFactory::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
    for (int i = 0; i < 64; i++)
    {
		SDL_SetRenderDrawColor(
            TextureFactory::Instance()->GetRenderer(),
            m_colorChessBoard[(i + ( i / 8 ) % 2 ) %2].r,
            m_colorChessBoard[(i + ( i / 8 ) % 2 ) %2].g,
            m_colorChessBoard[(i + ( i / 8 ) % 2 ) %2].b,
            m_colorChessBoard[(i + ( i / 8 ) % 2 ) %2].a
            );

        SDL_RenderFillRect(TextureFactory::Instance()->GetRenderer(),
                           m_rectPtrChessBoardTile[i]);

        srand (i);

        m_rectFloatingChessTile.x = rand()%(640-64);
        m_rectFloatingChessTile.y = rand()%(640-64);

        TextureFactory::Instance()->SetTextureAlpha(ID_CHESS_TILE, 55);
        TextureFactory::Instance()->DrawTexture(ID_CHESS_TILE, &m_rectFloatingChessTile, m_rectPtrChessBoardTile[i]);
	}

    // label textures

    SDL_SetRenderDrawBlendMode(TextureFactory::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);

    for(int i = 0; i < 8 ; i++)
    {
        TextureFactory::Instance()->DrawTexture(std::string(1, ('a' + i)),
                                                nullptr, m_rectPtrBoardLabelsH[i]);

        TextureFactory::Instance()->DrawTexture(std::string(1, ('8' - i)),
                                                nullptr, m_rectPtrBoardLabelsV[i]);
    }
}

void ChessBoard::DrawBoardOverlay()
{
    if (!m_simulating && m_chessPieceIdx > -1)
    {
        int x = m_chessPieceIdx % 8;
        int y = m_chessPieceIdx / 8;

        std::string overlay = AttackSquares(m_currentCBDescription.m_customDescription, x, y, '\0' );

        for (int i = 0; i < 64; i++)
        {
            SDL_SetRenderDrawBlendMode(TextureFactory::Instance()->GetRenderer(),
                                       SDL_BLENDMODE_BLEND);

            //hide unused rectangles with 0 alpha value
            SDL_SetRenderDrawColor(TextureFactory::Instance()->GetRenderer(),
                                   m_colorChessBoard[2].r,
                                   m_colorChessBoard[2].g,
                                   m_colorChessBoard[2].b,
                                   ((overlay[i]!='-') ? 100 : 0));

            SDL_RenderFillRect(TextureFactory::Instance()->GetRenderer(),
                               m_rectPtrChessBoardTile[i]);
		}

    }
}

void ChessBoard::DrawPieces()
{
    std::string chessBoardShuffle {};
    std::string fenChessBoard {};

    //init descriptions
    RunChessBoardSimulator(IsSimulating(), chessBoardShuffle, fenChessBoard);

    for (int i = 0; i < 64; i++)
    {
        if (chessBoardShuffle[i] == '-')
        {
			continue;
		}
        for (int j = 0; j < 12; j++)
        {
            if (chessBoardShuffle[i] == STR_CB_LOOKUPREF[j])
            {
                TextureFactory::Instance()->DrawTexture(std::to_string(STR_CB_LOOKUPREF[j]),
                                                        nullptr,
                                                        m_rectPtrChessBoardTile[i]);
                //Variant 2 get from texture pack with index number
                // SDL_RenderCopy(TextureFactory::instance()->getRenderer(),
                //                TextureFactory::instance()->getTextureFromPack("chess_pack", j),
                //                NULL,
                //                m_RectPtrChessBoardTile[i]);
				continue;
			}
		}
    }
}

void ChessBoard::ViewDescriptionNext()
{
    if(!IsViewing())
    {
        std::string tempCustDescr, tempFENDescr, tempSimTime, tempChessPcs;
        tempCustDescr = GetMutableDescriptionsQueue().front().m_customDescription;
        tempFENDescr = GetMutableDescriptionsQueue().front().m_fenDescription;
        tempSimTime = GetMutableDescriptionsQueue().front().m_simulationTime;
        tempChessPcs = GetMutableDescriptionsQueue().front().m_chessPieces;
        GetMutableDescriptionsQueue().pop();
        GetMutableDescriptionsQueue().push({tempCustDescr, tempFENDescr, tempSimTime, tempChessPcs});
        SetBoardDescriptionFromQueueBack();
    }
    else
    {
        GetMutableCBDescriptionsVecSeek() += 1;
        GetMutableCBDescriptionsVecSeek() %= GetMutableDescriptionsVector().size();
        SetBoardDescriptionFromVector();
    }
}

void ChessBoard::ViewDescriptionPrevious()
{
    if(!IsViewing())
    {
        for (int i = 0; i < GetMutableDescriptionsQueue().size()-1; i++)
        {
            ChessBoardDescriptions temp = GetMutableDescriptionsQueue().front();
            GetMutableDescriptionsQueue().pop();
            GetMutableDescriptionsQueue().push(temp);
        }
    }
    else
    {
        if (GetMutableCBDescriptionsVecSeek() - 1 < 0)
            GetMutableCBDescriptionsVecSeek() = GetMutableDescriptionsVector().size()-1;
        else
            GetMutableCBDescriptionsVecSeek() -= 1;
        SetBoardDescriptionFromVector();
    }
}

void ChessBoard::CopyFENtoClipboard()
{
    std::string clipboardText{};

    if(!IsViewing())
        clipboardText = GetMutableDescriptionsQueue().back().m_fenDescription;
    else
        clipboardText = GetCurrentDescription().m_fenDescription;

    SDL_SetClipboardText(clipboardText.c_str());
}



void ChessBoard::ShuffleChessSet(char (&chessSet)[65])
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine randEngine;
    randEngine.seed(seed);
    //random chess board with all pieces
    std::shuffle(chessSet, chessSet+64, randEngine);

    srand(time(nullptr));
}

bool ChessBoard::IsIllegalBishops(char (&chessSet)[65])
{
    int blackBishopOnBlack = 0;
    int blackBishopOnWhite = 0;
    int whiteBishopOnBlack = 0;
    int whiteBishopOnWhite = 0;

    for (int i = 0; i < 64; i++)
    {
        if(chessSet[i]=='b'){ // black bishop found
            if ((( i / 8 ) % 2 == 0 && ( i % 8 ) % 2 == 1) ||
                (( i / 8 ) % 2 == 1 && ( i % 8 ) % 2 == 0))
            { // check if color of square is black
                blackBishopOnBlack += 1;
            } else {
                blackBishopOnWhite += 1;
            }
        }
        if(chessSet[i]=='B')
        { // white bishop found
            if ((( i / 8 ) % 2 == 0 && ( i % 8 ) % 2 == 1) ||
                (( i / 8 ) % 2 == 1 && ( i % 8 ) % 2 == 0))
            { // check if color of square is black
                whiteBishopOnBlack += 1;
            } else
            {
                whiteBishopOnWhite += 1;
            }
        }
    }

    return (blackBishopOnBlack > 1 ||
        blackBishopOnWhite > 1 ||
        whiteBishopOnBlack > 1 ||
            whiteBishopOnWhite > 1 );

}

void ChessBoard::RemoveIllegalPawnsAndKings(char (&chessSet)[65], int& removePscCount)
{
    for(int i = 0; i < 64; i++)
    {
        if(i < 8 || i > (64 - 9))
        {
            if(chessSet[i] == 'p' || chessSet[i] == 'P')
            {
                if (removePscCount - 1 < 0)
                    continue;
                chessSet[i] = '-';
                removePscCount -= 1;
            }
        }

        //Remove both Kings to reintroduce back when board is processed
        if((chessSet[i] == 'k' || chessSet[i] == 'K'))
        {
            chessSet[i] = '-';
        }
    }
}

bool ChessBoard::IsIllegalPawn(char (&chessSet)[65]){

    bool illegal_pawn = false;
    for(int i = 0; i < 64; i++)
    {
        if(i < 8 || i > (64 - 9))
        {
            if(chessSet[i] == 'p' || chessSet[i] == 'P')
            {
                illegal_pawn = true;
                break;
            }
        }
    }
    return illegal_pawn;
}

bool ChessBoard::isExtraPiecesToRemove(char (&chessSet)[65], int& removePscCount)
{
    if(removePscCount > 0)
    {
        int rand_index = rand()%64;

        if(chessSet[rand_index]!= '-')
        {
            chessSet[rand_index] = '-';
            removePscCount -= 1;
        }

        return true;
    }
    else
    {
        return false;
    }
}

void ChessBoard::AddKingsToBoard(char (&chessSet)[65])
{
    // Reintroduce Kings
    // Add  Black King
    bool isSafeSquare = false;

    std::string emptySquaresLookup(chessSet);

    //while isSafeSquare is false
    while(!isSafeSquare)
    {
        // random index
        int rand_index = rand()%64;

        //if not empty start new cycle
        if (chessSet[rand_index] != '-')
        {
            continue;
        }
        for (int blackPiece = 0; blackPiece < 6; blackPiece++)
        { //test each black piece
            std::string attackedPieces = AttackSquares(emptySquaresLookup,
                                                       rand_index % 8,
                                                       rand_index / 8,
                                                       CP_LABELS_BLACK[blackPiece]);

            //for cell in attack board
            for (char piece : attackedPieces)
            {
                //if white piece is oposite of current black piece
                if (piece == CP_LABELS_WHITE[blackPiece])
                {
                    isSafeSquare = false;
                    break;
                } else
                {
                    isSafeSquare = true;
                }
            }
            if (isSafeSquare == false)
            {
                break;
            } else
            {
                isSafeSquare = true;
            }
        }
        if (isSafeSquare == true)
        {
            chessSet[rand_index] = 'k';
        }
    }

    // Add  White King
    isSafeSquare = false;

    emptySquaresLookup = std::string(chessSet);

    //while isSafeSquare is false
    while(!isSafeSquare)
    {
        // random index
        int randIdx = rand()%64;

        //if not empty start new cycle
        if (chessSet[randIdx] != '-')
        {
            continue;
        }

        //test each black piece
        for (int whitePiece = 0; whitePiece < 6; whitePiece++)
        {
            std::string attackedPieces = AttackSquares(emptySquaresLookup,
                                                       randIdx % 8,
                                                       randIdx / 8,
                                                       CP_LABELS_WHITE[whitePiece]);
            //for cell in attack board
            for (char piece : attackedPieces)
            {
                //if white piece is oposite of current black piece
                if (piece == CP_LABELS_BLACK[whitePiece])
                {

                    isSafeSquare = false;
                    break;
                } else
                {
                    isSafeSquare = true;
                }
            }
            if (isSafeSquare == false)
            {
                break;
            } else
            {
                isSafeSquare = true;
            }
        }
        if (isSafeSquare == true)
        {
            chessSet[randIdx] = 'K';
        }
    }
}


void ChessBoard::ReadDescriptionFile(std::fstream& dataStream)
{
    if (!IsViewing())
    {
        GetMutableDescriptionsVector().clear();

        dataStream.open(Constants::FILE_DESCRIPTIONS, std::ios::in);

        ChessBoardDescriptions tempCBDescription;

        while(dataStream >> tempCBDescription)
        {
            GetMutableDescriptionsVector().push_back(tempCBDescription);
        }
        dataStream.close();
    }
}

void ChessBoard::OpenDescriptionFileForWriting(std::fstream& dataStream)
{
    if (IsSimulating())
    {
        dataStream.close();
    }
    else
    {
        dataStream.open(Constants::FILE_DESCRIPTIONS, std::ios::out);
        if(!dataStream.is_open())
            std::cout << "Failed to open data/descriptions.csv" << std::endl;
    }
}

void ChessBoard::SetCurrentBoardDescriptionSrc()
{
    if(!IsViewing())
    {
        SetBoardDescriptionFromQueueBack();
    }
    else
    {
        SetBoardDescriptionFromVector();
    }
}
