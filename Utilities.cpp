//Utilities.cpp

#include "Utilities.h"
#include "Constants.h"
#include <iostream>

/*
_boardDescription - chess board description in custom format 
_boardOverlay - chess board description to update attack positions of the rook piece
_piece - the attacking rook ('r' or 'R')
_x, _y - attacking piece positions
_References to piece descriptions
_is the same piece of opposite color attacked
returns board with allowed positions ['X'] and attacked pieces of opposite color
if default piece then it takes the rook (make sure it is either 'r' or 'R') from the _boardDescription at position x,y as attacking piece
else puts the desired color rook at position x, y 
*/
void RookAttack(const std::string& boardDescription,
                std::string& boardOverlay,
                const char attackingPiece,
                int x, int y,
                const char (&blackPieces)[6],
                const char (&whitePieces)[6])
{

    boardOverlay[(y * 8) + x] = attackingPiece; //mark init position

    int posX = x; int posY = y;

    bool isHit = false;

    //sweep up
    while (!isHit && posY > 0)
    {
        posY -= 1;

        //empty square on path
        if (boardDescription[ (posY * 8) + posX ] == '-')
        {
            boardOverlay[ (posY * 8) + posX ] = 'X';
        }

        for (char wPiece : whitePieces)
        {
            if (boardDescription[ (posY * 8) + posX ] == wPiece)
            {
                //store the attacked white piece and end while
                if(attackingPiece == 'r')
                {
                    boardOverlay[ (posY * 8) + posX ] = wPiece;
                }
                isHit = true;
                break;
            }
        }
        for (char bPiece : blackPieces)
        {
            if (boardDescription[ (posY * 8) + posX ] == bPiece)
            {
                //store the attacked black piece and end while
                if(attackingPiece == 'R')
                {
                    boardOverlay[ (posY * 8) + posX ] = bPiece;
                }
                isHit = true;
                break;
            }
        }
    }

    posX = x; posY = y;

    isHit = false;

    //sweep down
    while (!isHit && posY < 7)
    {
        posY += 1;

        //empty square on path
        if (boardDescription[(posY * 8) + posX] == '-')
        {
            boardOverlay[(posY * 8) + posX] = 'X';
        }

        for (char wPiece : whitePieces)
        {
            if (boardDescription[(posY * 8) + posX] == wPiece)
            {
                //store the attacked white piece and end while
                if(attackingPiece == 'r')
                {
                    boardOverlay[(posY * 8) + posX] = wPiece;
                }
                isHit = true;
                break;
            }
        }
        for (char bPiece : blackPieces){
            if ( boardDescription[(posY * 8) + posX] == bPiece)
            {
                //store the attacked black piece and end while
                if(attackingPiece == 'R')
                {
                    boardOverlay[(posY * 8) + posX] = bPiece;
                }
                isHit = true;
                break;
            }
        }
    }

    posX = x; posY = y;

    isHit = false;

    //sweep left
    while (!isHit && posX > 0)
    {
        posX -= 1;
        //empty square
        if (boardDescription[ (posY * 8) + posX ] == '-')
        {
            boardOverlay[(posY * 8) + posX] = 'X';
        }

        for (char wPiece : whitePieces){
            if (boardDescription[(posY * 8) + posX] == wPiece)
            {
                if(attackingPiece == 'r')
                { //store the attacked white piece and end while
                    boardOverlay[(posY * 8) + posX] = wPiece;
                }
                isHit = true;
                break;
            }
        }
        for (char bPiece : blackPieces)
        {
            if (boardDescription[(posY * 8) + posX] == bPiece)
            {
                //store the attacked black piece and end while
                if(attackingPiece == 'R')
                {
                    boardOverlay[(posY * 8) + posX] = bPiece;
                }
                isHit = true;
                break;
            }
        }
    }

    posX = x; posY = y;

    isHit = false;

    //sweep right
    while (!isHit && posX < 7)
    {
        posX += 1;
        //empty square on path
        if (boardDescription[(posY * 8) + posX] == '-')
        {
            boardOverlay[(posY * 8) + posX] = 'X';
        }

        for (char wPiece : whitePieces)
        {
            if (boardDescription[ (posY * 8) + posX ] == wPiece){
                if(attackingPiece == 'r')
                { //store the attacked white piece and end while
                    boardOverlay[(posY * 8) + posX ] = wPiece;
                }
                isHit = true;
                break;
            }
        }
        for (char bPiece : blackPieces){
            if ( boardDescription[ (posY * 8) + posX ] == bPiece){
                    if(attackingPiece == 'R'){ //store the attacked black piece and end while
                        boardOverlay[ (posY * 8) + posX ] = bPiece;
                    }
                    isHit = true;
                    break;
            }
        }
    }
}

/*
_boardDescription - chess board description in custom format 
_boardOverlay - chess board description to update attack positions of the bishop piece
_x, _y - attacking piece positions
_piece - the attacking bishop ('b' or 'B')
returns board with allowed positions ['X'] and attacked pieces of opposite color
if default piece then it takes the bishop (make sure it is either 'b' or 'B') from the _boardDescription at position x,y as attacking piece
else puts the desired color bishop at position x, y 
*/
void BishopAttack(const std::string& boardDescription,
                  std::string& boardOverlay,
                  const char attackingPiece,
                  int x, int y,
                  const char (&blackPieces)[6],
                  const char (&whitePieces)[6])
{
    //mark bishop initial position
    boardOverlay[(y * 8) + x] = attackingPiece;

    int posX = x; int posY = y;

    bool isHit = false;

    //sweep left up
    while (!isHit && ( posY > 0 && posX > 0 ))
    {
        posY -= 1;
        posX -= 1;
        if ( boardDescription[ (posY * 8) + posX ] == '-')
        { //empty square on path
                boardOverlay[ (posY * 8) + posX ] = 'X';
        }

        for (char wPiece : whitePieces)
        {
            if ( boardDescription[ (posY * 8) + posX ] == wPiece)
            {
                //store the attacked white piece and end while
                if(attackingPiece == 'b')
                {
                    boardOverlay[(posY * 8) + posX] = wPiece;
                }
                isHit = true;
                break;
            }
        }
        for (char bPiece : blackPieces){
            if (boardDescription[(posY * 8) + posX] == bPiece)
            {
                if(attackingPiece == 'B')
                { //store the attacked black piece and end while
                    boardOverlay[(posY * 8) + posX] = bPiece;
                }
                isHit = true;
                break;
            }
        }
    }

    posX = x; posY = y;

    isHit = false;

    //sweep right up
    while (!isHit && ( posY > 0 && posX < 7 ))
    {
        posY -= 1;
        posX += 1;

        //empty square on path
        if ( boardDescription[ (posY * 8) + posX ] == '-')
        {
            boardOverlay[(posY * 8) + posX] = 'X';
        }

        for (char wPiece : whitePieces){
            if (boardDescription[ (posY * 8) + posX ] == wPiece){
                if(attackingPiece == 'b')
                { //store the attacked white piece and end while
                    boardOverlay[(posY * 8) + posX] = wPiece;
                }
                isHit = true;
                break;
            }
        }

        for (char bPiece : blackPieces){
            if (boardDescription[ (posY * 8) + posX] == bPiece){
                    if(attackingPiece == 'B'){  //store the attacked black piece and end while
                        boardOverlay[(posY * 8) + posX] = bPiece;
                    }
                    isHit = true;
                    break;
            }
        }
    }

    posX = x; posY = y;

    isHit = false;

    //sweep left down
    while (!isHit && (posY < 7 && posX > 0))
    {
        posY += 1;
        posX -= 1;

        //empty square
        if (boardDescription[ (posY * 8) + posX ] == '-')
        {
            boardOverlay[(posY * 8) + posX] = 'X';
        }

        for (char wPiece : whitePieces){
            if (boardDescription[(posY * 8) + posX] == wPiece)
            {
                //store the attacked white piece and end while
                if(attackingPiece == 'b')
                {
                    boardOverlay[(posY * 8) + posX] = wPiece;
                }
                isHit = true;
                break;
            }
        }
        for (char bPiece : blackPieces)
        {
            if (boardDescription[(posY * 8) + posX] == bPiece)
            {
                //store the attacked black piece and end while
                if(attackingPiece == 'B')
                {
                    boardOverlay[(posY * 8) + posX] = bPiece;
                }
                isHit = true;
                break;
            }
        }
    }

    posX = x; posY = y;

    isHit = false;

    //sweep right down
    while (!isHit && ( posY < 7 && posX < 7 ))
    {
        posY += 1;
        posX += 1;

        //empty square on path
        if (boardDescription[ (posY * 8) + posX ] == '-')
        {
            boardOverlay[(posY * 8) + posX] = 'X';
        }

        for (char wPiece : whitePieces){
            if (boardDescription[(posY * 8) + posX] == wPiece)
            {
                //store the attacked white piece and end while
                if(attackingPiece == 'b')
                {
                    boardOverlay[ (posY * 8) + posX ] = wPiece;
                }
                isHit = true;
                break;
            }
        }
        for (char bPiece : blackPieces){
            if (boardDescription[ (posY * 8) + posX] == bPiece)
            {
                //store the attacked black piece and end while
                if(attackingPiece == 'B')
                {
                    boardOverlay[(posY * 8) + posX] = bPiece;
                }
                isHit = true;
                break;
            }
        }
    }
}

/*
_boardDescription - chess board description in custom format 
_boardOverlay - chess board description to update attack positions of the queen piece
_x, _y - attacking piece positions
_piece - the attacking queen ('q' or 'Q')
_&isOpposite - returns true if opposite of same kind is attacked
returns board with allowed positions ['X'] and attacked pieces of opposite color
if default piece then it takes the queen (make sure it is either 'q' or 'Q') from the _boardDescription at position x,y as attacking piece
else puts the desired color queen at position x, y 
*/
void QueenAttack(const std::string& boardDescription,
                 std::string& boardOverlay,
                 const char attackingPiece,
                 int x, int y,
                 const char (&blackPieces)[6],
                 const char (&whitePieces)[6])
{
    if(attackingPiece == 'q')
    {
        //call rookAttack
        RookAttack (boardDescription,
                   boardOverlay,
                   'r', x, y,
                   blackPieces,
                   whitePieces);

        //call bishopAttack
        BishopAttack(
            boardDescription,
            boardOverlay,
            'b', x, y,
            blackPieces,
            whitePieces);
    }
    if(attackingPiece == 'Q')
    {
        //call rookAttack
        RookAttack(boardDescription,
                   boardOverlay,
                   'R', x, y,
                   blackPieces,
                   whitePieces);

        //call bishopAttack
        BishopAttack(boardDescription,
                     boardOverlay,
                     'B', x, y,
                     blackPieces,
                     whitePieces );
    }
    //mark queen initial position
    boardOverlay[(y * 8) + x] = attackingPiece;
}

/*
_boardDescription - chess board description in custom format 
_boardOverlay - chess board description to update attack positions of the knight piece
_x, _y - attacking piece positions
_piece - the attacking knight ('n' or 'N')
returns board with allowed positions ['X'] and attacked pieces of opposite color
if default piece then it takes the knight (make sure it is either 'n' or 'N') from the _boardDescription at position x,y as attacking piece
else puts the desired color knight at position x, y 
*/
void knightAttack(const std::string& boardDescription,
                  std::string& boardOverlay,
                  const char attackingPiece,
                  int x, int y,
                  const char (&blackPieces)[6],
                  const char (&whitePieces)[6])
{
    //mark bishop initial position
    boardOverlay[ (y * 8) + x ] = attackingPiece;

    int posX = x; int posY = y;

    bool isHit = false;

    //turn
    int knightPointX[] = {1, 2, 2, 1, -1, -2, -2, -1};

    int knightPointY[] = {2, 1, -1, -2, -2, -1, 1, 2};

    for (int p = 0; p < 8; p++)
    {
        posY = y + knightPointY[p];
        posX = x + knightPointX[p];

        if((posY >= 0 && posY <= 7) && (posX >= 0 && posX <= 7))
        {
            //empty square on path
            if (boardDescription[(posY * 8) + posX] == '-')
            {
                boardOverlay[(posY * 8) + posX] = 'X';
            }
            for (char wPiece : whitePieces)
            {
                if (boardDescription[(posY * 8) + posX] == wPiece)
                {
                    //store the attacked white piece and end while
                    if(attackingPiece == 'n')
                    {
                        boardOverlay[(posY * 8) + posX] = wPiece;
                    }
                }
            }
            for (char bPiece : blackPieces){
                if (boardDescription[(posY * 8) + posX ] == bPiece)
                {
                    //store the attacked black piece and end while
                    if(attackingPiece == 'N')
                    {
                        boardOverlay[(posY * 8) + posX] = bPiece;
                    }
                }
            }
        }
    }
}

/*
_boardDescription - chess board description in custom format 
_boardOverlay - chess board description to update attack positions of the king piece
_x, _y - attacking piece positions
_piece - the attacking king ('k' or 'K')
returns board with allowed positions ['X'] and attacked pieces of opposite color
if default piece then it takes the king (make sure it is either 'k' or 'K') from the _boardDescription at position x,y as attacking piece
else puts the desired color king at position x, y 
*/
void KingAttack(const std::string& boardDescription,
                std::string& boardOverlay,
                const char attackingPiece,
                int x, int y,
                const char (&blackPieces)[6],
                const char (&whitePieces)[6])
{
    boardOverlay[ (y * 8) + x ] = attackingPiece; //mark bishop initial position

    int posX = x; int posY = y;

    bool isHit = false;

    //turn
    int kingPointX[] = {0, 1, 1, 1, 0, -1, -1, -1};

    int kingPointY[] = {1, 1, 0, -1, -1, -1, 0, 1};

    for (int p = 0; p < 8; p++)
    {
        posY = y + kingPointY[p];
        posX = x + kingPointX[p];

        if ((posY >= 0 && posY <= 7) && (posX >= 0 && posX <= 7))
        {
            //empty square on path
            if ( boardDescription[ (posY * 8) + posX ] == '-')
            {
                boardOverlay[(posY * 8) + posX] = 'X';
            }
            for (char wPiece : whitePieces)
            {
                if (boardDescription[(posY * 8) + posX] == wPiece)
                {
                    //store the attacked white piece and end while
                    if(attackingPiece == 'k')
                    {
                        boardOverlay[ (posY * 8) + posX ] = wPiece;
                    }
                }
            }
            for (char bPiece : blackPieces)
            {
                if (boardDescription[(posY * 8) + posX] == bPiece)
                {
                    //store the attacked black piece and end while
                    if(attackingPiece == 'K')
                    {
                        boardOverlay[(posY * 8) + posX] = bPiece;
                    }
                }
            }
        }
    }
}

/*
_boardDescription - chess board description in custom format 
_boardOverlay - chess board description to update attack positions of the pawn piece
_x, _y - attacking piece positions
_piece - the attacking king ('p' or 'P')
returns board with allowed positions ['X'] and attacked pieces of opposite color
if default piece then it takes the pawn (make sure it is either 'p' or 'P') from the _boardDescription at position x,y as attacking piece
else puts the desired color pawn at position x, y 
*/
void PawnAttack(const std::string& boardDescription,
                std::string& boardOverlay,
                const char attackingPiece,
                int x, int y,
                const char (&blackPieces)[6],
                const char (&whitePieces)[6])
{
    //mark bishop initial position
    boardOverlay[(y * 8) + x] = attackingPiece;

    int posX = x; int posY = y;

    //attack
    int pawnPointX[] = {-1, 1};

    for (int p = 0; p < 2; p++)
    {
        posY = y + ( ( attackingPiece == 'p') ? 1 : -1 );
        posX = x + pawnPointX[p];

        if((posY >= 0 && posY <= 7) && (posX >= 0 && posX <= 7))
        {
            for (char wPiece : whitePieces)
            {
                if (boardDescription[(posY * 8) + posX] == wPiece)
                {
                    //store the attacked white piece and end while
                    if(attackingPiece == 'p')
                    {
                        boardOverlay[(posY * 8) + posX] = wPiece;
                    }
                }
            }
            for (char bPiece : blackPieces)
            {
                if (boardDescription[(posY * 8) + posX] == bPiece)
                {
                    //store the attacked black piece and end while
                    if(attackingPiece == 'P')
                    {
                        boardOverlay[(posY * 8) + posX] = bPiece;
                    }
                }
            }
        }
    }

    posX = x; posY = y;

    //move
    if (attackingPiece == 'p' && posY < 7)
    {
        posY += 1;

        if (boardDescription[ (posY * 8) + posX] == '-')
        { //empty square on path
            boardOverlay[(posY * 8) + posX] = 'X';
            if (y == 1)
            {
                posY += 1;
                //empty square on path
                if ( boardDescription[(posY * 8) + posX] == '-')
                {
                    boardOverlay[(posY * 8) + posX] = 'X';
                }
            }
        }
    }

    posY = y;

    if (attackingPiece == 'P' && posY > 0)
    {
        posY -= 1;
        //empty square on path
        if (boardDescription[(posY * 8) + posX] == '-')
        {
            boardOverlay[(posY * 8) + posX] = 'X';
            if (y == 6)
            {
                posY -= 1;
                //empty square on path
                if (boardDescription[ (posY * 8) + posX ] == '-')
                {
                    boardOverlay[ (posY * 8) + posX ] = 'X';
                }
            }
        }
    }
}

/*
_boardDescription - in custom format 
_x, _y - chess piece positions
_piece - chess piece (default = '\0')
Returns board with allowed positions ['X'] and attacked pieces of opposite color
if default piece then it takes the piece from _boardDescription at position x,y
else puts the desired piece at position x, y 
*/
std::string AttackSquares(const std::string& boardDescription,
                          int x, int y,
                          char attackingPiece)
{
    //a string to write sequential results in
    std::string boardOverlay = "";

    //fill with empty sqwares '-'
    for (int i = 0; i < 64; i++)
    {
        boardOverlay.append("-");
    }

    // when default piece - take the piece with corrds x,y from the board
    if(!attackingPiece)
    {
        attackingPiece = boardDescription[(y * 8 + x)];
    }

    if (attackingPiece == 'r')
    {
        RookAttack(boardDescription,
                   boardOverlay,
                   'r', x, y,
                   Constants::CP_LABELS_BLACK,
                   Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'R')
    {
        RookAttack(boardDescription,
                   boardOverlay,
                   'R', x, y,
                   Constants::CP_LABELS_BLACK,
                   Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'b')
    {
        BishopAttack(boardDescription,
                     boardOverlay,
                     'b', x, y,
                     Constants::CP_LABELS_BLACK,
                     Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'B')
    {
        BishopAttack(boardDescription,
                     boardOverlay,
                     'B', x, y,
                     Constants::CP_LABELS_BLACK,
                     Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'q')
    {
        QueenAttack(boardDescription,
                    boardOverlay,
                    'q', x, y,
                    Constants::CP_LABELS_BLACK,
                    Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'Q')
    {
        QueenAttack(boardDescription,
                    boardOverlay,
                    'Q', x, y,
                    Constants::CP_LABELS_BLACK,
                    Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'n')
    {
        knightAttack(boardDescription,
                     boardOverlay,
                     'n', x, y,
                     Constants::CP_LABELS_BLACK,
                     Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'N')
    {
        knightAttack(boardDescription,
                     boardOverlay,
                     'N', x, y,
                     Constants::CP_LABELS_BLACK,
                     Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'k')
    {
        KingAttack (boardDescription,
                   boardOverlay,
                   'k', x, y,
                   Constants::CP_LABELS_BLACK,
                   Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'K')
    {
        KingAttack (boardDescription,
                   boardOverlay,
                   'K', x, y,
                   Constants::CP_LABELS_BLACK,
                   Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'p')
    {
        PawnAttack(boardDescription,
                   boardOverlay,
                   'p', x, y,
                   Constants::CP_LABELS_BLACK,
                   Constants::CP_LABELS_WHITE);
    }

    if (attackingPiece == 'P')
    {
        PawnAttack(boardDescription,
                   boardOverlay,
                   'P', x, y,
                   Constants::CP_LABELS_BLACK,
                   Constants::CP_LABELS_WHITE);
    }     

    return boardOverlay;
}

void OpenURL(const char* url)
{
    try {
        std::string command = std::string("x-www-browser ") + std::string(url);
        //Ubuntu only - open URL in the default browser
        system(command.c_str());

    } catch (...) {
        std::cout << "Error opening URL in browser." << std::endl;
    }
}
