//Game.cpp

#include <iostream>
#include "Game.h"
#include "TextureFactory.h"
#include "Constants.h"

Game::Game()
{
    Game::m_window = NULL;
    Game::m_running = true;
}

Game::~Game()
{
    std::cout << "Destructing Game Object." << std::endl;
}

//Initialize SDL library
bool Game::init(const char* title,
                int xpos, int ypos,
                int width, int height,
                int flags)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
		std::cout << "SDL init success\n";

        m_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        //window init success
        if (m_window != 0)
		{
			std::cout << "window creation success\n";
            SDL_Renderer* renderer = SDL_CreateRenderer(m_window, -1, 0);
            //renderer init success
            if (renderer != 0)
			{
                TextureFactory::instance()->setRenderer(renderer);

				std::cout << "renderer creation success\n";
			}
            else
            {
				std::cout << "renderer init failed\n";
				return false;
			}
		}
        else
        {
			std::cout << "window init failed\n";
			return false;
		}
	}
    else
    {
		std::cout << "SDL init fail\n";
		return false;
	}

	//Init TTF library
    if(TTF_Init() == -1)
    {
		return false;
    }

    std::cout << "init success\n";
    m_running = true;
    m_chessBoard.setSimulating(false);
    m_chessBoard.initBoard();

    prepTextures();

	return true;
}

// Prepare textures for rendering
void Game::prepTextures()
{
    using namespace Constants;
    // Loading fonts and storing them into map as pointer variables
    TextureFactory::instance()->loadFont(TTF_DEJAVUSANS,"DejaVu", 48);
    TextureFactory::instance()->loadFont(TTF_SEGOEPR,"Segoe", 72);
    TextureFactory::instance()->loadFont(TTF_SEGOEPR,"Segoe28", 28);

    // Title texture
    TextureFactory::instance()->textureFromFont("textTitleTexture","Segoe",
                                                "Chess Board Generator",
                                                COL_TXT_DARK,
                                                1280, 0);
    // Button Start Simulation
    TextureFactory::instance()->textureFromFont("buttonStartTex","DejaVu",
                                                "     Start\n Simulation",
                                                COL_TXT_LIGHT,
                                                300, 0);
    // Button Stop Simulation
    TextureFactory::instance()->textureFromFont("buttonStopTex","DejaVu",
                                                "     Stop\n Simulation",
                                                COL_TXT_LIGHT,
                                                300, 0);

    // Parametrizing the layout of the destination rectangles
	int ww, wh;
    SDL_GetWindowSize(m_window, &ww, &wh);
	int padding = 40;
	int chess_sq = (ww / 2) / 8;

    m_textTitleRect = {ww / 2 + padding, padding / 2, ww / 2 - 2*padding, chess_sq};
    m_buttonStartRect = {ww / 2 + padding, ww / 2 - chess_sq, (ww / 2 - 3*padding)/2, chess_sq};
    m_buttonStopRect = {ww / 2 + 2*padding + (ww / 2 - 3*padding)/2, ww / 2 - chess_sq, (ww / 2 - 3*padding)/2, chess_sq};
    m_textFENRect = {padding / 2, ww / 2 + 10, ww / 2 - padding, chess_sq - padding / 2};
    m_textTimeRect = {ww / 2 + padding, chess_sq * 2 , ww / 2 - 2*padding, wh / 3};

    m_chessBoard.prepChessPieceTextures();

    // Add label textures for the board

    for(int i = 0; i < 8; i++)
    {
        std::string h_label = std::string(1, ('a' + i));
        TextureFactory::instance()->textureFromFont(h_label,
                                                    "DejaVu",
                                                    h_label.c_str(),
                                                    (i%2)?COL_CB_DARK : COL_CB_LIGHT,
                                                    64, 32);

        std::string v_label = std::string(1, ('8' - i));
        TextureFactory::instance()->textureFromFont(v_label,
                                                    "DejaVu",
                                                    v_label.c_str(),
                                                    (i%2)?COL_CB_LIGHT : COL_CB_DARK,
                                                    64, 32);
    }

    //Test IMG texture
    TextureFactory::instance()->textureFromImage("images/a.png", "test");

}

void Game::update()
{
    //Render All
    SDL_RenderPresent(TextureFactory::instance()->getRenderer());

    //Slow down visual shuffling
    SDL_Delay(6);
}

void Game::handleEvents()
{
	SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: m_running = false; break;
        /*
         * store last 20 generated chessboard descriptions - in queues.
         * seek trough the simulations using Down Arrow Key
        */
        case SDL_KEYUP:{
            if(event.key.keysym.sym == SDLK_DOWN)
            {
                if(!m_chessBoard.isSimulating())
                {
                    m_chessBoard.setChessPieceIdx(-1);
					std::string temp;
                    temp = m_chessBoard.getMutableFENDescriptionQueue().front();
                    m_chessBoard.getMutableFENDescriptionQueue().pop();
                    m_chessBoard.getMutableFENDescriptionQueue().push(temp);
                    temp = m_chessBoard.getMutableCustomDescriptionQueue().front();
                    m_chessBoard.getMutableCustomDescriptionQueue().pop();
                    m_chessBoard.getMutableCustomDescriptionQueue().push(temp);
                    m_chessBoard.setBoardDescriptionFromQueueBack();
				}
			}
		}; break;
        case SDL_MOUSEBUTTONDOWN:
        {
			int msx, msy;
            if (event.button.button == SDL_BUTTON_LEFT)
            {
				SDL_GetMouseState(&msx, &msy);
                m_mouseDownX = msx;
                m_mouseDownY = msy;
			}
		}; break;
        case SDL_MOUSEBUTTONUP:
        {
			int msx, msy;
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
				SDL_GetMouseState(&msx, &msy);
			}
			//int msx, msy;
            if (event.button.button == SDL_BUTTON_LEFT)
            {
				SDL_GetMouseState(&msx, &msy);
                if(buttonClicked(&m_buttonStartRect,
                                  m_mouseDownX,m_mouseDownY,
                                  msx, msy))
                {
                    m_chessBoard.setSimulating(true);
				}
                if(buttonClicked(&m_buttonStopRect,
                                  m_mouseDownX,m_mouseDownY,
                                  msx, msy))
                {
                    m_chessBoard.setSimulating(false);
				}
                if(buttonClicked(&m_textFENRect,
                                  m_mouseDownX,m_mouseDownY,
                                  msx, msy) &&
                    !m_chessBoard.isSimulating())
                { // Copy FEN code to clipboard
                    SDL_SetClipboardText(m_chessBoard.getMutableFENDescriptionQueue().back().c_str());
				}	
                for (int i = 0; i < 64; i++)
                {
                    if(buttonClicked(m_chessBoard.getChessBoardSquareRect(i),
                                      m_mouseDownX, m_mouseDownY,
                                      msx, msy) &&
                        !m_chessBoard.isSimulating())
                    {
                        m_chessBoard.setChessPieceIdx(i);
						//chessPieceIdx = i;
                        m_chessBoard.setBoardDescriptionFromQueueBack();
						break;
					}	
				}		
			}
		}; break;
		default: break;
		}
	}
}

void Game::clean()
{
    std::cout << "cleaning game\n";
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(TextureFactory::instance()->getRenderer());
    SDL_Quit();
}

bool Game::isRunning() const
{
    return Game::m_running;
}

/*
Checks positions of the mousedown / mouseup separately
and compare against
given SDL_Rect coordinates and dimensions
*/
bool Game::buttonClicked(const SDL_Rect* r,
                         int xDown, int yDown,
                         int xUp, int yUp) const
{
    if(((xDown > r->x) && (xDown < r->x +r->w)) &&
        ((xUp > r->x) && (xUp < r->x +r->w))&&
        ((yDown > r->y) && (yDown < r->y +r->h)) &&
        ((yUp > r->y) && (yUp < r->y +r->h)))
    {
        return true; //click coordinates inside  SDL_Rect r
    }
    return false; //click coordinates outside inside  SDL_Rect r
}

void Game::draw()
{
    drawStaticElements();
    drawDynamicElements();
}

// Private functions
void Game::drawStaticElements()
{

    //Window BG Color
    SDL_SetRenderDrawColor(TextureFactory::instance()->getRenderer(),
                           Constants::COL_WINDOW_BG.r,
                           Constants::COL_WINDOW_BG.g,
                           Constants::COL_WINDOW_BG.b,
                           Constants::COL_WINDOW_BG.a);
    SDL_RenderClear(TextureFactory::instance()->getRenderer());

	// Title
    TextureFactory::instance()->drawTexture("textTitleTexture", NULL, &m_textTitleRect);

    // Buttons BG Color
    SDL_SetRenderDrawColor(TextureFactory::instance()->getRenderer(),
                           Constants::COL_BUTTON_BG.r,
                           Constants::COL_BUTTON_BG.g,
                           Constants::COL_BUTTON_BG.b,
                           Constants::COL_BUTTON_BG.a);

    SDL_RenderFillRect(TextureFactory::instance()->getRenderer(),
                       &m_buttonStartRect);

    TextureFactory::instance()->drawTexture("buttonStartTex", NULL, &m_buttonStartRect);

    SDL_RenderFillRect(TextureFactory::instance()->getRenderer(),
                       &m_buttonStopRect);

    TextureFactory::instance()->drawTexture("buttonStopTex", NULL, &m_buttonStopRect);

    //test IMG
    TextureFactory::instance()->drawTexture("test", NULL, &m_buttonStopRect);

    m_chessBoard.drawBoard();
}

void Game::drawDynamicElements()
{
    // Dynamic Draw / Text
    // FEN Chess Board Notation - click to copy to clipboard
    TextureFactory::instance()->textureFromFont("textInfoTexture",
                                                "Segoe28",
                                                m_chessBoard.getMutableFENDescriptionQueue().back().c_str(),
                                                Constants::COL_TXT_DARK,
                                                1280, 0);

    TextureFactory::instance()->drawTexture("textInfoTexture",
                                            NULL, &m_textFENRect);

    TextureFactory::instance()->destroyTexture("textInfoTexture");

    // Statistics for the simulation time
    std::string dynamic_text_Times = m_chessBoard.getSimulationSummary();

    TextureFactory::instance()->textureFromFont("textTimeTexture",
                                                "Segoe28",
                                                dynamic_text_Times.c_str(),
                                                Constants::COL_TXT_LIGHT, 640, 0);

    TextureFactory::instance()->drawTexture("textTimeTexture",
                                            NULL, &m_textTimeRect);

    TextureFactory::instance()->destroyTexture("textTimeTexture");

    // Highlight allowed moves by clicking on chess piece when not simulating
    m_chessBoard.drawBoardOverlay();

    // Draw the chess pieces
    m_chessBoard.drawPieces();

}
