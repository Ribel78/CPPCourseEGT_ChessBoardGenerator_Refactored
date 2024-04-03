//Game.cpp
#include <stdlib.h>
#include <iostream>
#include "Game.h"
#include "TextureFactory.h"
#include "Resources.h"
#include "Constants.h"
#include "Utilities.h"


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
auto Game::init(const char* title,
                int xpos, int ypos,
                int width, int height,
                int flags) -> bool
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
		std::cout << "SDL init success\n";

        m_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (m_window != 0)
		{
            std::cout << "Window creation success\n";

            SDL_Renderer* renderer = SDL_CreateRenderer(m_window, -1, 0);
            if (renderer != 0)
			{
                TextureFactory::instance()->setRenderer(renderer);

                std::cout << "Renderer creation success\n";
			}
            else
            {
                std::cout << "Renderer init failed\n";

				return false;
			}
		}
        else
        {
            std::cout << "Window init failed\n";
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

    m_offsetX = -1;

    prepTextures();

	return true;
}

void Game::prepTextures()
{
    //Prepare resources
    prepFonts();
    prepStaticFontTextures();
    prepStaticImageTextures();
    prepChessPieceTextures();
    prepBoardLabelsTextures();
}

void Game::update()
{
    //Render All
    SDL_RenderPresent(TextureFactory::instance()->getRenderer());

    if(m_dataStream.is_open())
        m_dataStream << m_chessBoard.getCurrentDescription().Custom << " "
                    << m_chessBoard.getCurrentDescription().FEN << " "
                    << m_chessBoard.getCurrentDescription().simulationTime << " "
                    << m_chessBoard.getCurrentDescription().chess_pieces
                    << std::endl;

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
         * seek trough the simulations using Down and Up Arrow Keys
        */
        case SDL_KEYUP:{
            if(event.key.keysym.sym == SDLK_DOWN)
            {
                if(!m_chessBoard.isSimulating())
                {
                    m_chessBoard.setChessPieceIdx(-1);
                    m_chessBoard.viewDescriptionNext();
				}
			}
            if(event.key.keysym.sym == SDLK_UP)
            {
                if(!m_chessBoard.isSimulating())
                {
                    m_chessBoard.setChessPieceIdx(-1);
                    m_chessBoard.viewDescriptionPrevious();
                }
            }
		}; break;
        case SDL_MOUSEBUTTONDOWN:
        {
            using namespace Constants;
			int msx, msy;

            if (event.button.button == SDL_BUTTON_LEFT)
            {
				SDL_GetMouseState(&msx, &msy);
                m_mouseDownX = msx;
                m_mouseDownY = msy;

                if(buttonFocus(m_chessBoard.getRectSliderKnob(), msx, msy))
                {
                    m_offsetX = m_mouseDownX - m_chessBoard.getRectSliderKnob()->x;
                }

                if(buttonFocus(m_chessBoard.getRectButtonViewer(),msx, msy))
                {
                    m_chessBoard.setButtonSimulatorTexID(Constants::ID_BTN_SIMULATOR_DOWN);
                    m_chessBoard.setButtonViewerTexID(Constants::ID_BTN_VIEWER_DOWN);
                }

                if(buttonFocus(m_chessBoard.getRectButtonSimulator(), msx, msy))
                {
                    m_chessBoard.setButtonStartTexID(Constants::ID_BTN_START_DOWN);
                    m_chessBoard.setButtonStopTexID(Constants::ID_BTN_STOP_DOWN);
                }
			}
		}; break;
        case SDL_MOUSEBUTTONUP:
        {
			int msx, msy;
            m_offsetX = -1;

            if (event.button.button == SDL_BUTTON_LEFT)
            {
                m_chessBoard.setButtonSimulatorTexID(Constants::ID_BTN_SIMULATOR_UP);
                m_chessBoard.setButtonViewerTexID(Constants::ID_BTN_VIEWER_UP);
                m_chessBoard.setButtonStartTexID(Constants::ID_BTN_START_UP);
                m_chessBoard.setButtonStopTexID(Constants::ID_BTN_STOP_UP);

				SDL_GetMouseState(&msx, &msy);
                // toggle simulation button
                if(buttonClicked(m_chessBoard.getRectButtonSimulator(),
                                  m_mouseDownX,m_mouseDownY,
                                  msx, msy) &&
                                !m_chessBoard.isViewing())
                {
                    if (m_chessBoard.isSimulating())
                    {
                        m_chessBoard.setSimulating(false);
                        m_dataStream.close();
                    }
                    else
                    {
                        m_chessBoard.setSimulating(true);
                        m_chessBoard.setChessPieceIdx(-1);
                        m_chessBoard.resetSimulationSummary();

                        m_dataStream.open("data/descriptions.csv", std::ios::out);
                        if(!m_dataStream.is_open())
                            std::cout << "Failed to open data/descriptions.csv" << std::endl;
                    }
				}

                //viewer button clicked
                if(buttonClicked(m_chessBoard.getRectButtonViewer(),
                                  m_mouseDownX,m_mouseDownY,
                                  msx, msy) &&
                                !m_chessBoard.isSimulating())
                {
                    if (m_chessBoard.isViewing())
                    {
                        m_chessBoard.setViewing(false);

                        m_chessBoard.getMutableCBDescriptionsVecSeek() = 0;
                    }
                    else
                    {
                        m_chessBoard.setViewing(true);

                        m_chessBoard.getMutableDescriptionsVector().clear();

                        m_dataStream.open("data/descriptions.csv", std::ios::in);

                        ChessBoardDescriptions temp_cb_descr;

                        while(m_dataStream >> temp_cb_descr)
                        {
                            m_chessBoard.getMutableDescriptionsVector().push_back(temp_cb_descr);
                        }
                        m_dataStream.close();
                    }

                    m_chessBoard.setChessPieceIdx(-1);
				}

                if(buttonClicked(m_chessBoard.getRectTextFEN(),
                                  m_mouseDownX,m_mouseDownY,
                                  msx, msy) &&
                    !m_chessBoard.isSimulating())
                {

                    m_chessBoard.copyFENtoClipboard();

                    openURL(Constants::URL_365CHESS);

                }

                for (int i = 0; i < 64; i++)
                {
                    if(buttonClicked(m_chessBoard.getRectChessBoardTile(i),
                                      m_mouseDownX, m_mouseDownY,
                                      msx, msy) &&
                        !m_chessBoard.isSimulating())
                    {
                        m_chessBoard.setChessPieceIdx(i);

                        if(!m_chessBoard.isViewing())
                        {
                            m_chessBoard.setBoardDescriptionFromQueueBack();
                        }
                        else
                        {
                            m_chessBoard.setBoardDescriptionFromVector();
                        }
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

auto Game::isRunning() const -> bool
{
    return Game::m_running;
}

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

auto Game::buttonFocus(const SDL_Rect* r,
                       int x, int y) const -> bool
{
    if(((x > r->x) && (x < r->x +r->w)) &&
       ((y > r->y) && (y < r->y +r->h)))
    {
        return true; //click coordinates inside  SDL_Rect r
    }
    return false; //click coordinates outside inside  SDL_Rect r
}

void Game::draw()
{
    m_chessBoard.drawWindowBackground();
    m_chessBoard.drawTitle();
    m_chessBoard.drawBoard();

    m_chessBoard.drawModeToggleButtons();
    m_chessBoard.drawFENDescription();
    m_chessBoard.drawStatistics();

    m_chessBoard.drawSlider(m_offsetX);

    m_chessBoard.drawBoardOverlay();
    m_chessBoard.drawPieces();
}

