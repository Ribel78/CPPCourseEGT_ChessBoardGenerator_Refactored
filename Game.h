#pragma once

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

    SDL_Window* m_window = NULL;
    bool m_running;

    ChessBoard m_chessBoard;

    std::fstream m_dataStream;

};
