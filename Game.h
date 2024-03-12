#pragma once

#include "ChessBoard.h"

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
    bool buttonClicked(SDL_Rect* r,
                       int xDown, int yDown,
                       int xUp, int yUp) const;

    void draw();


private:

    SDL_Window* m_window = NULL;
    SDL_Renderer* m_renderer = NULL;
    bool m_running;

    int m_mouseDownX, m_mouseDownY;

    //Chess drawing functions
    void drawStaticElements();
    void drawDynamicElements();

    //Rectangles
    SDL_Rect m_textTitleRect;
    SDL_Rect m_buttonStartRect;
    SDL_Rect m_buttonStopRect;
    SDL_Rect m_textFENRect;
    SDL_Rect m_textTimeRect;

    ChessBoard m_chessBoard;

};
