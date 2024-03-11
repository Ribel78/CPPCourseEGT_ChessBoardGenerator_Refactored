//Game.cpp
#include "Game.h"

//Initialize SDL library
bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL init success\n";

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window != 0) //window init success
		{
			std::cout << "window creation success\n";
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer != 0) //renderer init success
			{
				std::cout << "renderer creation success\n";
				//SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);//not needed
				cb.setRenderer(renderer);
			}
			else {
				std::cout << "renderer init failed\n";
				return false;
			}
		}
		else {
			std::cout << "window init failed\n";
			return false;
		}
	}
	else {
		std::cout << "SDL init fail\n";
		return false;
	}
	//Init TTF library
	if(TTF_Init() == -1){
		return false;
	}	
	std::cout << "init success\n";
	running = true;
	cb.setSimulating(false);
	return true;
}

//Initialize SDL TTF library - rename the function to texture load or init
void Game::prep_textures(){

	// loading fonts into pointer variables
	TextureFactory::Instance()->loadFont("fonts/DejaVuSans.ttf","DejaVu", 48);
	TextureFactory::Instance()->loadFont("fonts/segoepr.ttf","Segoe", 72);
    TextureFactory::Instance()->loadFont("fonts/segoepr.ttf","Segoe28", 28);

	// Load textures
	TextureFactory::Instance()->textureFromFont(	"textTitleTexture","Segoe",
													"Chess Board Generator",
                                                    {0, 0, 0, 255}, 1280, renderer, 0);
	//formerly buttonStartTex
	TextureFactory::Instance()->textureFromFont(	"buttonStartTex","DejaVu",
													"     Start\n Simulation",
                                                    {235 ,235 ,255 ,255}, 300, renderer, 0);
	//formerly buttonStopTex
	TextureFactory::Instance()->textureFromFont(	"buttonStopTex","DejaVu",
													"     Stop\n Simulation",
                                                    {235 ,235 ,255 ,255}, 300, renderer, 0);


    // Parametrizing the layout of the destination rectangles
	int ww, wh;
	SDL_GetWindowSize(window, &ww, &wh);
	int padding = 40;
	int chess_sq = (ww / 2) / 8;

	textTitleRect = {ww / 2 + padding, padding / 2, ww / 2 - 2*padding, chess_sq};  
	buttonStartRect = {ww / 2 + padding, ww / 2 - chess_sq, (ww / 2 - 3*padding)/2, chess_sq}; 
	buttonStopRect = {ww / 2 + 2*padding + (ww / 2 - 3*padding)/2, ww / 2 - chess_sq, (ww / 2 - 3*padding)/2, chess_sq}; 
    textFENRect = {padding / 2, ww / 2 + 10, ww / 2 - padding, chess_sq - padding / 2};
	textTimeRect = {ww / 2 + padding, chess_sq * 2 , ww / 2 - 2*padding, wh / 3};

}



/*
Checks positions of the mousedown / mouseup separately 
and compare against 
given SDL_Rect coordinates and dimensions
*/
bool Game::buttonClicked(SDL_Rect* r,  int xDown, int yDown, int xUp, int yUp){

	if(((xDown > r->x) && (xDown < r->x +r->w)) && ((xUp > r->x) && (xUp < r->x +r->w))&&
		((yDown > r->y) && (yDown < r->y +r->h)) && ((yUp > r->y) && (yUp < r->y +r->h))){
			
			return true; //click coordinates inside  SDL_Rect r
	}
	return false; //click coordinates outside inside  SDL_Rect r
}

void Game::handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: running = false; break;

		case SDL_KEYUP:{ // Use down arrow to select a simulation from the past 20 stored in a queue
			if(event.key.keysym.sym == SDLK_DOWN){
				if(!cb.isSimulating()){

					cb.setChessPieceIdx(-1);
					std::string temp;
					temp = cb.queueFENSetDescription.front();
					cb.queueFENSetDescription.pop();
					cb.queueFENSetDescription.push(temp);
					temp = cb.queueCustomSetDescription.front();
					cb.queueCustomSetDescription.pop();
					cb.queueCustomSetDescription.push(temp);
					cb.setBoardDescriptionFromQueueBack();
				}
			}
		}; break;
		case SDL_MOUSEBUTTONDOWN: {
			int msx, msy;
			if (event.button.button == SDL_BUTTON_LEFT) {
				SDL_GetMouseState(&msx, &msy);
				mouseDownX = msx;
				mouseDownY = msy;
			}
		}; break;
		case SDL_MOUSEBUTTONUP: {			
			int msx, msy;
			if (event.button.button == SDL_BUTTON_RIGHT) {
				SDL_GetMouseState(&msx, &msy);
			}
			//int msx, msy;
			if (event.button.button == SDL_BUTTON_LEFT) {
				SDL_GetMouseState(&msx, &msy);
				if(buttonClicked(&buttonStartRect,mouseDownX,mouseDownY, msx, msy)){
					cb.setSimulating(true);
				}
				if(buttonClicked(&buttonStopRect,mouseDownX,mouseDownY, msx, msy)){
					cb.setSimulating(false);
				}
				if(buttonClicked(&textFENRect,mouseDownX,mouseDownY, msx, msy) && !cb.isSimulating()){ // Copy FEN code to clipboard
					SDL_SetClipboardText(cb.queueFENSetDescription.back().c_str());
				}	
				for (int i = 0; i < 64; i++){
					if(buttonClicked(cb.getChessBoardSquareRect(i), mouseDownX, mouseDownY, msx, msy) && !cb.isSimulating()){
						cb.setChessPieceIdx(i);
						//chessPieceIdx = i;
						cb.setBoardDescriptionFromQueueBack();
						break;
					}	
				}		
			}
		}; break;
		default: break;
		}
	}
}

void Game::update() {
	//Render All
	SDL_RenderPresent(renderer);

	//To show some visual shuffling wait 50 ms
    SDL_Delay(6);
}

void Game::clean() {
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool Game::isRunning() {
	return Game::running;
}

Game::Game() {
	Game::window = NULL;
	Game::renderer = NULL;
	Game::running = true;
}

Game::~Game() {
	// delete window;
	// delete renderer;
}

void Game::drawStaticElements(){
	//Background color
	SDL_SetRenderDrawColor(renderer, 23,138,207, 255);
	SDL_RenderClear(renderer);	

	// Static Draw
	// Title
	TextureFactory::Instance()->drawTexture(renderer,"textTitleTexture",NULL,&textTitleRect);
	// Buttons
	SDL_SetRenderDrawColor(renderer, 50, 50, 110, 255); //Button BG 
	SDL_RenderFillRect(renderer,&buttonStartRect);
	TextureFactory::Instance()->drawTexture(renderer,"buttonStartTex",NULL, &buttonStartRect);
	SDL_RenderFillRect(renderer,&buttonStopRect);
	TextureFactory::Instance()->drawTexture(renderer,"buttonStopTex",NULL, &buttonStopRect);

}

void Game::drawDynamicElements()
{
    // Dynamic Draw / Text
    // FEN Chess Board Notation - click to copy to clipboard
	const char* dynamic_text_FEN = cb.queueFENSetDescription.back().c_str();
	TextureFactory::Instance()->textureFromFont(	"textInfoTexture", 
                                                    "Segoe28",
													dynamic_text_FEN, 
                                                    {0, 0, 0, 255}, 1280, renderer, 0);

	TextureFactory::Instance()->drawTexture(renderer, "textInfoTexture", NULL, &textFENRect);
	TextureFactory::Instance()->destroyTexture("textInfoTexture");

    // Statistics for the simulation time
    std::string dynamic_text_Times = cb.getSimulationSummary();
	TextureFactory::Instance()->textureFromFont(	"textTimeTexture", 
                                                    "Segoe28",
                                                    dynamic_text_Times.c_str(),
                                                    {255, 255, 255, 255}, 640, renderer, 0);
	
	TextureFactory::Instance()->drawTexture(renderer, "textTimeTexture", NULL, &textTimeRect);
	TextureFactory::Instance()->destroyTexture("textTimeTexture");

}
