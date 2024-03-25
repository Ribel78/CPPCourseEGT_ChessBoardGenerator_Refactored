//ChessBoard.cpp

#include <algorithm>
#include <random>
#include "ChessBoard.h"
#include "Constants.h"
#include "Utilities.h"
#include "TextureFactory.h"

ChessBoard::ChessBoard()
{
    using namespace Constants;
    //Posible UI feature
    setPiecesToRemove(16);

    //simulate chessboard
    m_simulating = false;
    m_viewing = false;

	//Chess Board Size and Color
    m_chessBoardSize = DIM_CB_SIZE;
    m_chessBoardColor[0] = COL_CB_LIGHT; //"light" square
    m_chessBoardColor[1] = COL_CB_DARK; //"dark" square
    m_chessBoardColor[2] = COL_CB_HIGHLIGHT; // highlight color

    for (int i = 0; i < 64; i++)
    {
        m_chessBoardSquare[i] = new SDL_Rect{0, 0, 0, 0};
    }

    for (int i = 0; i < 8; i++)
    {
        m_chessBoardLabelsV[i] = new SDL_Rect{0, 0, 0, 0};
        m_chessBoardLabelsH[i] = new SDL_Rect{0, 0, 0, 0};
    }
    m_srcChessTileRect = new SDL_Rect{0, 0, 64, 64};

    int padding = 40;
    int chess_sq = (DIM_WINDOW_WIDTH / 2) / 8;

    m_textFENRect = {padding / 2, DIM_WINDOW_WIDTH / 2 + 10,
                     DIM_WINDOW_WIDTH / 2 - padding,
                     chess_sq - padding / 2};

    m_textTimeRect = {DIM_WINDOW_WIDTH / 2 + padding,
                      chess_sq * 2 ,
                      DIM_WINDOW_WIDTH / 2 - 2*padding,
                      DIM_WINDOW_HEIGHT / 3};
    m_buttonSimulationRect = {DIM_WINDOW_WIDTH / 2 + padding,
                              DIM_WINDOW_WIDTH / 2 - chess_sq,
                              (DIM_WINDOW_WIDTH / 2 - 3*padding)/2,
                              chess_sq};
    m_buttonViewerRect = {DIM_WINDOW_WIDTH / 2 + 2*padding + (DIM_WINDOW_WIDTH / 2 - 3*padding)/2,
                          DIM_WINDOW_WIDTH / 2 - chess_sq,
                          (DIM_WINDOW_WIDTH / 2 - 3*padding)/2,
                          chess_sq};
    m_textTitleRect = {DIM_WINDOW_WIDTH / 2 + padding,
                       padding / 2,
                       DIM_WINDOW_WIDTH / 2 - 2*padding,
                       chess_sq};

    m_windowRect = {0, 0, DIM_WINDOW_WIDTH, DIM_WINDOW_HEIGHT};

    m_chessPieceIdx = -1;

    m_current_CB_Description = {STR_CB_INIT_DESCR, STR_CB_INIT_FEN, "0.0"};
    m_CB_Descriptions.push(m_current_CB_Description);
}

ChessBoard::~ChessBoard()
{
    // delete chess board rectangles
    for (int i = 0; i < 64; i++)
    {
        delete m_chessBoardSquare[i];
    }

    // delete chess board labels
    for (int i = 0; i < 8; i++)
    {
        delete m_chessBoardLabelsV[i];
        delete m_chessBoardLabelsH[i];
    }
    delete m_srcChessTileRect;

}

void ChessBoard::prepChessPieceTextures()
{
	// Create textures from font chess characters - only the black pieces (6)
    TTF_Font* DejaVu = TextureFactory::instance()->getFont("DejaVu");

    SDL_Surface* tempSurfaceText = NULL;

    using namespace Constants;

    for (int i = 0; i < 12; i++)
    {
        if (i / 6 == 0)
        { // white chess glyphs - K, Q, R, B, N, P

            tempSurfaceText = TTF_RenderUTF8_Blended(DejaVu, CPB_UNICODE[i % 6].c_str(), COL_CP_LIGHT);
            m_chessPieces[i] = SDL_CreateTextureFromSurface(TextureFactory::instance()->getRenderer(),
                                                            tempSurfaceText);
        }
        else
        { // black chess glyphs - k, q, r, b, n, p
            tempSurfaceText = TTF_RenderUTF8_Blended(DejaVu, CPB_UNICODE[i % 6].c_str(), COL_CP_DARK);
            m_chessPieces[i] = SDL_CreateTextureFromSurface(TextureFactory::instance()->getRenderer(),
                                                            tempSurfaceText);
        }
    }
    SDL_FreeSurface(tempSurfaceText);
}

bool ChessBoard::isSimulating() const
{
    return ChessBoard::m_simulating;
}

bool ChessBoard::isViewing() const
{
    return ChessBoard::m_viewing;
}

void ChessBoard::setViewing(const bool state)
{
    ChessBoard::m_viewing = state;
}

void ChessBoard::setSimulating(const bool state)
{
    ChessBoard::m_simulating = state;
}

void ChessBoard::setPiecesToRemove(int amount)
{
    if (amount < 0 || amount > 30)
    {
        m_piecesToRemove = 8; //default
    } else
    {
        m_piecesToRemove = amount;
    }
}

/*
Randomize a char string of 64 chars of modified FEN chess board description
and update references to custom and FEN annotation string variables
_shuff - bool to shuffle (1) or keep ordered chess set
_&custDescription - string reference to write the chess board description to
_&fenDescription - string reference to write the FEN notation of the chess board description
*/
void ChessBoard::shufflePieces(const bool shuff,
                               std::string& custDescription,
                               std::string& fenDescription)
{

    //Mark start of simulation
    m_timer.markStart();

    //always initialize a chess set with all pieces
    char chess_set[] = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";

    //check if simulation needs to be repeated - cases - same colored bishops or exceeded piece removal
    bool rep_sim = true;

    if (shuff)
    {
        while(rep_sim == true)
        {
            //Shuffle all 32 pieces
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::default_random_engine rand_en;
            rand_en.seed(seed);
            //random chess board with all pieces
            std::shuffle(chess_set, chess_set+64, rand_en);
            //new seed for each subsequent use of rand in the code
            srand(time(NULL));

            // Check if bishops are on different colors if not re-shuffle
            int blackBishopOnBlack = 0;
            int blackBishopOnWhite = 0;
            int whiteBishopOnBlack = 0;
            int whiteBishopOnWhite = 0;

            for (int i = 0; i < 64; i++)
            {
                if(chess_set[i]=='b'){ // black bishop found
                    if ((( i / 8 ) % 2 == 0 && ( i % 8 ) % 2 == 1) ||
                        (( i / 8 ) % 2 == 1 && ( i % 8 ) % 2 == 0))
                    { // check if color of square is black
                        blackBishopOnBlack += 1;
                    } else {
                        blackBishopOnWhite += 1;
                    }
                }
                if(chess_set[i]=='B')
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

            if (blackBishopOnBlack > 1 ||
                blackBishopOnWhite > 1 ||
                whiteBishopOnBlack > 1 ||
                whiteBishopOnWhite > 1 )
            {
                //if Bishops of a kind on same color square - start a new cycle
                continue;
            }

            //remove all pawns if foud on end rows - keep count of removed pieces
            int pieces_to_remove = m_piecesToRemove; // !!! this variable controls how many pieces to see on the board

            while(pieces_to_remove > 0)
            {
                for(int i = 0; i < 64; i++)
                {
                    if(i < 8 || i > (64 - 9))
                    {
                        if(chess_set[i] == 'p' || chess_set[i] == 'P')
                        {
                            chess_set[i] = '-';
                            pieces_to_remove -= 1;
                        }
                    }

                    //Remove both Kings to reintroduce back when board is processed
                    if((chess_set[i] == 'k' || chess_set[i] == 'K'))
                    {
                        chess_set[i] = '-';
                    }
                }
                /*Keep removing randomly selected pieces until m_piecesToRemove pieces in total are removed
                not counting the the removed kings*/
                if(pieces_to_remove > 0)
                {
                    int rand_index = rand()%64;

                    if(chess_set[rand_index]!= '-')
                    {
                        chess_set[rand_index] = '-';
                        pieces_to_remove -= 1;
                    }
                }
            }
            /*
             * pieces_to_remove could be also negative - more pawns on
             * end rows than m_piecesToRemove
             * break the main while loop - start over
            */
            if (pieces_to_remove < 0)
            {
                continue;
            }

            // Reintroduce Kings
            // Add  Black King
            bool isSafeSquare = false;

            std::string emptySquaresLookup(chess_set);

            //while isSafeSquare is false
            while(!isSafeSquare)
            {
                // random index
                int rand_index = rand()%64;

                //if not empty start new cycle
                if (chess_set[rand_index] != '-')
                {
                    continue;
                }
                for (int b_p = 0; b_p < 6; b_p++)
                { //test each black piece
                    std::string attackedPieces = attackSquares(emptySquaresLookup,
                                                               rand_index % 8,
                                                               rand_index / 8,
                                                               Constants::CP_LABELS_BLACK[b_p]);

                    //for cell in attack board
                    for (char piece : attackedPieces)
                    {
                        //if white piece is oposite of current black piece
                        if (piece == Constants::CP_LABELS_WHITE[b_p])
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
                    chess_set[rand_index] = 'k';
                }
            }

            // Add  White King
            isSafeSquare = false;

            emptySquaresLookup =std::string(chess_set);

            //while isSafeSquare is false
            while(!isSafeSquare)
            {
                // random index
                int rand_index = rand()%64;

                //if not empty start new cycle
                if (chess_set[rand_index] != '-')
                {
                    continue;
                }

                //test each black piece
                for (int w_p = 0; w_p < 6; w_p++)
                {
                    std::string attackedPieces = attackSquares(emptySquaresLookup,
                                                               rand_index % 8,
                                                               rand_index / 8,
                                                               Constants::CP_LABELS_WHITE[w_p]);
                    //for cell in attack board
                    for (char piece : attackedPieces)
                    {
                        //if white piece is oposite of current black piece
                        if (piece == Constants::CP_LABELS_BLACK[w_p])
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
                    chess_set[rand_index] = 'K';
                }
            }

            //End simulation - caclulate simulation duration in ns
            m_timer.markEnd();
            m_timer.setDurationInNanoseconds();
            m_timer.updateStats();

            /*Parsing the randomized custom chess board description to FEN notation
            More info here:
            https://www.chess.com/terms/fen-chess */

            //char array to hold the pieces including separators in FEN Format
            char FEN[71] = {'0',};
            parseFEN(chess_set, FEN);

            //update external variables
            custDescription = chess_set;
            fenDescription = FEN;

            //converting chess_set and FEN to string and push to queue
            m_current_CB_Description = {std::string(chess_set), std::string(FEN), std::to_string(m_timer.getSimulationTime())};

            m_CB_Descriptions.push(m_current_CB_Description);

            //if queue exceeds 20 pop one out
            if(m_CB_Descriptions.size()==21)
            {
                m_CB_Descriptions.pop();
            }

            //terminate while loop
            rep_sim = false;
        }
    }
    else
    { //if shuff = false, display last board descriptions in queue
        if(!isViewing()){
            custDescription = m_CB_Descriptions.back().Custom;
            fenDescription = m_CB_Descriptions.back().FEN;
        }
        else
        {
            setBoardDescriptionFromVector();
            custDescription = getCurrentDescription().Custom;
            fenDescription = getCurrentDescription().FEN;
        }

    }
}

/*
Parse custom chess board description to FEN notation
Not Important but fun to have. Click on the notation in the window to copy it in the clipboard
Takes char array pointers to the custom chess board description and
char array with 71 chars for the notation (the longest FEN notation is 71 characters).
Both arrays must be already initialized outside the function.
*/
void ChessBoard::parseFEN(const char chess_set[65], char FEN[71])
{
    //char array to hold the positions including separators '/' (64+7)
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
}

void ChessBoard::setBoardDescriptionFromQueueBack()
{
    m_current_CB_Description.Custom = m_CB_Descriptions.back().Custom;
}

void ChessBoard::setBoardDescriptionFromVector()
{
    int idx = m_CB_Descriptions_Vec_Seek % m_CB_Descriptions_Vec.size();
    m_current_CB_Description.Custom = m_CB_Descriptions_Vec.at(idx).Custom;
    m_current_CB_Description.FEN = m_CB_Descriptions_Vec.at(idx).FEN;
    m_current_CB_Description.simulationTime = m_CB_Descriptions_Vec.at(idx).simulationTime;
}

int& ChessBoard::getMutable_CB_Descriptions_Vec_Seek()
{
    return m_CB_Descriptions_Vec_Seek;
}

std::queue<ChessBoardDescriptions>& ChessBoard::getMutableDescriptionsQueue()
{
    return m_CB_Descriptions;
}

std::vector<ChessBoardDescriptions>& ChessBoard::getMutableDescriptionsVector()
{
    return m_CB_Descriptions_Vec;
}

ChessBoardDescriptions ChessBoard::getCurrentDescription() const
{
    return m_current_CB_Description;
}

void ChessBoard::setChessPieceIdx(int idx){
    m_chessPieceIdx = idx;
}

SDL_Rect* ChessBoard::getChessBoardSquareRect(int idx) const
{
    return m_chessBoardSquare[idx];
}

SDL_Rect* ChessBoard::getTextFENRect()
{
    return &m_textFENRect;
}

SDL_Rect* ChessBoard::getbuttonViewerRect()
{
    return &m_buttonViewerRect;
}

SDL_Rect* ChessBoard::getbuttonSimulationRect()
{
    return &m_buttonSimulationRect;
}

SDL_Rect* ChessBoard::getWindowRect()
{
    return &m_windowRect;
}

std::string ChessBoard::getSimulationSummary() const
{
    if(!isViewing())
    {
    return m_timer.simulationTimeStatistics();
    }
    else
    {
        std::string file_stat {};
        file_stat.append("Record ");
        file_stat.append(std::to_string(m_CB_Descriptions_Vec_Seek + 1));
        file_stat.append(" of ");
        file_stat.append(std::to_string(m_CB_Descriptions_Vec.size()));
        file_stat.append("\n");
        file_stat.append("Simulation Time: ");
        file_stat.append(m_current_CB_Description.simulationTime);
        file_stat.append("\n\n\n");

        return file_stat;
    }
}

void ChessBoard::resetSimulationSummary()
{
    m_timer.reset();
}

void ChessBoard::initBoard()
{
    for (int i = 0; i < 64; i++)
    {
        m_chessBoardSquare[i]->x = (i % 8)*(m_chessBoardSize / 8);
        m_chessBoardSquare[i]->y = (i / 8)*(m_chessBoardSize / 8);
        m_chessBoardSquare[i]->w = m_chessBoardSquare[i]->h = m_chessBoardSize / 8;
    }

    int square_size = m_chessBoardSize / 8;
    int label_size = square_size /4;

    for(int i = 0; i <8 ; i++)
    {
        //Labels
        m_chessBoardLabelsV[i]->x = 0;
        m_chessBoardLabelsV[i]->y = (i * square_size);
        m_chessBoardLabelsV[i]->w = m_chessBoardLabelsV[i]->h = label_size;

        m_chessBoardLabelsH[i]->x = (i * square_size) + (square_size - label_size);
        m_chessBoardLabelsH[i]->y = (square_size * 7) + (square_size - label_size);
        m_chessBoardLabelsH[i]->w = m_chessBoardLabelsH[i]->h = label_size;
    }
}

void ChessBoard::drawTitle()
{
    // Title
    if(isViewing())
        TextureFactory::instance()->drawTexture("textTitleViewer", NULL, &m_textTitleRect);
    else
        TextureFactory::instance()->drawTexture("textTitleGenerator", NULL, &m_textTitleRect);

}

void ChessBoard::drawModeToggleButtons()
{
    //NEW - Implement Start/Stop Simulation toggle button
    if (!isViewing())
    {
        if (isSimulating())
        {
            TextureFactory::instance()->drawTexture("button_stop_up", NULL, &m_buttonSimulationRect);
            TextureFactory::instance()->drawTexture("button_viewer_disabled", NULL, &m_buttonViewerRect);
        }
        else
        {
            TextureFactory::instance()->drawTexture("button_start_up", NULL, &m_buttonSimulationRect);
            TextureFactory::instance()->drawTexture("button_viewer_up", NULL, &m_buttonViewerRect);
        }
    }
    else
    {
        TextureFactory::instance()->drawTexture("button_simulator_up", NULL, &m_buttonViewerRect);
    }

}

void ChessBoard::drawWindowBackground(){
    //Window BG Texture
    TextureFactory::instance()->drawTexture("background", NULL, &m_windowRect);
}

void ChessBoard::drawBoard()
{
    SDL_SetRenderDrawBlendMode(TextureFactory::instance()->getRenderer(), SDL_BLENDMODE_BLEND);
    for (int i = 0; i < 64; i++)
    {
		SDL_SetRenderDrawColor(
            TextureFactory::instance()->getRenderer(),
            m_chessBoardColor[(i + ( i / 8 ) % 2 ) %2 ].r,
            m_chessBoardColor[(i + ( i / 8 ) % 2 ) %2].g,
            m_chessBoardColor[(i + ( i / 8 ) % 2 ) %2].b,
            m_chessBoardColor[(i + ( i / 8 ) % 2 ) %2].a
            );

        SDL_RenderFillRect(TextureFactory::instance()->getRenderer(),
                           m_chessBoardSquare[i]);
        srand (i);
        m_srcChessTileRect->x = rand()%(640-64);
        m_srcChessTileRect->y = rand()%(640-64);
        TextureFactory::instance()->setTextureAlpha("chess_tile", 55);
        TextureFactory::instance()->drawTexture("chess_tile", m_srcChessTileRect, m_chessBoardSquare[i]);
	}

    // Draw label textures for the board

    SDL_SetRenderDrawBlendMode(TextureFactory::instance()->getRenderer(),
                               SDL_BLENDMODE_BLEND);

    for(int i = 0; i < 8 ; i++)
    {
        TextureFactory::instance()->drawTexture(std::string(1, ('a' + i)),
                                                NULL, m_chessBoardLabelsH[i]);

        TextureFactory::instance()->drawTexture(std::string(1, ('8' - i)),
                                                NULL, m_chessBoardLabelsV[i]);
    }
}

void ChessBoard::drawFENDescription()
{
    std::string dynamic_fen {};
    if (!isViewing())
    {
        dynamic_fen = getMutableDescriptionsQueue().back().FEN;
    }
    else
    {
        setBoardDescriptionFromVector();
        dynamic_fen = getCurrentDescription().FEN;
    }
    TextureFactory::instance()->textureFromFont("textInfoTexture",
                                                "Segoe28",
                                                dynamic_fen.c_str(),
                                                Constants::COL_TXT_LIGHT,
                                                1280, 0);

    TextureFactory::instance()->drawTexture("textInfoTexture",
                                            NULL, &m_textFENRect);

    TextureFactory::instance()->destroyTexture("textInfoTexture");

}

void ChessBoard::drawStatistics()
{
    // Statistics for the simulation time
    std::string dynamic_text_Times = getSimulationSummary();

    TextureFactory::instance()->textureFromFont("textTimeTexture",
                                                "Segoe28",
                                                dynamic_text_Times.c_str(),
                                                Constants::COL_TXT_LIGHT, 640, 0);

    TextureFactory::instance()->drawTexture("textTimeTexture",
                                            NULL, &m_textTimeRect);

    TextureFactory::instance()->destroyTexture("textTimeTexture");
}

void ChessBoard::drawBoardOverlay()
{
	//bool showOverlay = true;
    if (!m_simulating && m_chessPieceIdx > -1)
    {
        int x = m_chessPieceIdx % 8;
        int y = m_chessPieceIdx / 8;

        std::string overlay = attackSquares(m_current_CB_Description.Custom, x, y, '\0' );

        for (int i = 0; i < 64; i++)
        {
            SDL_SetRenderDrawBlendMode(TextureFactory::instance()->getRenderer(),
                                       SDL_BLENDMODE_BLEND);

            //hide unused rectangles with 0 alpha value
            SDL_SetRenderDrawColor(TextureFactory::instance()->getRenderer(),
                                   m_chessBoardColor[2].r,
                                   m_chessBoardColor[2].g,
                                   m_chessBoardColor[2].b,
                                   ((overlay[i]!='-') ? 100 : 0));

            SDL_RenderFillRect(TextureFactory::instance()->getRenderer(),
                               m_chessBoardSquare[i]);
		}

    } else
    {
		//Board descriptions is empty - no overlay
        m_current_CB_Description.Custom = "----------------------------------------------------------------";
	}
}

void ChessBoard::drawPieces()
{
    //declare string descriptions
	std::string chessBoardShuffle; 
	std::string fenChessBoard;

	//init string descriptions
	shufflePieces(isSimulating(), chessBoardShuffle, fenChessBoard);

    for (int i = 0; i < 64; i++)
    {
        if (chessBoardShuffle[i] == '-')
        {
			continue;
		}
        for (int j = 0; j < 12; j++)
        {
            if (chessBoardShuffle[i] == Constants::STR_CB_LOOKUPREF[j])
            {
                SDL_RenderCopy(TextureFactory::instance()->getRenderer(),
                               m_chessPieces[j],
                               NULL,
                               m_chessBoardSquare[i]);
				continue;
			}
		}
	}
}


