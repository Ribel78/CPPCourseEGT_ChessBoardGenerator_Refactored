#pragma once

#include "Interface.h"
#include "ChessBoard.h"
#include <fstream>

class Game
{
public:
	Game();
	~Game();

    auto init(const char* title,
              int xpos, int ypos,
              int width, int height,
              int flags) -> bool;
    void prepTextures();
    void update();
    void handleEvents();
    void clean();
    auto isRunning() const -> bool;
    void draw();

private:
    bool m_running;
    SDL_Window* m_window = NULL;
    ChessBoard m_chessBoard;
    Interface m_interface;
    std::fstream m_dataStream;
};
