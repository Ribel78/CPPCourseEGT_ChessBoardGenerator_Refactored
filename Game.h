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
    bool buttonClicked(const SDL_Rect* r,
                       int xDown, int yDown,
                       int xUp, int yUp) const;
    auto buttonFocus(const SDL_Rect* r, int x, int y) const -> bool;
    void draw();

private:

    SDL_Window* m_window = NULL;
    bool m_running;

    int m_mouseDownX, m_mouseDownY, m_offsetX;

    ChessBoard m_chessBoard;

    std::fstream data_stream;

};
