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

    //m_offsetX = -1;

    prepTextures();

	return true;
}

void Game::prepTextures()
{
    //Prepare resources
    prepFonts();
    prepStaticFontTextures();
    prepStaticImageTextures();
    prepChessPieceTextures(true);
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

            m_chessBoard.setChessPieceIdx(-1);

            if(event.key.keysym.sym == SDLK_DOWN)
            {
                if(!m_chessBoard.isSimulating())
                {
                    m_chessBoard.viewDescriptionNext();
				}
			}
            if(event.key.keysym.sym == SDLK_UP)
            {
                if(!m_chessBoard.isSimulating())
                {
                    m_chessBoard.viewDescriptionPrevious();
                }
            }
		}; break;
        case SDL_MOUSEBUTTONDOWN:
        {
			int msx, msy;

            if (event.button.button == SDL_BUTTON_LEFT)
            {
				SDL_GetMouseState(&msx, &msy);

                m_chessBoard.setMouseDownCoords(msx, msy);

                m_chessBoard.updateBtnTexturesOnFocus();
			}
		}; break;
        case SDL_MOUSEBUTTONUP:
        {
			int msx, msy;
            m_chessBoard.m_offsetX = -1;

            if (event.button.button == SDL_BUTTON_LEFT)
            {
                m_chessBoard.resetAllButtonsTexID();

				SDL_GetMouseState(&msx, &msy);
                // toggle simulation button
                if(m_chessBoard.isButtonClicked(m_chessBoard.getRectButtonSimulator(), msx, msy) &&
                                !m_chessBoard.isViewing())
                {
                    m_chessBoard.openDescriptionFileForWriting(m_dataStream);

                    if (!m_chessBoard.isSimulating())
                    {
                        m_chessBoard.setChessPieceIdx(-1);
                        m_chessBoard.resetSimulationStatistics();
                    }

                    m_chessBoard.toggleSimulating();
				}

                //viewer button clicked
                if(m_chessBoard.isButtonClicked(m_chessBoard.getRectButtonViewer(), msx, msy) &&
                                !m_chessBoard.isSimulating())
                {
                    if (m_chessBoard.isViewing())
                    {
                        m_chessBoard.getMutableCBDescriptionsVecSeek() = 0;
                    }

                    m_chessBoard.readDescriptionFile(m_dataStream);

                    m_chessBoard.toggleViewing();

                    m_chessBoard.setChessPieceIdx(-1);
				}

                if(m_chessBoard.isButtonClicked(m_chessBoard.getRectTextFEN(), msx, msy) &&
                    !m_chessBoard.isSimulating())
                {

                    m_chessBoard.copyFENtoClipboard();

                    openURL(Constants::URL_365CHESS);

                }

                for (int i = 0; i < 64; i++)
                {
                    if(m_chessBoard.isButtonClicked(m_chessBoard.getRectChessBoardTile(i), msx, msy) &&
                        !m_chessBoard.isSimulating())
                    {
                        m_chessBoard.setChessPieceIdx(i);

                        m_chessBoard.setCurrentBoardDescriptionSrc();

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



void Game::draw()
{
    m_chessBoard.drawWindowBackground();
    m_chessBoard.drawTitle();
    m_chessBoard.drawBoard();

    m_chessBoard.drawModeToggleButtons();
    m_chessBoard.drawFENDescription();
    m_chessBoard.drawStatistics();

    m_chessBoard.drawSlider();

    m_chessBoard.drawBoardOverlay();
    m_chessBoard.drawPieces();
}

