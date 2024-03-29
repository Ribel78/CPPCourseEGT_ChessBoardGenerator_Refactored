//Constants.cpp

#include "Constants.h"

namespace Constants
{
    // window dimensions
    const int DIM_WINDOW_WIDTH = 1280;
    const int DIM_WINDOW_HEIGHT = 720;
    // chess board size
    const int DIM_CB_SIZE = DIM_WINDOW_WIDTH / 2;
    const int DIM_CB_TILE_SIZE = DIM_CB_SIZE / 8;
    const int DIM_PADDING = 40;
    const int DIM_CP_TO_REMOVE = 16;

    //file paths
    const char* TTF_DEJAVUSANS = "fonts/DejaVuSans.ttf";
    const char* TTF_SEGOEPR = "fonts/segoepr.ttf";
    const char* TTF_FREESERIF = "fonts/FreeSerif.ttf";
    const char* IMG_BACKGROUND = "images/background.jpg";
    const char* IMG_CHESS_TILE = "images/chess_board.jpg";
    const char* IMG_BUTTON_START_UP = "images/button_start_up.png";
    const char* IMG_BUTTON_START_DOWN = "images/button_start_down.png";
    const char* IMG_BUTTON_STOP_UP = "images/button_stop_up.png";
    const char* IMG_BUTTON_STOP_DOWN = "images/button_stop_down.png";
    const char* IMG_BUTTON_VIEWER_UP = "images/button_viewer_up.png";
    const char* IMG_BUTTON_VIEWER_DOWN = "images/button_viewer_down.png";
    const char* IMG_BUTTON_VIEWER_DISABLED = "images/button_viewer_disabled.png";
    const char* IMG_BUTTON_SIMULATOR_UP = "images/button_simulator_up.png";
    const char* IMG_BUTTON_SIMULATOR_DOWN = "images/button_simulator_down.png";
    const char* IMG_SLIDER_SLIT = "images/slider_slit.png";
    const char* IMG_SLIDER_KNOB = "images/slider_knob.png";

    //texture IDs
    const std::string ID_BACKGROUND = "background";
    const std::string ID_CHESS_TILE = "chess_tile";
    const std::string ID_BTN_START_UP = "button_start_up";
    const std::string ID_BTN_START_DOWN = "button_start_down";
    const std::string ID_BTN_STOP_UP = "button_stop_up";
    const std::string ID_BTN_STOP_DOWN = "button_stop_down";
    const std::string ID_BTN_VIEWER_UP = "button_viewer_up";
    const std::string ID_BTN_VIEWER_DOWN = "button_viewer_down";
    const std::string ID_BTN_VIEWER_DISABLED = "button_viewer_disabled";
    const std::string ID_BTN_SIMULATOR_UP = "button_simulator_up";
    const std::string ID_BTN_SIMULATOR_DOWN = "button_simulator_down";
    const std::string ID_TXT_STATS = "textStats";
    const std::string ID_TXT_FEN = "textFEN";
    const std::string ID_TXT_TITLE_VIEWER = "textTitleViewer";
    const std::string ID_TXT_TITLE_SIMULATOR = "textTitleSimulator";
    const std::string ID_SLIDER_SLIT = "slider_slit";
    const std::string ID_SLIDER_KNOB = "slider_knob";

    // font IDs
    const std::string ID_FONT_DEJAVU = "DejaVu";
    const std::string ID_FONT_SEGOE = "Segoe";
    const std::string ID_FONT_SEGOE28 = "Segoe28";
    const std::string ID_FONT_FREESERIF = "FreeMono";


    // Colors

    const SDL_Color COL_WINDOW_BG {23, 138, 207, 255};
    const SDL_Color COL_BUTTON_BG {50, 50, 110, 255};

    // chess pieces colors
    const SDL_Color COL_CP_LIGHT {254, 237, 211, 255};
    const SDL_Color COL_CP_DARK {0, 0, 0 , 255};

    // chess board colors
    const SDL_Color COL_CB_LIGHT {150, 200, 255, 255};
    const SDL_Color COL_CB_DARK {0, 100, 150, 255};
    const SDL_Color COL_CB_HIGHLIGHT {50, 255, 180, 120};

    //text colors
    const SDL_Color COL_TXT_LIGHT {235 ,235 ,255 ,255};
    const SDL_Color COL_TXT_DARK {0 ,0 ,0 ,255};

    // chess board string descriptions
    const std::string STR_CB_INIT_DESCR = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";
    const std::string STR_CB_INIT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    const std::string STR_CB_LOOKUPREF = "KQRBNPkqrbnp";

    //chess piece black unicode
    const std::string CPB_UNICODE[6] = {"\u265A", "\u265B", "\u265C", "\u265D", "\u265E", "\u265F"};

    const char CP_LABELS_BLACK[6] {'k','q','r','b','n','p'};
    const char CP_LABELS_WHITE[6] {'K','Q','R','B','N','P'};

} // namespace Constants
