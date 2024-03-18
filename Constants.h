#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace Constants
{
    // window dimensions
    extern const int DIM_WINDOW_WIDTH;
    extern const int DIM_WINDOW_HEIGHT;

    // chess board size
    extern const int DIM_CB_SIZE;

    // file paths
    extern const char* TTF_DEJAVUSANS;
    extern const char* TTF_SEGOEPR;
    extern const char* TEX_BACKGROUND;
    extern const char* TEX_CHESS_TILE;

    // colors

    extern const SDL_Color COL_WINDOW_BG;
    extern const SDL_Color COL_BUTTON_BG;

    // chess pieces colors
    extern const SDL_Color COL_CP_LIGHT;
    extern const SDL_Color COL_CP_DARK;

    // chess board colors
    extern const SDL_Color COL_CB_LIGHT;
    extern const SDL_Color COL_CB_DARK;
    extern const SDL_Color COL_CB_HIGHLIGHT;

    //text colors
    extern const SDL_Color COL_TXT_LIGHT;
    extern const SDL_Color COL_TXT_DARK;

    // chess board string descriptions
    extern const std::string STR_CB_INIT_DESCR;
    extern const std::string STR_CB_INIT_FEN;
    extern const std::string STR_CB_LOOKUPREF;

    //chess piece black unicode
    extern const std::string CPB_UNICODE[6];

    extern const char CP_LABELS_BLACK[6];
    extern const char CP_LABELS_WHITE[6];


} // namespace Constants
