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
	simulating = false;
	return true;
}

//chess pieces in uinicode white to black in order King, Queen, Rook, Bishop, Knight, Pawn 
//std::string cp_unicode[12] = {"\u2654", "\u2655", "\u2656", "\u2657", "\u2658", "\u2659", "\u265A", "\u265B", "\u265C", "\u265D", "\u265E", "\u265F"};
//only black chess characters
std::string cpb_unicode[6] = {"\u265A", "\u265B", "\u265C", "\u265D", "\u265E", "\u265F"};
//chess pieces lookup reference according to FEN abbreviations
std::string cp_lookupRef = "KQRBNPkqrbnp";

//Initialize SDL TTF library - rename the function to texture load or init
bool Game::prep_textures(){

	// loading fonts into pointer variables
	TextureFactory::Instance()->loadFont("fonts/DejaVuSans.ttf","DejaVu", 48);
	TextureFactory::Instance()->loadFont("fonts/segoepr.ttf","Segoe", 72);
	// formerly textTitleTexture
	TextureFactory::Instance()->textureFromFont(	"textTitleTexture","Segoe",
													"Chess Board Generator",
													{0, 0, 0, 255}, 0, renderer, 0);
	//formerly buttonStartTex
	TextureFactory::Instance()->textureFromFont(	"buttonStartTex","DejaVu",
													"     Start\n Simulation",
													{235 ,235 ,255 ,255}, 0, renderer, 0);
	//formerly buttonStopTex
	TextureFactory::Instance()->textureFromFont(	"buttonStopTex","DejaVu",
													"     Stop\n Simulation",
													{235 ,235 ,255 ,255}, 0, renderer, 0);

	//Create textures from font chess characters - only the black pieces (6)	
	TTF_Font* DejaVu = TextureFactory::Instance()->fonts["DejaVu"];
	SDL_Surface* tempSurfaceText = NULL;
	for (int i = 0; i < 12; i++){
		if (i / 6 == 0){ //white first - K, Q, R, B, N, P

			tempSurfaceText = TTF_RenderUTF8_Blended(DejaVu, cpb_unicode[i%6].c_str(), {254, 237, 211, 255});
			chessPieces[i] = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

		} else { //black second - k, q, r, b, n, p
			
			tempSurfaceText = TTF_RenderUTF8_Blended(DejaVu, cpb_unicode[i%6].c_str(), {0 , 0, 0, 255});
			chessPieces[i] = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

		}
	}
	SDL_FreeSurface(tempSurfaceText);

	//Position destination rectangles
	textTitleRect = {680, 20, 560, 64};	
	buttonStartRect = {680, 560, 256, 64};
	buttonStopRect = {980, 560, 256, 64};

	//dynamic text KJFKKF
	infoFont = TTF_OpenFont("fonts/segoepr.ttf", 28); // try updating the text content
	if(infoFont == NULL){
		std::cout << "infoFont not loaded" << std::endl;
	}	

	return true;
}

bool Game::isClickableTextureClicked(SDL_Texture* t, SDL_Rect* r,  int xDown, int yDown, int xUp, int yUp){
	int tw, th;
	// get info about texture dimensions and assign it to variables
	SDL_QueryTexture(t, 0, 0, &tw, &th);

	// checks positions of the mouse when down and up separately and compare against the SDL_Rect positions
	if(((xDown > r->x) && (xDown < r->x +tw)) && ((xUp > r->x) && (xUp < r->x +tw))&&
		((yDown > r->y) && (yDown < r->y +th)) && ((yUp > r->y) && (yUp < r->y +th))){
			//click coordinates inside  SDL_Rect rectangle
			return true;
	}
	//try logic directly with SDL_Rect instead of Texture width and height
	return false;
}

bool Game::buttonClicked(SDL_Rect* r,  int xDown, int yDown, int xUp, int yUp){
	//int tw, th;
	// get info about texture dimensions and assign it to variables
	//SDL_QueryTexture(t, 0, 0, &tw, &th);

	// checks positions of the mouse when down and up separately and compare against the SDL_Rect positions
	if(((xDown > r->x) && (xDown < r->x +r->w)) && ((xUp > r->x) && (xUp < r->x +r->w))&&
		((yDown > r->y) && (yDown < r->y +r->h)) && ((yUp > r->y) && (yUp < r->y +r->h))){
			//click coordinates inside  SDL_Rect rectangle
			return true;
	}
	//try logic directly with SDL_Rect instead of Texture width and height
	return false;
}

void Game::handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: running = false; break;

		case SDL_KEYUP:{ // Use down arrow to select a simulation from the past 20 stored in a queue
			if(event.key.keysym.sym == SDLK_DOWN){
				if(!isSimulating()){
					chessPieceIdx = -1;
					std::string temp;
					temp = queueFENSetDescription.front();
					queueFENSetDescription.pop();
					queueFENSetDescription.push(temp);
					temp = queueCustomSetDescription.front();
					queueCustomSetDescription.pop();
					queueCustomSetDescription.push(temp);
					boardDescription = queueCustomSetDescription.back(); //or temp;
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
					setSimulating(true);
				}
				if(buttonClicked(&buttonStopRect,mouseDownX,mouseDownY, msx, msy)){
					setSimulating(false);
				}
				if(buttonClicked(&infoTextRect,mouseDownX,mouseDownY, msx, msy) && !isSimulating()){ // Copy FEN code to clipboard
					SDL_SetClipboardText(queueFENSetDescription.back().c_str());
				}	
				for (int i = 0; i < 64; i++){
					if(buttonClicked(chess_square[i], mouseDownX, mouseDownY, msx, msy) && !isSimulating()){
						chessPieceIdx = i;
						boardDescription = queueCustomSetDescription.back();
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

	//Destroy all Dynamic Text textures after Rendering to free memory
	SDL_DestroyTexture(textTimeTexture);
	SDL_DestroyTexture(textInfoTexture);

	//To show some visual shuffling wait 50 ms
	SDL_Delay(50);
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
	Game::simulating = false; //simulate chessboard

	//Chess Board Size and Color
	chess_size = 640;
    chess_color[0] = {214, 187, 141, 255}; //"white" square
	chess_color[1] = {198, 130, 66, 255}; //"black" square
    chess_color[2] = {100, 255, 100, 50}; // highlight color

    for (int i = 0; i < 64; i++){
        chess_square[i] = new SDL_Rect{0, 0, 0, 0};
    }

	chessPieceIdx = -1;

	queueCustomSetDescription.push("rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR");
	queueFENSetDescription.push("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}

Game::~Game() {
	// delete window;
	// delete renderer;
	//Chess
    for (int i = 0; i < 64; i++){
        delete chess_square[i];
    }	

	//dynamic text textures
	SDL_FreeSurface(tempSurfaceDynamicText); //closing surface for dynamic text
	TTF_CloseFont(infoFont); // closing font for dynamic text
}

//Chess

bool Game::isSimulating() {
	return Game::simulating;
}

void Game::setSimulating(bool state){
	Game::simulating = state;
}

void Game::initBackground(){
	SDL_SetRenderDrawColor(renderer, 23,138,207, 255);
	SDL_RenderClear(renderer);
}

//Positions all 64 SDL_Rect-s on a chess board
void Game::initBoard(){
    for (int i = 0; i < 64; i++){
        chess_square[i]->x = (i % 8)*(chess_size / 8);
        chess_square[i]->y = (i / 8)*(chess_size / 8);
        chess_square[i]->w = chess_square[i]->h = chess_size / 8;
    }	
}

//Draws colored squares on the positioned SDL_Rect-s
void Game::drawBoard(){
	for (int i = 0; i < 64; i++){
		SDL_SetRenderDrawColor(
			renderer,
			chess_color[(i + ( i / 8 ) % 2 ) %2 ].r,
			chess_color[(i + ( i / 8 ) % 2 ) %2].g,
			chess_color[(i + ( i / 8 ) % 2 ) %2].b,
			chess_color[(i + ( i / 8 ) % 2 ) %2].a
		);
		SDL_RenderFillRect(renderer,chess_square[i]);
	}
}
//Draw allowed positions of the selected chess piece
void Game::drawBoardOverlay(){
	//bool showOverlay = true;
	if (!simulating && chessPieceIdx > -1){
		int x = chessPieceIdx % 8;
		int y = chessPieceIdx / 8;
		std::string overlay = attackSquares(boardDescription, x, y, '\0' );
		for (int i = 0; i < 64; i++){
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 
				chess_color[2].r, chess_color[2].g, 
				chess_color[2].b, ((overlay[i]!='-') ? 100 : 0));
			SDL_RenderFillRect(renderer,chess_square[i]);
		}
	} else {
		//Board descriptions is empty - no overlay
		boardDescription = "----------------------------------------------------------------";
	}
}

void Game::drawStaticText(){
	// Static Draw
	// Title
	TextureFactory::Instance()->drawTexture(renderer,"textTitleTexture",NULL,&textTitleRect);
	// Buttons
	SDL_SetRenderDrawColor(renderer, 50,50,110,255); //Button BG 
	SDL_RenderFillRect(renderer,&buttonStartRect);
	TextureFactory::Instance()->drawTexture(renderer,"buttonStartTex",NULL, &buttonStartRect);
	SDL_RenderFillRect(renderer,&buttonStopRect);
	TextureFactory::Instance()->drawTexture(renderer,"buttonStopTex",NULL, &buttonStopRect);

	//Dynamic Draw / Text
	//FEN Chess Board Notation - click to copy to clipboard
	tempSurfaceDynamicText = TTF_RenderText_Blended(infoFont, 
	queueFENSetDescription.back().c_str(), {0,0,0,255});
	
	if(tempSurfaceDynamicText == NULL){
		std::cout << "Surface, not created" << std::endl;
	}
	textInfoTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceDynamicText);
	//query info from a texture and write to variables 
	int tw, th;
	SDL_QueryTexture(textInfoTexture, 0, 0, &tw, &th);
	int ww, wh;
	SDL_GetWindowSize(window, &ww, &wh);
	// Game object SDL_Rect - gets the dimensions from the texture for filling later
	infoTextRect = {(ww-tw)/2, 650, tw, th}; // for the textInfoTexture
	SDL_RenderCopy(renderer, textInfoTexture, NULL, &infoTextRect);


	//Statistics for the simulation time
	tempSurfaceDynamicText = TTF_RenderText_Blended_Wrapped(infoFont, 
	timer.simulationTimeToString().c_str(), {255,255,255,255}, 0);

	textTimeTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceDynamicText);
	//query info from a texture and write to variables
	SDL_QueryTexture(textTimeTexture, 0, 0, &tw, &th);
	timeTextRect = {ww/2 + 20, 200, tw, th}; // for the textInfoTexture
	SDL_RenderCopy(renderer, textTimeTexture, NULL, &timeTextRect);	
	
	SDL_FreeSurface(tempSurfaceDynamicText);

}

void Game::drawPieces(){

	//Example FEN chess board description - Rp5k/4pqpb/1R4P1/r1p1Pp1n/1r2PQ1P/3NN3/1BPpP2p/bP1BKpnP
	//init string descriptions
	std::string chessBoardShuffle;
	std::string fenChessBoard;

	shufflePieces(isSimulating(), chessBoardShuffle, fenChessBoard);

	for (int i = 0; i < 64; i++){
		if (chessBoardShuffle[i] == '-'){
			continue;
		}
		for (int j = 0; j < 12; j++){
			if (chessBoardShuffle[i] == cp_lookupRef[j]){
				SDL_RenderCopy(renderer, chessPieces[j], NULL, chess_square[i]);
				continue;
			}
		}
	}
}

/*
Randomize a char string of 64 chars of modified FEN chess board description
and update references of custom and correct annotation variables
_shuff - bool to shuffle (1) or keep ordered chess set
_&custDescription - string reference to write the chess board description to
_&fenDescription - string reference to write the FEN notation of the chess board description
*/
void Game::shufflePieces(bool shuff, std::string &custDescription, std::string &fenDescription){


	//Timer object mark start of simulation
	timer.markStart();

	char chess_set[] = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";

	//https://www.chess.com/terms/fen-chess
	char tempFEN[71]; //char array to hold the positions including separators '/' (64+7)
	char FEN[71] = {'0',}; //char array to hold the pieces including separators in FEN Format

	if (shuff){
		//goto label
		repeatSimulation:
		
		//Shuffle all 32 pieces
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine rand_en;
		rand_en.seed(seed);
		//random chess board with all pieces
		std::shuffle(chess_set, chess_set+64, rand_en); 

		// Check if bishops are on different colors if not re-shuffle
		int blackBishopOnBlack = 0;
		int blackBishopOnWhite = 0;
		int whiteBishopOnBlack = 0;
		int whiteBishopOnWhite = 0;
		for (int i = 0; i < 64; i++){
			if(chess_set[i]=='b'){ // black bishop found
				if( (( i / 8 ) % 2 == 0 && ( i % 8 ) % 2 == 1) ||
					(( i / 8 ) % 2 == 1 && ( i % 8 ) % 2 == 0) ){ // check if color of square is black
					blackBishopOnBlack += 1;
				} else {
					blackBishopOnWhite += 1;
				}
			}
			if(chess_set[i]=='B'){ // white bishop found
				if( (( i / 8 ) % 2 == 0 && ( i % 8 ) % 2 == 1) ||
					(( i / 8 ) % 2 == 1 && ( i % 8 ) % 2 == 0) ){ // check if color of square is black
					whiteBishopOnBlack += 1;
				} else {
					whiteBishopOnWhite += 1;
				}
			}
		}
		if (blackBishopOnBlack > 1 || blackBishopOnWhite > 1 || whiteBishopOnBlack > 1 || whiteBishopOnWhite > 1 ) {
			//If Bishops of a kind on same color square - reshuffle
			goto repeatSimulation;
		}

		//Remove all pawns if foud on end rows - keep count of removed pieces
		int pieces_to_remove = 8;	
		while(pieces_to_remove > 0){
			for(int i = 0; i < 64; i++){
				if(i < 8 || i > (64 - 9)){
					if(chess_set[i] == 'p' || chess_set[i] == 'P'){
						chess_set[i] = '-';
						pieces_to_remove -= 1;
					}
				}
				//Remove both Kings to reintroduce back when board is processed
				if((chess_set[i] == 'k' || chess_set[i] == 'K')){
						chess_set[i] = '-';
				}
			}
			//check if pieces_to_remove is negative (in case more than 8 pawns on end rows) - cancel this simulation
			if (pieces_to_remove < 0){
				goto repeatSimulation;
			}

			/*Remove randomly selected pieces until 8 pieces in total are removed 
			not counting the the removed kings*/
			if(pieces_to_remove > 0){
				int rand_index = rand()%64;
				if(chess_set[rand_index]!= '-'){
					chess_set[rand_index] = '-';
					pieces_to_remove -= 1;
				} 
			}
		}
		
		// Reintroduce Kings
		//std::string cp_lookupRef = "KQRBNPkqrbnp";
		char blackPieces[6] = {'k','q','r','b','n','p'};
		char whitePieces[6] = {'K','Q','R','B','N','P'}; 

		// Add  Black King
		bool isSafeSquare = false;
		std::string emptySquaresLookup(chess_set);
		while(!isSafeSquare){ //while isSafeSquare is false
			int rand_index = rand()%64; // random index
			if (chess_set[rand_index] != '-'){ //if not empty start new cycle
				continue;
			}
			for (int b_p = 0; b_p < 6; b_p++){ //test each black piece
				std::string attackedPieces = attackSquares(		emptySquaresLookup, 
															rand_index % 8, rand_index / 8, blackPieces[b_p]);				
				for (char piece : attackedPieces){ //for cell in attack board
					if (piece == whitePieces[b_p]){ //if white piece is oposite of current black piece
						isSafeSquare = false;
						break; //can't place here
					} else {
						isSafeSquare = true;
					}
				}	
				if (isSafeSquare == false){
					break;
				} else {
					isSafeSquare = true;
				}		
			}
			if (isSafeSquare == true){
				chess_set[rand_index] = 'k';
			}
		}	

		// Add  White King
		isSafeSquare = false;
		emptySquaresLookup =std::string(chess_set);

		while(!isSafeSquare){ //while isSafeSquare is false

			int rand_index = rand()%64; // random index
			if (chess_set[rand_index] != '-'){ //if not empty start new cycle
				continue;
			}
			for (int w_p = 0; w_p < 6; w_p++){ //test each black piece

				std::string attackedPieces = attackSquares(		emptySquaresLookup, 
															rand_index % 8, rand_index / 8, whitePieces[w_p]);
				
				for (char piece : attackedPieces){ //for cell in attack board
					if (piece == blackPieces[w_p]){ //if white piece is oposite of current black piece
						isSafeSquare = false;
						break; //can't place here
					} else {
						isSafeSquare = true;
					}
				}	
				if (isSafeSquare == false){
					break;
				} else {
					isSafeSquare = true;
				}		
			}
			if (isSafeSquare == true){
				chess_set[rand_index] = 'K';
			}
		}
		

		//End simulation - caclulate simulation duration in ns
		timer.markEnd();
		timer.setDurationInNanoseconds();
		timer.updateStats();

		std::string temp(chess_set);
		custDescription = temp;
		queueCustomSetDescription.push(custDescription);

		//parse custom description to FEN notation for dispaly
		int j = 0;
		for (int i = 0; i < 64; i++){
			tempFEN[j] = chess_set[i];
			if( (i+1) % 8 == 0 && (( i+1 ) > 0 && ( i+1 ) < 64)){
				j+=1;
				tempFEN[j] = '/';
			}
			j+=1;
		}

		int empty_space = 0;
		j=0;
		int count = 0;
		while(count < 71)
		{
			if (tempFEN[count] != '-' && empty_space==0){

				FEN[j] = tempFEN[count];
				count += 1;
				j += 1;

			} else if(tempFEN[count] == '-'){

				empty_space += 1;
				FEN[j] = ('0' + empty_space);
				count += 1;

			} else{
				j += 1;
				empty_space = 0;
			}
		}

		temp = std::string(FEN);
		fenDescription = temp;
		queueFENSetDescription.push(fenDescription);

		//if queue exceeds 20 pop one out
		if(queueCustomSetDescription.size()==21){
			queueCustomSetDescription.pop();
			queueFENSetDescription.pop();
		}
	}
		 else{
			// custDescription = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";
			// fenDescription = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
			custDescription = queueCustomSetDescription.back();
			fenDescription = queueFENSetDescription.back();
		}
}