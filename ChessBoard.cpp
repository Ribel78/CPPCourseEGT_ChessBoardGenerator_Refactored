#include "ChessBoard.h"

ChessBoard::ChessBoard(){

    this->renderer = renderer;

    setPiecesToRemove(22);

	ChessBoard::simulating = false; //simulate chessboard 

	//Chess Board Size and Color
	chess_board_size = 640;
    chess_board_color[0] = {214, 187, 141, 255}; //"white" square
	chess_board_color[1] = {198, 130, 66, 255}; //"black" square
    chess_board_color[2] = {100, 255, 100, 50}; // highlight color
    for (int i = 0; i < 64; i++){
        chess_board_square[i] = new SDL_Rect{0, 0, 0, 0};
    }

	chessPieceIdx = -1;

	queueCustomSetDescription.push("rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR");
	queueFENSetDescription.push("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");     

}

ChessBoard::~ChessBoard(){

	//Chess
    for (int i = 0; i < 64; i++){
        delete chess_board_square[i];
    }	

}

void ChessBoard::setRenderer(SDL_Renderer* renderer){
    this->renderer = renderer;
}
void ChessBoard::prep_chess_piece_textures()
{
    //TODO See if Texture factory can be used here or move to new class - ChessBoard?

	//chess pieces in uinicode white to black in order King, Queen, Rook, Bishop, Knight, Pawn 
	//std::string cp_unicode[12] = {"\u2654", "\u2655", "\u2656", "\u2657", "\u2658", "\u2659", "\u265A", "\u265B", "\u265C", "\u265D", "\u265E", "\u265F"};
	//only black chess characters
	std::string cpb_unicode[6] = {"\u265A", "\u265B", "\u265C", "\u265D", "\u265E", "\u265F"};

	// Create textures from font chess characters - only the black pieces (6)
    TTF_Font *DejaVu = TextureFactory::Instance()->fonts["DejaVu"];
    SDL_Surface *tempSurfaceText = NULL;
    for (int i = 0; i < 12; i++)
    {
        if (i / 6 == 0)
        { // white first - K, Q, R, B, N, P

            tempSurfaceText = TTF_RenderUTF8_Blended(DejaVu, cpb_unicode[i % 6].c_str(), {254, 237, 211, 255});
            chessPieces[i] = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);
        }
        else
        { // black second - k, q, r, b, n, p

            tempSurfaceText = TTF_RenderUTF8_Blended(DejaVu, cpb_unicode[i % 6].c_str(), {0, 0, 0, 255});
            chessPieces[i] = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);
        }
    }
    SDL_FreeSurface(tempSurfaceText);
}

//Chess

bool ChessBoard::isSimulating() {
	return ChessBoard::simulating;
}

void ChessBoard::setSimulating(bool state){
	ChessBoard::simulating = state;
}

void ChessBoard::setBoardDescriptionFromQueueBack(){
    boardDescription = queueCustomSetDescription.back();
}

void ChessBoard::setChessPieceIdx(int idx){
    chessPieceIdx = idx;
}

SDL_Rect* ChessBoard::getChessBoardSquareRect(int idx){
    return chess_board_square[idx];
}

std::string ChessBoard::getSimulationSummary(){
    return timer.simulationTimeToString();
}

//Positions all 64 SDL_Rect-s on a chess board
void ChessBoard::initBoard(){
    for (int i = 0; i < 64; i++){
        chess_board_square[i]->x = (i % 8)*(chess_board_size / 8);
        chess_board_square[i]->y = (i / 8)*(chess_board_size / 8);
        chess_board_square[i]->w = chess_board_square[i]->h = chess_board_size / 8;
    }	
}

//Draws colored squares on the positioned SDL_Rect-s
void ChessBoard::drawBoard(){
	for (int i = 0; i < 64; i++){
		SDL_SetRenderDrawColor(
			renderer,
			chess_board_color[(i + ( i / 8 ) % 2 ) %2 ].r,
			chess_board_color[(i + ( i / 8 ) % 2 ) %2].g,
			chess_board_color[(i + ( i / 8 ) % 2 ) %2].b,
			chess_board_color[(i + ( i / 8 ) % 2 ) %2].a
		);
		SDL_RenderFillRect(renderer,chess_board_square[i]);
	}
}
//Draw allowed positions of the selected chess piece
void ChessBoard::drawBoardOverlay(){
	//bool showOverlay = true;
	if (!simulating && chessPieceIdx > -1){
		int x = chessPieceIdx % 8;
		int y = chessPieceIdx / 8;
		std::string overlay = attackSquares(boardDescription, x, y, '\0' );
		for (int i = 0; i < 64; i++){
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 
				chess_board_color[2].r, chess_board_color[2].g, 
				chess_board_color[2].b, ((overlay[i]!='-') ? 100 : 0));
			SDL_RenderFillRect(renderer,chess_board_square[i]);
		}
	} else {
		//Board descriptions is empty - no overlay
		boardDescription = "----------------------------------------------------------------";
	}
}


void ChessBoard::drawPieces(){
	//chess pieces lookup string - capitals are white
	std::string cp_lookupRef = "KQRBNPkqrbnp";

	//Example FEN chess board description - Rp5k/4pqpb/1R4P1/r1p1Pp1n/1r2PQ1P/3NN3/1BPpP2p/bP1BKpnP
	//declare string descriptions
	std::string chessBoardShuffle;
	std::string fenChessBoard;

	//init string descriptions
	shufflePieces(isSimulating(), chessBoardShuffle, fenChessBoard);

	for (int i = 0; i < 64; i++){
		if (chessBoardShuffle[i] == '-'){
			continue;
		}
		for (int j = 0; j < 12; j++){
			if (chessBoardShuffle[i] == cp_lookupRef[j]){
				SDL_RenderCopy(renderer, chessPieces[j], NULL, chess_board_square[i]);
				continue;
			}
		}
	}
}

void ChessBoard::setPiecesToRemove(int amount){
    if (amount < 0 || amount > 30){
        piecesToRemove = 8; //default
    } else {
        piecesToRemove = amount;
    }
    
}

/*
Randomize a char string of 64 chars of modified FEN chess board description
and update references of custom and correct annotation variables
_shuff - bool to shuffle (1) or keep ordered chess set
_&custDescription - string reference to write the chess board description to
_&fenDescription - string reference to write the FEN notation of the chess board description
*/
void ChessBoard::shufflePieces(bool shuff, std::string &custDescription, std::string &fenDescription){

	//Timer object mark start of simulation
	timer.markStart();

	char chess_set[] = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";

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
		int pieces_to_remove = piecesToRemove; // !!! this variable controls how many pieces to see on the board
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
			//check if pieces_to_remove is negative (in case pawns on end rows exceeds the set amount) - cancel this simulation
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
																rand_index % 8, rand_index / 8, 
																blackPieces[b_p]);				
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

		/*Parsing the randomized custom chess board description to FEN notation
		More info here: //https://www.chess.com/terms/fen-chess */

		char FEN[71] = {'0',}; //char array to hold the pieces including separators in FEN Format
        parseFEN(chess_set, FEN);

		//converting chess_set to string and push to queue
		std::string temp(chess_set);
		custDescription = temp;
		queueCustomSetDescription.push(custDescription);

        temp = std::string(FEN);
		fenDescription = temp;
		queueFENSetDescription.push(fenDescription);

		//if queue exceeds 20 pop one out
		if(queueCustomSetDescription.size()==21){
			queueCustomSetDescription.pop();
			queueFENSetDescription.pop();
		}

	} else{
		// custDescription = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";
		// fenDescription = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
		custDescription = queueCustomSetDescription.back();
		fenDescription = queueFENSetDescription.back();
	}
}

/*
Parse custom chess board description to FEN notation
Not Important but fun to have. Click on the notation in the window to copy it in the clipboard
Takes char array pointers to the custom chess board description and 
char array with 71 chars for the notation (the longest FEN notation is 71 characters).
Both arrays must be already initialized outside the function.
*/
void ChessBoard::parseFEN(char chess_set[65], char FEN[71])
{

   	char tempFEN[71]; //char array to hold the positions including separators '/' (64+7)
    int j = 0;
    for (int i = 0; i < 64; i++)
    {
        tempFEN[j] = chess_set[i];
        if ((i + 1) % 8 == 0 && ((i + 1) > 0 && (i + 1) < 64))
        {
            j += 1;
            tempFEN[j] = '/';
        }
        j += 1;
    }

    int empty_space = 0;
    j = 0;
    int count = 0;
    while (count < 71)
    {
        if (tempFEN[count] != '-' && empty_space == 0)
        {

            FEN[j] = tempFEN[count];
            count += 1;
            j += 1;
        }
        else if (tempFEN[count] == '-')
        {

            empty_space += 1;
            FEN[j] = ('0' + empty_space);
            count += 1;
        }
        else
        {
            j += 1;
            empty_space = 0;
        }
    }
}
