#pragma once

#include "ChessBoard.h"
#include <fstream>

class Game
{
public:
	Game();
	~Game();

    bool init(const char* title,
              int xpos, int ypos,
              int width, int height,
              int flags);
    void prepTextures();
    void update();
    void handleEvents();
    void clean();
    bool isRunning() const;
    bool buttonClicked(const SDL_Rect* r,
                       int xDown, int yDown,
                       int xUp, int yUp) const;
    void draw();

private:

    SDL_Window* m_window = NULL;
    bool m_running;

    int m_mouseDownX, m_mouseDownY;

    ChessBoard m_chessBoard;

    std::fstream data_stream;

};
