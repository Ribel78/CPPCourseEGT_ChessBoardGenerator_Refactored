#include "ChessBoard.h"
#include "Constants.h"

ChessBoard::ChessBoard()
{
    //this->renderer = renderer;
    //Posible UI feature
    setPiecesToRemove(8);

    //simulate chessboard
    ChessBoard::m_simulating = false;

	//Chess Board Size and Color
    m_chessBoardSize = Constants::CB_SIZE;
    m_chessBoardColor[0] = Constants::CB_LIGHT; //"light" square
    m_chessBoardColor[1] = Constants::CB_DARK; //"dark" square
    m_chessBoardColor[2] = Constants::CB_HIGHLIGHT; // highlight color

    for (int i = 0; i < 64; i++)
    {
        m_chessBoardSquare[i] = new SDL_Rect{0, 0, 0, 0};
    }

    for (int i = 0; i < 8; i++)
    {
        m_chessBoardLabelsV[i] = new SDL_Rect{0, 0, 0, 0};
        m_chessBoardLabelsH[i] = new SDL_Rect{0, 0, 0, 0};
    }

    m_chessPieceIdx = -1;

    m_queueCustomDescription.push(Constants::CB_INIT_DESCR);

    m_queueFENDescription.push(Constants::CB_INIT_FEN);

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

            tempSurfaceText = TTF_RenderUTF8_Blended(DejaVu, CPB_UNICODE[i % 6].c_str(), CP_LIGHT);
            m_chessPieces[i] = SDL_CreateTextureFromSurface(TextureFactory::instance()->getRenderer(),
                                                            tempSurfaceText);
        }
        else
        { // black chess glyphs - k, q, r, b, n, p
            tempSurfaceText = TTF_RenderUTF8_Blended(DejaVu, CPB_UNICODE[i % 6].c_str(), CP_DARK);
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

    char chess_set[] = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";
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
                //if Bishops of a kind on same color square - reshuffle
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

            //converting chess_set to string and push to queue
            std::string temp(chess_set);

            custDescription = temp;

            m_queueCustomDescription.push(custDescription);
            temp = std::string(FEN);

            fenDescription = temp;

            m_queueFENDescription.push(fenDescription);
            //if queue exceeds 20 pop one out
            if(m_queueCustomDescription.size()==21)
            {
                m_queueCustomDescription.pop();
                m_queueFENDescription.pop();
            }

            //terminate while loop
            rep_sim = false;
        }
    }
    else
    { //if shuff = false, display last board descriptions in queue
        custDescription = m_queueCustomDescription.back();
        fenDescription = m_queueFENDescription.back();
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
    m_boardDescription = m_queueCustomDescription.back();
}

std::queue<std::string>& ChessBoard::getMutableCustomDescriptionQueue()
{
    return m_queueCustomDescription;
}

std::queue<std::string>& ChessBoard::getMutableFENDescriptionQueue()
{
    return m_queueFENDescription;
}

void ChessBoard::setChessPieceIdx(int idx){
    m_chessPieceIdx = idx;
}

SDL_Rect* ChessBoard::getChessBoardSquareRect(int idx) const
{
    return m_chessBoardSquare[idx];
}

std::string ChessBoard::getSimulationSummary() const
{
    return m_timer.simulationTimeToString();
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

void ChessBoard::drawBoard()
{
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

void ChessBoard::drawBoardOverlay()
{
	//bool showOverlay = true;
    if (!m_simulating && m_chessPieceIdx > -1)
    {
        int x = m_chessPieceIdx % 8;
        int y = m_chessPieceIdx / 8;

        std::string overlay = attackSquares(m_boardDescription, x, y, '\0' );

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
        m_boardDescription = "----------------------------------------------------------------";
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
            if (chessBoardShuffle[i] == Constants::CB_LOOKUPREF[j])
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
