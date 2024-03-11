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
    void prep_textures();
    void update();
    void handleEvents();
    void clean();
    bool isRunning();
    bool buttonClicked(SDL_Rect* r,
                       int xDown, int yDown,
                       int xUp, int yUp);
	
	//Chess drawing functions
    void drawStaticElements();
    void drawDynamicElements();

	ChessBoard cb;

private:

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	bool running;

	int mouseDownX, mouseDownY;
	//Drawing Rectangles
	SDL_Rect textTitleRect;
	SDL_Rect buttonStartRect;
	SDL_Rect buttonStopRect;
	SDL_Rect textFENRect;
	SDL_Rect textTimeRect;	

};
