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

    initBoardRects();
    setAndPushCurrentDescription(STR_CB_INIT_DESCR,
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

auto ChessBoard::isSimulating() const -> bool
{
    return ChessBoard::m_simulating;
}

auto ChessBoard::isViewing() const -> bool
{
    return ChessBoard::m_viewing;
}

void ChessBoard::setViewing(const bool state)
{
    ChessBoard::m_viewing = state;
}

void ChessBoard::toggleSimulating()
{
    setSimulating(!isSimulating());
}
void ChessBoard::toggleViewing()
{
    setViewing(!isViewing());
}

void ChessBoard::setSimulating(const bool state)
{
    ChessBoard::m_simulating = state;
}

void ChessBoard::setPiecesToRemove(int amount)
{
    m_piecesToRemove = (amount < 0 || amount > 29) ? DIM_CP_TO_REMOVE : amount;
}

auto ChessBoard::getPiecesToRemove() -> int
{
    return m_piecesToRemove;
}

/*
Update references to custom and FEN annotation description strings with
random chess board description
_shuff - bool to shuffle (1) or keep ordered chess set
_&custDescription - string reference to write the chess board description to
_&fenDescription - string reference to write the FEN notation of the chess board description
*/
void ChessBoard::runChessBoardSimulator(const bool isSimulation,
                               std::string& custDescription,
                               std::string& fenDescription)
{
    if (isSimulation)
    {
        m_timer.markStart();

        generateRandomChessBoard(m_piecesToRemove, custDescription, fenDescription);

        m_timer.markEnd();

        updateStatistics();

        setAndPushCurrentDescription(custDescription,
                              fenDescription,
                              std::to_string(m_timer.getDuration()),
                              std::to_string(32 - m_piecesToRemove));
    }
    else
    {
        if(!isViewing()){
            custDescription = m_cbDescriptions.back().Custom;
            fenDescription = m_cbDescriptions.back().FEN;
        }
        else
        {
            setBoardDescriptionFromVector();
            custDescription = getCurrentDescription().Custom;
            fenDescription = getCurrentDescription().FEN;
        }
    }
}

void ChessBoard::generateRandomChessBoard(int removePcs,
                                          std::string& custDescription,
                                          std::string& fenDescription)
{
    char chess_set[] = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";
    bool repeatSimulation = true;

    while(repeatSimulation == true)
    {
        shuffleChessSet(chess_set);

        if(isIllegalBishops(chess_set))
            continue;

        int pieces_to_remove = removePcs;

        while(pieces_to_remove >= 0)
        {
            removeIllegalPawnsAndKings(chess_set, pieces_to_remove);

            if(!isExtraPiecesToRemove(chess_set, pieces_to_remove))
                break;
        }

        if (pieces_to_remove < 0 || isIllegalPawn(chess_set))
        {
            continue;
        }

        addKingsToBoard(chess_set);

        custDescription = chess_set;
        fenDescription = parseFEN(chess_set);

        repeatSimulation = false;
    }
}

/*
Parse custom chess board description to FEN notation
*/
std::string ChessBoard::parseFEN(const char chess_set[65])
{
    //char array to hold the positions including separators '/' (64+7)
    char FEN[71] {};
    char tempFEN[71];

    int j = 0;

    for (int i = 0; i < 64; i++)
    {
        tempFEN[j] = chess_set[i];
        if ((i + 1) % 8 == 0 && ((i + 1) > 0 && (i + 1) < 64))
        {
            j += 1;
            tempFEN[j] = '/';
        }
        j += 1;
    }

    int empty_space = 0;

    j = 0;

    int count = 0;

    while (count < 71)
    {
        if (tempFEN[count] != '-' && empty_space == 0)
        {

            FEN[j] = tempFEN[count];
            count += 1;
            j += 1;
        }
        else if (tempFEN[count] == '-')
        {
            empty_space += 1;
            FEN[j] = ('0' + empty_space);
            count += 1;
        }
        else
        {
            j += 1;
            empty_space = 0;
        }
    }
    return std::string(FEN);
}

void ChessBoard::setBoardDescriptionFromQueueBack()
{
    m_currentCBDescription.Custom = m_cbDescriptions.back().Custom;
    m_currentCBDescription.chess_pieces = m_cbDescriptions.back().chess_pieces;
}

void ChessBoard::setBoardDescriptionFromVector()
{
    int idx = m_cbDescriptionsVecSeek % m_cbDescriptionsVec.size();
    m_currentCBDescription.Custom = m_cbDescriptionsVec.at(idx).Custom;
    m_currentCBDescription.FEN = m_cbDescriptionsVec.at(idx).FEN;
    m_currentCBDescription.simulationTime = m_cbDescriptionsVec.at(idx).simulationTime;
    m_currentCBDescription.chess_pieces = m_cbDescriptionsVec.at(idx).chess_pieces;
}

auto ChessBoard::getMutableCBDescriptionsVecSeek() -> int&
{
    return m_cbDescriptionsVecSeek;
}

auto ChessBoard::getMutableDescriptionsQueue() -> std::queue<ChessBoardDescriptions>&
{
    return m_cbDescriptions;
}

auto ChessBoard::getMutableDescriptionsVector() -> std::vector<ChessBoardDescriptions>&
{
    return m_cbDescriptionsVec;
}

auto ChessBoard::getCurrentDescription() const -> ChessBoardDescriptions
{
    return m_currentCBDescription;
}

void ChessBoard::setAndPushCurrentDescription(std::string cd,
                                       std::string fen,
                                       std::string sim_time,
                                       std::string n_pieces)
{
    m_currentCBDescription = {cd, fen, sim_time, n_pieces};

    m_cbDescriptions.push(m_currentCBDescription);

    if(m_cbDescriptions.size() == DIM_QUEUE_LAST_SIMULATIONS + 1)
    {
        m_cbDescriptions.pop();
    }
}

void ChessBoard::setChessPieceIdx(int idx){
    m_chessPieceIdx = idx;
}

auto ChessBoard::getRectChessBoardTile(int idx) const -> SDL_Rect*
{
    return m_rectPtrChessBoardTile[idx];
}





auto ChessBoard::getSimulationSummary() const -> std::string
{
    std::string timeStatsString {};
    if(!isViewing())
    {
        timeStatsString.append("Chess pieces: ");
        timeStatsString.append(m_currentCBDescription.chess_pieces);
        timeStatsString.append("\n");
        timeStatsString.append("Number of simulations: ");
        timeStatsString.append(std::to_string(m_numberOfSimulations));
        timeStatsString.append("\n");
        timeStatsString.append("Last Simulation Time: ");
        timeStatsString.append(std::to_string(m_timer.getDuration()));
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
        timeStatsString.append(m_currentCBDescription.simulationTime);
        timeStatsString.append("\n");
        timeStatsString.append("Number of pieces: ");
        timeStatsString.append(m_currentCBDescription.chess_pieces);
        timeStatsString.append("\n\n");
    }

    return timeStatsString;
}

void ChessBoard::updateStatistics()
{
    m_numberOfSimulations += 1;
    m_totalSimulationTime += m_timer.getDuration();
    m_averageSimulationTime = m_totalSimulationTime / m_numberOfSimulations;
}

void ChessBoard::resetSimulationStatistics()
{
    m_numberOfSimulations = 0;
    m_totalSimulationTime = 0;
    m_averageSimulationTime = 0;
}

void ChessBoard::initBoardRects()
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

void ChessBoard::drawBoard()
{
    SDL_SetRenderDrawBlendMode(TextureFactory::instance()->getRenderer(), SDL_BLENDMODE_BLEND);
    for (int i = 0; i < 64; i++)
    {
		SDL_SetRenderDrawColor(
            TextureFactory::instance()->getRenderer(),
            m_colorChessBoard[(i + ( i / 8 ) % 2 ) %2].r,
            m_colorChessBoard[(i + ( i / 8 ) % 2 ) %2].g,
            m_colorChessBoard[(i + ( i / 8 ) % 2 ) %2].b,
            m_colorChessBoard[(i + ( i / 8 ) % 2 ) %2].a
            );

        SDL_RenderFillRect(TextureFactory::instance()->getRenderer(),
                           m_rectPtrChessBoardTile[i]);

        srand (i);

        m_rectFloatingChessTile.x = rand()%(640-64);
        m_rectFloatingChessTile.y = rand()%(640-64);

        TextureFactory::instance()->setTextureAlpha(ID_CHESS_TILE, 55);
        TextureFactory::instance()->drawTexture(ID_CHESS_TILE, &m_rectFloatingChessTile, m_rectPtrChessBoardTile[i]);
	}

    // label textures

    SDL_SetRenderDrawBlendMode(TextureFactory::instance()->getRenderer(), SDL_BLENDMODE_BLEND);

    for(int i = 0; i < 8 ; i++)
    {
        TextureFactory::instance()->drawTexture(std::string(1, ('a' + i)),
                                                NULL, m_rectPtrBoardLabelsH[i]);

        TextureFactory::instance()->drawTexture(std::string(1, ('8' - i)),
                                                NULL, m_rectPtrBoardLabelsV[i]);
    }
}

void ChessBoard::drawBoardOverlay()
{
    if (!m_simulating && m_chessPieceIdx > -1)
    {
        int x = m_chessPieceIdx % 8;
        int y = m_chessPieceIdx / 8;

        std::string overlay = attackSquares(m_currentCBDescription.Custom, x, y, '\0' );

        for (int i = 0; i < 64; i++)
        {
            SDL_SetRenderDrawBlendMode(TextureFactory::instance()->getRenderer(),
                                       SDL_BLENDMODE_BLEND);

            //hide unused rectangles with 0 alpha value
            SDL_SetRenderDrawColor(TextureFactory::instance()->getRenderer(),
                                   m_colorChessBoard[2].r,
                                   m_colorChessBoard[2].g,
                                   m_colorChessBoard[2].b,
                                   ((overlay[i]!='-') ? 100 : 0));

            SDL_RenderFillRect(TextureFactory::instance()->getRenderer(),
                               m_rectPtrChessBoardTile[i]);
		}

    }
}

void ChessBoard::drawPieces()
{
    std::string chessBoardShuffle {};
    std::string fenChessBoard {};

    //init descriptions
    runChessBoardSimulator(isSimulating(), chessBoardShuffle, fenChessBoard);

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
                TextureFactory::instance()->drawTexture(std::to_string(STR_CB_LOOKUPREF[j]),
                                                        NULL,
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

void ChessBoard::viewDescriptionNext()
{
    if(!isViewing())
    {
        std::string tempCustDescr, tempFENDescr, tempSimTime, tempChessPcs;
        tempCustDescr = getMutableDescriptionsQueue().front().Custom;
        tempFENDescr = getMutableDescriptionsQueue().front().FEN;
        tempSimTime = getMutableDescriptionsQueue().front().simulationTime;
        tempChessPcs = getMutableDescriptionsQueue().front().chess_pieces;
        getMutableDescriptionsQueue().pop();
        getMutableDescriptionsQueue().push({tempCustDescr, tempFENDescr, tempSimTime, tempChessPcs});
        setBoardDescriptionFromQueueBack();
    }
    else
    {
        getMutableCBDescriptionsVecSeek() += 1;
        getMutableCBDescriptionsVecSeek() %= getMutableDescriptionsVector().size();
        setBoardDescriptionFromVector();
    }
}

void ChessBoard::viewDescriptionPrevious()
{
    if(!isViewing())
    {
        for (int i = 0; i < getMutableDescriptionsQueue().size()-1; i++)
        {
            ChessBoardDescriptions temp = getMutableDescriptionsQueue().front();
            getMutableDescriptionsQueue().pop();
            getMutableDescriptionsQueue().push(temp);
        }
    }
    else
    {
        if (getMutableCBDescriptionsVecSeek() - 1 < 0)
            getMutableCBDescriptionsVecSeek() = getMutableDescriptionsVector().size()-1;
        else
            getMutableCBDescriptionsVecSeek() -= 1;
        setBoardDescriptionFromVector();
    }
}

void ChessBoard::copyFENtoClipboard()
{
    std::string clipboard_text{};

    if(!isViewing())
        clipboard_text = getMutableDescriptionsQueue().back().FEN;
    else
        clipboard_text = getCurrentDescription().FEN;

    SDL_SetClipboardText(clipboard_text.c_str());
}



void ChessBoard::shuffleChessSet(char (&char_array)[65])
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rand_en;
    rand_en.seed(seed);
    //random chess board with all pieces
    std::shuffle(char_array, char_array+64, rand_en);

    srand(time(NULL));
}

bool ChessBoard::isIllegalBishops(char (&char_array)[65])
{
    int blackBishopOnBlack = 0;
    int blackBishopOnWhite = 0;
    int whiteBishopOnBlack = 0;
    int whiteBishopOnWhite = 0;

    for (int i = 0; i < 64; i++)
    {
        if(char_array[i]=='b'){ // black bishop found
            if ((( i / 8 ) % 2 == 0 && ( i % 8 ) % 2 == 1) ||
                (( i / 8 ) % 2 == 1 && ( i % 8 ) % 2 == 0))
            { // check if color of square is black
                blackBishopOnBlack += 1;
            } else {
                blackBishopOnWhite += 1;
            }
        }
        if(char_array[i]=='B')
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

void ChessBoard::removeIllegalPawnsAndKings(char (&char_array)[65], int& mutable_int)
{
    for(int i = 0; i < 64; i++)
    {
        if(i < 8 || i > (64 - 9))
        {
            if(char_array[i] == 'p' || char_array[i] == 'P')
            {
                if (mutable_int - 1 < 0)
                    continue;
                char_array[i] = '-';
                mutable_int -= 1;
            }
        }

        //Remove both Kings to reintroduce back when board is processed
        if((char_array[i] == 'k' || char_array[i] == 'K'))
        {
            char_array[i] = '-';
        }
    }
}

bool ChessBoard::isIllegalPawn(char (&char_array)[65]){

    bool illegal_pawn = false;
    for(int i = 0; i < 64; i++)
    {
        if(i < 8 || i > (64 - 9))
        {
            if(char_array[i] == 'p' || char_array[i] == 'P')
            {
                illegal_pawn = true;
                break;
            }
        }
    }
    return illegal_pawn;
}

bool ChessBoard::isExtraPiecesToRemove(char (&char_array)[65], int& mutable_int)
{
    if(mutable_int > 0)
    {
        int rand_index = rand()%64;

        if(char_array[rand_index]!= '-')
        {
            char_array[rand_index] = '-';
            mutable_int -= 1;
        }

        return true;
    }
    else
    {
        return false;
    }
}

void ChessBoard::addKingsToBoard(char (&char_arr)[65])
{
    // Reintroduce Kings
    // Add  Black King
    bool isSafeSquare = false;

    std::string emptySquaresLookup(char_arr);

    //while isSafeSquare is false
    while(!isSafeSquare)
    {
        // random index
        int rand_index = rand()%64;

        //if not empty start new cycle
        if (char_arr[rand_index] != '-')
        {
            continue;
        }
        for (int b_p = 0; b_p < 6; b_p++)
        { //test each black piece
            std::string attackedPieces = attackSquares(emptySquaresLookup,
                                                       rand_index % 8,
                                                       rand_index / 8,
                                                       CP_LABELS_BLACK[b_p]);

            //for cell in attack board
            for (char piece : attackedPieces)
            {
                //if white piece is oposite of current black piece
                if (piece == CP_LABELS_WHITE[b_p])
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
            char_arr[rand_index] = 'k';
        }
    }

    // Add  White King
    isSafeSquare = false;

    emptySquaresLookup = std::string(char_arr);

    //while isSafeSquare is false
    while(!isSafeSquare)
    {
        // random index
        int rand_index = rand()%64;

        //if not empty start new cycle
        if (char_arr[rand_index] != '-')
        {
            continue;
        }

        //test each black piece
        for (int w_p = 0; w_p < 6; w_p++)
        {
            std::string attackedPieces = attackSquares(emptySquaresLookup,
                                                       rand_index % 8,
                                                       rand_index / 8,
                                                       CP_LABELS_WHITE[w_p]);
            //for cell in attack board
            for (char piece : attackedPieces)
            {
                //if white piece is oposite of current black piece
                if (piece == CP_LABELS_BLACK[w_p])
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
            char_arr[rand_index] = 'K';
        }
    }
}


void ChessBoard::readDescriptionFile(std::fstream& dataStream)
{
    if (!isViewing())
    {
        getMutableDescriptionsVector().clear();

        dataStream.open(Constants::FILE_DESCRIPTIONS, std::ios::in);

        ChessBoardDescriptions temp_cb_descr;

        while(dataStream >> temp_cb_descr)
        {
            getMutableDescriptionsVector().push_back(temp_cb_descr);
        }
        dataStream.close();
    }
}

void ChessBoard::openDescriptionFileForWriting(std::fstream& dataStream)
{
    if (isSimulating())
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

void ChessBoard::setCurrentBoardDescriptionSrc()
{
    if(!isViewing())
    {
        setBoardDescriptionFromQueueBack();
    }
    else
    {
        setBoardDescriptionFromVector();
    }
}
