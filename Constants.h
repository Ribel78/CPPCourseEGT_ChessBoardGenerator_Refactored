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
    extern const int DIM_CB_TILE_SIZE;
    extern const int DIM_PADDING;
    extern const int DIM_CP_TO_REMOVE;

    // file paths
    extern const char* TTF_DEJAVUSANS;
    extern const char* TTF_SEGOEPR;
    extern const char* TTF_FREESERIF;
    extern const char* IMG_BACKGROUND;
    extern const char* IMG_CHESS_TILE;
    extern const char* IMG_BUTTON_START_UP;
    extern const char* IMG_BUTTON_START_DOWN;
    extern const char* IMG_BUTTON_STOP_UP;
    extern const char* IMG_BUTTON_STOP_DOWN;
    extern const char* IMG_BUTTON_VIEWER_UP;
    extern const char* IMG_BUTTON_VIEWER_DOWN;
    extern const char* IMG_BUTTON_VIEWER_DISABLED;
    extern const char* IMG_BUTTON_SIMULATOR_UP;
    extern const char* IMG_BUTTON_SIMULATOR_DOWN;
    extern const char* IMG_SLIDER_SLIT;
    extern const char* IMG_SLIDER_KNOB;

    // texture IDs
    extern const std::string ID_BACKGROUND;
    extern const std::string ID_CHESS_TILE;
    extern const std::string ID_BTN_START_UP;
    extern const std::string ID_BTN_START_DOWN;
    extern const std::string ID_BTN_STOP_UP;
    extern const std::string ID_BTN_STOP_DOWN;
    extern const std::string ID_BTN_VIEWER_UP;
    extern const std::string ID_BTN_VIEWER_DOWN;
    extern const std::string ID_BTN_VIEWER_DISABLED;
    extern const std::string ID_BTN_SIMULATOR_UP;
    extern const std::string ID_BTN_SIMULATOR_DOWN;
    extern const std::string ID_TXT_STATS;
    extern const std::string ID_TXT_FEN;
    extern const std::string ID_TXT_TITLE_VIEWER;
    extern const std::string ID_TXT_TITLE_SIMULATOR;
    extern const std::string ID_SLIDER_SLIT;
    extern const std::string ID_SLIDER_KNOB;

    // font IDs
    extern const std::string ID_FONT_DEJAVU;
    extern const std::string ID_FONT_SEGOE;
    extern const std::string ID_FONT_SEGOE28;
    extern const std::string ID_FONT_FREESERIF;

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
