#pragma once

#include "Interface.h"
#include "ChessBoard.h"
#include <fstream>

class Game
{
public:
	Game();
	~Game();

    auto Init(const char* title,
              int xpos, int ypos,
              int width, int height,
              int flags) -> bool;
    void PrepareTextures();
    void Update();
    void HandleEvents();
    void Clean();
    auto IsRunning() const -> bool;
    void Draw();

private:
    bool m_running;
    SDL_Window* m_window = NULL;
    ChessBoard m_chessBoard;
    Interface m_interface;
    std::fstream m_dataStream;
};
