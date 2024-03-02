//Game.h
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <queue>
#include <iostream>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>
#include <cstring>

#include "Utilities.h"
#include "Timer.h"
#include "TextureFactory.h"

class Game {
public:
	Game();
	~Game();
	bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
    bool prep_textures();
    void prep_chess_piece_textures();
    void update();
    void handleEvents();
    void clean();
    bool isRunning();
    bool isSimulating(); //chess 
	void setSimulating(bool); //chess
	bool buttonClicked(SDL_Rect* r,  int xDown, int yDown, int xUp, int yUp);
	
	//Chess drawing functions
	void initBoard();
	void drawBoard();
	void drawBoardOverlay();
    void drawStaticElements();
    void drawDynamicElements();
    void drawPieces();

    /*
    Used to generate random chess board with 75% (24 pieces)
    */
    void shufflePieces(bool, std::string&, std::string&);

	/*
	Utility function to find the allowed (attack) squares of given chess piece
	Taking into account of the rules of chess pieces moves.
	Used in drawBoardOverlay and chess board generation
	*/
	friend std::string attackSquares(	std::string boardDescription, 
										int x, int y, 
										char piece);

private:

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	bool running;
	bool simulating;

	int mouseDownX, mouseDownY;

	//Chess Related fields
	int chess_size;
    SDL_Rect* chess_square[64];
    SDL_Color chess_color[3];	
	SDL_Texture* chessPieces[12];

	//Drawing Rectangles - currently hardcoded
	SDL_Rect textTitleRect;
	SDL_Rect buttonStartRect;
	SDL_Rect buttonStopRect;
	SDL_Rect infoTextRect;
	SDL_Rect timeTextRect;	

	//Seek trough the last 20 simulations using Down Arrow
	std::queue<std::string> queueCustomSetDescription;
	std::queue<std::string> queueFENSetDescription;

	//Simulation Time
	Timer timer;

	//Used in drawing the Board Overlay
	int chessPieceIdx;
	std::string boardDescription;

};
