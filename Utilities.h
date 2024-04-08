#pragma once

#include <string>

void RookAttack(const std::string& boardDescription,
                std::string& boardOverlay,
                const char attackingPiece,
                int x, int y,
                const char (&blackPieces)[6],
                const char (&whitePieces)[6]);

void BishopAttack(const std::string& boardDescription,
                  std::string& boardOverlay,
                  const char attackingPiece,
                  int x, int y,
                  const char (&blackPieces)[6],
                  const char (&whitePieces)[6]);

void QueenAttack(const std::string& boardDescription,
                 std::string& boardOverlay,
                 const char attackingPiece,
                 int x, int y,
                 const char (&blackPieces)[6],
                 const char (&whitePieces)[6]);

void KingAttack(const std::string& boardDescription,
                std::string& boardOverlay,
                const char attackingPiece,
                int x, int y,
                const char (&blackPieces)[6],
                const char (&whitePieces)[6]);

void PawnAttack(const std::string& boardDescription,
                std::string& boardOverlay,
                const char attackingPiece,
                int x, int y,
                const char (&blackPieces)[6],
                const char (&whitePieces)[6]);

std::string AttackSquares(const std::string& boardDescription,
                          int x, int y,
                          char piece = '\0' );

void OpenURL(const char* url);

