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
    bool isButtonClicked(const SDL_Rect* r, int xUp, int yUp) const;
    auto buttonFocus(const SDL_Rect* r) const -> bool;
    void setMouseDownCoords(int x, int y);
    void updateBtnTexturesOnFocus();
    void readDescriptionFile();
    void openDescriptionFileForWriting();
    void setCurrentBoardDescriptionSrc();

    void draw();

private:

    SDL_Window* m_window = NULL;
    bool m_running;

    int m_mouseDownX, m_mouseDownY, m_offsetX;

    ChessBoard m_chessBoard;

    std::fstream m_dataStream;

};
