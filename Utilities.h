#pragma once

//#include <iostream>
#include <string>

void rookAttack(const std::string& boardDescription,
                std::string& boardOverlay,
                const char attackingPiece,
                int x, int y,
                const char (&blackPieces)[6],
                const char (&whitePieces)[6]);

void bishopAttack(const std::string& boardDescription,
                  std::string& boardOverlay,
                  const char attackingPiece,
                  int x, int y,
                  const char (&blackPieces)[6],
                  const char (&whitePieces)[6]);

void queenAttack(const std::string& boardDescription,
                 std::string& boardOverlay,
                 const char attackingPiece,
                 int x, int y,
                 const char (&blackPieces)[6],
                 const char (&whitePieces)[6]);

void kingAttack(const std::string& boardDescription,
                std::string& boardOverlay,
                const char attackingPiece,
                int x, int y,
                const char (&blackPieces)[6],
                const char (&whitePieces)[6]);

void pawnAttack(const std::string& boardDescription,
                std::string& boardOverlay,
                const char attackingPiece,
                int x, int y,
                const char (&blackPieces)[6],
                const char (&whitePieces)[6]);

std::string attackSquares(const std::string& boardDescription,
                          int x, int y,
                          char piece = '\0' );
