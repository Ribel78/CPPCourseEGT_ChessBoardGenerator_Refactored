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

    // chess pieces colors
    extern const SDL_Color CP_LIGHT;
    extern const SDL_Color CP_DARK;

    // chess board colors
    extern const SDL_Color CB_LIGHT;
    extern const SDL_Color CB_DARK;
    extern const SDL_Color CB_HIGHLIGHT;

    // chess board string descriptions
    extern const std::string CB_INIT_DESCR;
    extern const std::string CB_INIT_FEN;



} // namespace Constants
