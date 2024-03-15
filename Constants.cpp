//Constants.cpp

#include "Constants.h"

namespace Constants
{
    // window dimensions
    const int DIM_WINDOW_WIDTH = 1280;
    const int DIM_WINDOW_HEIGHT = 720;

    // chess board size
    extern const int DIM_CB_SIZE = 640;

    //file paths
    const char* TTF_DEJAVUSANS = "fonts/DejaVuSans.ttf";
    const char* TTF_SEGOEPR = "fonts/segoepr.ttf";

    // Colors

    const SDL_Color COL_WINDOW_BG {23, 138, 207, 255};
    const SDL_Color COL_BUTTON_BG {50, 50, 110, 255};

    // chess pieces colors
    const SDL_Color COL_CP_LIGHT {254, 237, 211, 255};
    const SDL_Color COL_CP_DARK {0, 0, 0 , 255};

    // chess board colors
    const SDL_Color COL_CB_LIGHT {214, 187, 141, 255};
    const SDL_Color COL_CB_DARK {198, 130, 66, 255};
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
