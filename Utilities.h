#pragma once

//#include <iostream>
#include <string>

void rookAttack(std::string &boardDescription,
                std::string &boardOverlay,
                char attackingPiece,
                int x, int y,
                char (&blackPieces)[6],
                char (&whitePieces)[6]);

void bishopAttack(std::string &boardDescription,
                  std::string &boardOverlay,
                  char attackingPiece,
                  int x, int y,
                  char (&blackPieces)[6],
                  char (&whitePieces)[6]);

void queenAttack(std::string &boardDescription,
                 std::string &boardOverlay,
                 char attackingPiece,
                 int x, int y,
                 char (&blackPieces)[6],
                 char (&whitePieces)[6]);

void kingAttack(std::string &boardDescription,
                std::string &boardOverlay,
                char attackingPiece,
                int x, int y,
                char (&blackPieces)[6],
                char (&whitePieces)[6]);

void pawnAttack(std::string &boardDescription,
                std::string &boardOverlay,
                char attackingPiece,
                int x, int y,
                char (&blackPieces)[6],
                char (&whitePieces)[6]);

std::string attackSquares(std::string boardDescription,
                          int x, int y,
                          char piece = '\0' );
