//ChessBoard.cpp

#include <algorithm>
#include <random>
#include "ChessBoard.h"
#include "Constants.h"
#include "Utilities.h"
#include "TextureFactory.h"
#include <iostream>

using namespace Constants;

ChessBoard::ChessBoard()
{
    m_simulating = false;
    m_viewing = false;

    //Posible UI feature
    setPiecesToRemove(DIM_CP_TO_REMOVE);

    //Chess Board Size and Colors
    m_chessBoardSize = DIM_CB_SIZE;
    m_ColorChessBoard[0] = COL_CB_LIGHT;
    m_ColorChessBoard[1] = COL_CB_DARK;
    m_ColorChessBoard[2] = COL_CB_HIGHLIGHT;

    //Generate and place various chess board rectangles
    initBoardRects();

    //Set up UI Rectangles

    m_RectTextFEN = {DIM_PADDING / 2, DIM_WINDOW_WIDTH / 2 + 10, DIM_WINDOW_WIDTH / 2 - DIM_PADDING, DIM_CB_TILE_SIZE - DIM_PADDING / 2};
    m_RectTextStats = {DIM_WINDOW_WIDTH / 2 + DIM_PADDING, DIM_CB_TILE_SIZE + DIM_PADDING , DIM_WINDOW_WIDTH / 2 - 2*DIM_PADDING, DIM_WINDOW_HEIGHT / 3};
    m_RectButtonSimulator = {DIM_WINDOW_WIDTH / 2 + DIM_PADDING, DIM_WINDOW_WIDTH / 2 - DIM_CB_TILE_SIZE, (DIM_WINDOW_WIDTH / 2 - 3*DIM_PADDING)/2, DIM_CB_TILE_SIZE};
    m_RectButtonViewer = {DIM_WINDOW_WIDTH / 2 + 2*DIM_PADDING + (DIM_WINDOW_WIDTH / 2 - 3*DIM_PADDING)/2, DIM_WINDOW_WIDTH / 2 - DIM_CB_TILE_SIZE, (DIM_WINDOW_WIDTH / 2 - 3*DIM_PADDING)/2, DIM_CB_TILE_SIZE};
    m_RectTextTitle = {DIM_WINDOW_WIDTH / 2 + DIM_PADDING, DIM_PADDING / 2, DIM_WINDOW_WIDTH / 2 - 2*DIM_PADDING, DIM_CB_TILE_SIZE};
    m_RectWindow = {0, 0, DIM_WINDOW_WIDTH, DIM_WINDOW_HEIGHT};
    m_Rect_SliderSlit = {DIM_WINDOW_WIDTH / 2 + DIM_PADDING, DIM_WINDOW_WIDTH / 2 - (5 * DIM_CB_TILE_SIZE / 2), DIM_WINDOW_WIDTH / 2 - 2*DIM_PADDING, DIM_CB_TILE_SIZE};
    m_Rect_SliderKnob = {m_Rect_SliderSlit.x + (DIM_CP_TO_REMOVE - 3) *((m_Rect_SliderSlit.w)/ 33), m_Rect_SliderSlit.y, m_Rect_SliderSlit.h, m_Rect_SliderSlit.h};


    setButtonViewerTexID(ID_BTN_VIEWER_UP);
    setButtonSimulatorTexID(ID_BTN_SIMULATOR_UP);
    setButtonStartTexID(ID_BTN_START_UP);
    setButtonStopTexID(ID_BTN_STOP_UP);

    m_chessPieceIdx = -1;

    m_current_CB_Description = {STR_CB_INIT_DESCR, STR_CB_INIT_FEN, "0.0"};
    m_CB_Descriptions.push(m_current_CB_Description);
}

ChessBoard::~ChessBoard()
{
    // delete chess board rectangles
    for (int i = 0; i < 64; i++)
    {
        delete m_RectPtrChessBoardTile[i];
    }

    // delete chess board labels
    for (int i = 0; i < 8; i++)
    {
        delete m_RectPtrBoardLabelsV[i];
        delete m_RectPtrBoardLabelsH[i];
    }
    delete m_RectPtrFloatingChessTile;

}

void ChessBoard::prepFonts()
{
    // Loading fonts
    TextureFactory::instance()->loadFont(TTF_DEJAVUSANS, ID_FONT_DEJAVU, 48);
    TextureFactory::instance()->loadFont(TTF_SEGOEPR, ID_FONT_SEGOE, 72);
    TextureFactory::instance()->loadFont(TTF_SEGOEPR, ID_FONT_SEGOE28, 28);
}

void ChessBoard::prepStaticFontTextures()
{
    // Title textures
    TextureFactory::instance()->textureFromFont(ID_TXT_TITLE_SIMULATOR,ID_FONT_SEGOE,
                                                "Chess Board Simulator",
                                                COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH, 0);
    TextureFactory::instance()->textureFromFont(ID_TXT_TITLE_VIEWER,ID_FONT_SEGOE,
                                                "Chess Board Viewer",
                                                COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH, 0);
}

void ChessBoard::prepStaticImageTextures()
{
    // Loading image textures
    TextureFactory::instance()->textureFromImage(IMG_BACKGROUND, ID_BACKGROUND);
    TextureFactory::instance()->textureFromImage(IMG_CHESS_TILE, ID_CHESS_TILE);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_START_UP, ID_BTN_START_UP);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_START_DOWN, ID_BTN_START_DOWN);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_STOP_UP, ID_BTN_STOP_UP);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_STOP_DOWN, ID_BTN_STOP_DOWN);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_VIEWER_UP, ID_BTN_VIEWER_UP);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_VIEWER_DOWN, ID_BTN_VIEWER_DOWN);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_VIEWER_DISABLED, ID_BTN_VIEWER_DISABLED);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_SIMULATOR_UP, ID_BTN_SIMULATOR_UP);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_SIMULATOR_DOWN, ID_BTN_SIMULATOR_DOWN);
    TextureFactory::instance()->textureFromImage(IMG_SLIDER_SLIT, ID_SLIDER_SLIT);
    TextureFactory::instance()->textureFromImage(IMG_SLIDER_KNOB, ID_SLIDER_KNOB);
}

void ChessBoard::prepChessPieceTextures()
{
	// Create textures from font chess characters - only the black pieces (6)
    TTF_Font* DejaVu = TextureFactory::instance()->getFont(ID_FONT_DEJAVU);

    SDL_Surface* tempSurfaceText = NULL;

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

void ChessBoard::prepBoardLabelsTextures()
{
    for(int i = 0; i < 8; i++)
    {
        std::string h_label = std::string(1, ('a' + i));
        TextureFactory::instance()->textureFromFont(h_label,
                                                    ID_FONT_DEJAVU,
                                                    h_label.c_str(),
                                                    (i%2)?COL_CB_DARK : COL_CB_LIGHT,
                                                    64, 32);

        std::string v_label = std::string(1, ('8' - i));
        TextureFactory::instance()->textureFromFont(v_label,
                                                    ID_FONT_DEJAVU,
                                                    v_label.c_str(),
                                                    (i%2)?COL_CB_LIGHT : COL_CB_DARK,
                                                    64, 32);
    }
}

void ChessBoard::setButtonViewerTexID(std::string texture_id)
{
    m_ButtonViewerTexID = texture_id;
}

void ChessBoard::setButtonSimulatorTexID(std::string texture_id)
{
    m_ButtonSimulatorTexID = texture_id;
}

void ChessBoard::setButtonStartTexID(std::string texture_id)
{
    m_ButtonStartTexID = texture_id;
}

void ChessBoard::setButtonStopTexID(std::string texture_id)
{
    m_buttonStopTexID = texture_id;
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
    if (amount < 0 || amount > 29)
    {
        m_piecesToRemove = 8; //default
    } else
    {
        m_piecesToRemove = amount;
    }
}

int ChessBoard::getPiecesToRemove()
{
    return m_piecesToRemove;
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
Click on the notation in the window to copy it in the clipboard
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

SDL_Rect* ChessBoard::getRectChessBoardTile(int idx) const
{
    return m_RectPtrChessBoardTile[idx];
}

SDL_Rect* ChessBoard::getRectTextFEN()
{
    return &m_RectTextFEN;
}

SDL_Rect* ChessBoard::getRectButtonViewer()
{
    return &m_RectButtonViewer;
}

SDL_Rect* ChessBoard::getRectButtonSimulator()
{
    return &m_RectButtonSimulator;
}

SDL_Rect* ChessBoard::getRectSliderKnob()
{
    return &m_Rect_SliderKnob;
}

SDL_Rect* ChessBoard::getRectWindow()
{
    return &m_RectWindow;
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

void ChessBoard::initBoardRects()
{
    //init squares rects
    for (int i = 0; i < 64; i++)
    {
        m_RectPtrChessBoardTile[i] = new SDL_Rect{0, 0, 0, 0};
    }
    //init label rects
    for (int i = 0; i < 8; i++)
    {
        m_RectPtrBoardLabelsV[i] = new SDL_Rect{0, 0, 0, 0};
        m_RectPtrBoardLabelsH[i] = new SDL_Rect{0, 0, 0, 0};
    }
    //place square rects
    for (int i = 0; i < 64; i++)
    {
        m_RectPtrChessBoardTile[i]->x = (i % 8)*(m_chessBoardSize / 8);
        m_RectPtrChessBoardTile[i]->y = (i / 8)*(m_chessBoardSize / 8);
        m_RectPtrChessBoardTile[i]->w = m_RectPtrChessBoardTile[i]->h = m_chessBoardSize / 8;
    }
    //place label rects
    int square_size = m_chessBoardSize / 8;
    int label_size = square_size /4;
    for(int i = 0; i <8 ; i++)
    {
        //Labels
        m_RectPtrBoardLabelsV[i]->x = 0;
        m_RectPtrBoardLabelsV[i]->y = (i * square_size);
        m_RectPtrBoardLabelsV[i]->w = m_RectPtrBoardLabelsV[i]->h = label_size;

        m_RectPtrBoardLabelsH[i]->x = (i * square_size) + (square_size - label_size);
        m_RectPtrBoardLabelsH[i]->y = (square_size * 7) + (square_size - label_size);
        m_RectPtrBoardLabelsH[i]->w = m_RectPtrBoardLabelsH[i]->h = label_size;
    }

    //init random board tile texture rect
    m_RectPtrFloatingChessTile = new SDL_Rect{0, 0, 64, 64};
}

void ChessBoard::drawTitle()
{
    // Title
    if(isViewing())
        TextureFactory::instance()->drawTexture(ID_TXT_TITLE_VIEWER, NULL, &m_RectTextTitle);
    else
        TextureFactory::instance()->drawTexture(ID_TXT_TITLE_SIMULATOR, NULL, &m_RectTextTitle);

}

void ChessBoard::drawModeToggleButtons()
{
    if (!isViewing())
    {
        if (isSimulating())
        {
            TextureFactory::instance()->drawTexture(m_buttonStopTexID, NULL, &m_RectButtonSimulator);
            TextureFactory::instance()->drawTexture(ID_BTN_VIEWER_DISABLED, NULL, &m_RectButtonViewer);
        }
        else
        {
            TextureFactory::instance()->drawTexture(m_ButtonStartTexID, NULL, &m_RectButtonSimulator);
            TextureFactory::instance()->drawTexture(m_ButtonViewerTexID, NULL, &m_RectButtonViewer);
        }
    }
    else
    {
        TextureFactory::instance()->drawTexture(m_ButtonSimulatorTexID, NULL, &m_RectButtonViewer);
    }

}

void ChessBoard::drawWindowBackground(){
    //Window BG Texture
    TextureFactory::instance()->drawTexture(ID_BACKGROUND, NULL, &m_RectWindow);
}

void ChessBoard::drawBoard()
{
    SDL_SetRenderDrawBlendMode(TextureFactory::instance()->getRenderer(), SDL_BLENDMODE_BLEND);
    for (int i = 0; i < 64; i++)
    {
		SDL_SetRenderDrawColor(
            TextureFactory::instance()->getRenderer(),
            m_ColorChessBoard[(i + ( i / 8 ) % 2 ) %2 ].r,
            m_ColorChessBoard[(i + ( i / 8 ) % 2 ) %2].g,
            m_ColorChessBoard[(i + ( i / 8 ) % 2 ) %2].b,
            m_ColorChessBoard[(i + ( i / 8 ) % 2 ) %2].a
            );

        SDL_RenderFillRect(TextureFactory::instance()->getRenderer(),
                           m_RectPtrChessBoardTile[i]);
        srand (i);
        m_RectPtrFloatingChessTile->x = rand()%(640-64);
        m_RectPtrFloatingChessTile->y = rand()%(640-64);
        TextureFactory::instance()->setTextureAlpha(ID_CHESS_TILE, 55);
        TextureFactory::instance()->drawTexture(ID_CHESS_TILE, m_RectPtrFloatingChessTile, m_RectPtrChessBoardTile[i]);
	}

    // Draw label textures for the board

    SDL_SetRenderDrawBlendMode(TextureFactory::instance()->getRenderer(),
                               SDL_BLENDMODE_BLEND);

    for(int i = 0; i < 8 ; i++)
    {
        TextureFactory::instance()->drawTexture(std::string(1, ('a' + i)),
                                                NULL, m_RectPtrBoardLabelsH[i]);

        TextureFactory::instance()->drawTexture(std::string(1, ('8' - i)),
                                                NULL, m_RectPtrBoardLabelsV[i]);
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
    TextureFactory::instance()->textureFromFont(ID_TXT_FEN,
                                                ID_FONT_SEGOE28,
                                                dynamic_fen.c_str(),
                                                Constants::COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH, 0);

    TextureFactory::instance()->drawTexture(ID_TXT_FEN,
                                            NULL, &m_RectTextFEN);

    TextureFactory::instance()->destroyTexture(ID_TXT_FEN);

}

void ChessBoard::drawStatistics()
{
    // Statistics for the simulation time
    std::string dynamic_text = getSimulationSummary();

    TextureFactory::instance()->textureFromFont(ID_TXT_STATS,
                                                ID_FONT_SEGOE28,
                                                dynamic_text.c_str(),
                                                Constants::COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH / 2, 0);

    TextureFactory::instance()->drawTexture(ID_TXT_STATS,
                                            NULL, &m_RectTextStats);

    TextureFactory::instance()->destroyTexture(ID_TXT_STATS);
}

void ChessBoard::drawSlider(const int& offsetX)
{
    TextureFactory::instance()->drawTexture(ID_SLIDER_SLIT, NULL, &m_Rect_SliderSlit);
    TextureFactory::instance()->drawTexture(ID_SLIDER_KNOB, NULL, &m_Rect_SliderKnob);
    if (offsetX >= 0)
    {
        int msx, msy;
        SDL_GetMouseState(&msx, &msy);
        m_Rect_SliderKnob.x = msx - offsetX;
        if (m_Rect_SliderKnob.x < m_Rect_SliderSlit.x)
            m_Rect_SliderKnob.x = m_Rect_SliderSlit.x;
        if (m_Rect_SliderKnob.x > m_Rect_SliderSlit.x + (m_Rect_SliderSlit.w - m_Rect_SliderKnob.w))
            m_Rect_SliderKnob.x = m_Rect_SliderSlit.x + (m_Rect_SliderSlit.w - m_Rect_SliderKnob.w);
    }
    int  val = (
                (
                    (m_Rect_SliderKnob.x - m_Rect_SliderSlit.x)/
                    ((m_Rect_SliderSlit.w)/ 33)
                ) % 34
               )+3;

    (val > 32)? val = 32 : val;

    setPiecesToRemove(32-val);

    SDL_Rect label = m_Rect_SliderSlit;
    label.h /= 2;
    label.y = m_Rect_SliderSlit.y - label.h;
    label.w /= 2;

    std::string label_text = "Number of chess pieces: ";

    (val < 10)? label_text += " " : label_text;

    label_text.append(std::to_string(val));

    TextureFactory::instance()->textureFromFont(
        "label", Constants::ID_FONT_SEGOE,
        label_text.c_str(),
        Constants::COL_TXT_LIGHT, 1280, 0);
    TextureFactory::instance()->drawTexture("label", NULL, &label);
    TextureFactory::instance()->destroyTexture("label");
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
                                   m_ColorChessBoard[2].r,
                                   m_ColorChessBoard[2].g,
                                   m_ColorChessBoard[2].b,
                                   ((overlay[i]!='-') ? 100 : 0));

            SDL_RenderFillRect(TextureFactory::instance()->getRenderer(),
                               m_RectPtrChessBoardTile[i]);
		}

    } else
    {
		//Board descriptions is empty - no overlay
        m_current_CB_Description.Custom = "----------------------------------------------------------------";
	}
}

void ChessBoard::drawPieces()
{
    std::string chessBoardShuffle {};
    std::string fenChessBoard {};

    //init descriptions
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
                               m_RectPtrChessBoardTile[i]);
				continue;
			}
		}
	}
}


