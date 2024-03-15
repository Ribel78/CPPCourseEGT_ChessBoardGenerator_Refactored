#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace Constants
{
    // window dimensions
    extern const int WINDOW_WIDTH;
    extern const int WINDOW_HEIGHT;

    // chess board size
    extern const int CB_SIZE;

    //font file paths
    extern const char* TTF_DEJAVUSANS;
    extern const char* TTF_SEGOEPR;

    // Colors

    extern const SDL_Color BG_COL_WINDOW;
    extern const SDL_Color BG_COL_BUTTON;

    // chess pieces colors
    extern const SDL_Color CP_LIGHT;
    extern const SDL_Color CP_DARK;

    // chess board colors
    extern const SDL_Color CB_LIGHT;
    extern const SDL_Color CB_DARK;
    extern const SDL_Color CB_HIGHLIGHT;

    //text colors
    extern const SDL_Color TXT_LIGHT;
    extern const SDL_Color TXT_DARK;

    // chess board string descriptions
    extern const std::string CB_INIT_DESCR;
    extern const std::string CB_INIT_FEN;
    extern const std::string CB_LOOKUPREF;

    extern const char CP_LABELS_BLACK[6];
    extern const char CP_LABELS_WHITE[6];


} // namespace Constants
