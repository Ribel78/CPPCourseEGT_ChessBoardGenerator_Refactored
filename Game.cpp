//Game.cpp
#include <stdlib.h>
#include <iostream>
#include "Game.h"
#include "TextureFactory.h"
#include "Resources.h"
#include "Constants.h"
#include "Utilities.h"
#include "Interface.h"


Game::Game(): m_interface(m_chessBoard)
{
    Game::m_window = nullptr;
    Game::m_running = true;
}

Game::~Game()
{
    std::cout << "Destructing Game Object." << std::endl;
}
//Initialize SDL library
auto Game::Init(const char* title,
                int xPos, int yPos,
                int width, int height,
                int flags) -> bool
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
		std::cout << "SDL init success\n";

        m_window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
        if (m_window != 0)
		{
            std::cout << "Window creation success\n";

            SDL_Renderer* renderer = SDL_CreateRenderer(m_window, -1, 0);
            if (renderer != 0)
			{
                TextureFactory::Instance()->SetRenderer(renderer);

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

    PrepareTextures();

	return true;
}

void Game::PrepareTextures()
{
    //Prepare resources
    PrepFonts();
    PrepStaticFontTextures();
    PrepStaticImageTextures();
    PrepChessPieceTextures(true);
    PrepBoardLabelsTextures();
}

void Game::Update()
{
    //Render All
    SDL_RenderPresent(TextureFactory::Instance()->GetRenderer());

    if(m_dataStream.is_open())
        m_dataStream << m_chessBoard.GetCurrentDescription().m_customDescription << " "
                     << m_chessBoard.GetCurrentDescription().m_fenDescription << " "
                     << m_chessBoard.GetCurrentDescription().m_simulationTime << " "
                     << m_chessBoard.GetCurrentDescription().m_chessPieces
                     << std::endl;

    //Slow down visual shuffling
    SDL_Delay(6);
}

void Game::HandleEvents()
{
	SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: m_running = false; break;

        case SDL_KEYUP:{

            m_chessBoard.SetChessPieceIdx(-1);

            if(event.key.keysym.sym == SDLK_DOWN)
            {
                if(!m_chessBoard.IsSimulating())
                {
                    m_chessBoard.ViewDescriptionNext();
				}
			}
            if(event.key.keysym.sym == SDLK_UP)
            {
                if(!m_chessBoard.IsSimulating())
                {
                    m_chessBoard.ViewDescriptionPrevious();
                }
            }
		}; break;
        case SDL_MOUSEBUTTONDOWN:
        {
            int mouseX, mouseY;

            if (event.button.button == SDL_BUTTON_LEFT)
            {
                SDL_GetMouseState(&mouseX, &mouseY);

                m_interface.SetMouseDownCoords(mouseX, mouseY);

                m_interface.UpdateBtnTexturesOnFocus();
			}
            //Experimental: Delete Pieces from Board by clicking with Middle Mouse in Viewer Mode
            if (event.button.button == SDL_BUTTON_MIDDLE)
            {
                SDL_GetMouseState(&mouseX, &mouseY);

                m_interface.SetMouseDownCoords(mouseX, mouseY);

                for (int i = 0; i < 64; i++)
                {
                    if(m_interface.IsButtonClicked(m_chessBoard.GetRectChessBoardTile(i), mouseX, mouseY))
                    {
                        m_chessBoard.SetChessPieceIdx(i);
                        m_chessBoard.SetBoardDescriptionFromQueueBack(); // if Simulator
                        std::cout << i << " - idx : cpIdx - " << m_chessBoard.GetChessPieceIdx() << " : cp-" <<
                        m_chessBoard.GetCurrentDescription().m_customDescription.at(i)<< std::endl;

                        std::string tempCD = m_chessBoard.GetCurrentDescription().m_customDescription.replace(i, 1, 1, '-');
                        m_chessBoard.SetAndPushCurrentDescription(
                            tempCD,
                            m_chessBoard.ParseToFENDescription(tempCD.c_str()),
                            "0.00000",
                            m_chessBoard.GetCurrentDescription().m_chessPieces
                            );
                        std::cout << m_chessBoard.GetCurrentDescription().m_customDescription << std::endl;
                        break;
                    }
                }
                m_chessBoard.SetChessPieceIdx(-1);
            }

		}; break;
        case SDL_MOUSEBUTTONUP:
        {
			int msx, msy;
            m_interface.setMouseOffset(-1);

            if (event.button.button == SDL_BUTTON_LEFT)
            {
                m_interface.ResetAllButtonsTexID();

				SDL_GetMouseState(&msx, &msy);
                // toggle simulation button
                if(m_interface.IsButtonClicked(m_interface.GetRectButtonSimulator(), msx, msy) &&
                    !m_chessBoard.IsViewing())
                {
                    m_chessBoard.OpenDescriptionFileForWriting(m_dataStream);

                    if (!m_chessBoard.IsSimulating())
                    {
                        m_chessBoard.SetChessPieceIdx(-1);
                        m_chessBoard.ResetSimulationStatistics();
                    }

                    m_chessBoard.ToggleSimulating();
				}

                //viewer button clicked
                if(m_interface.IsButtonClicked(m_interface.GetRectButtonViewer(), msx, msy) &&
                    !m_chessBoard.IsSimulating())
                {
                    if (m_chessBoard.IsViewing())
                    {
                        m_chessBoard.GetMutableCBDescriptionsVecSeek() = 0;
                    }

                    m_chessBoard.ReadDescriptionFile(m_dataStream);

                    m_chessBoard.ToggleViewing();

                    m_chessBoard.SetChessPieceIdx(-1);
				}

                if(m_interface.IsButtonClicked(m_interface.GetRectTextFEN(), msx, msy) &&
                    !m_chessBoard.IsSimulating())
                {
                    m_chessBoard.CopyFENtoClipboard();

                    OpenURL(Constants::URL_365CHESS);
                }

                for (int i = 0; i < 64; i++)
                {
                    if(m_interface.IsButtonClicked(m_chessBoard.GetRectChessBoardTile(i), msx, msy) &&
                        !m_chessBoard.IsSimulating())
                    {
                        m_chessBoard.SetChessPieceIdx(i);

                        m_chessBoard.SetCurrentBoardDescriptionSrc();

                        break;
                    }
                }
			}
		}; break;

		default: break;
		}
	}
}

void Game::Clean()
{
    std::cout << "cleaning game\n";
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(TextureFactory::Instance()->GetRenderer());
    SDL_Quit();
}

auto Game::IsRunning() const -> bool
{
    return Game::m_running;
}

void Game::Draw()
{
    m_interface.DrawWindowBackground();
    m_interface.DrawTitle();

    m_chessBoard.DrawBoard();

    m_interface.DrawModeToggleButtons();
    m_interface.DrawFENDescription();
    m_interface.DrawStatistics();
    m_interface.DrawSlider();

    m_chessBoard.DrawBoardOverlay();
    m_chessBoard.DrawPieces();
}

