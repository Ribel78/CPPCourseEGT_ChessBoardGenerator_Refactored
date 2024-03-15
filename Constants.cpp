#include "Constants.h"

namespace Constants
{
    // window dimensions
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    // chess board size
    extern const int CB_SIZE = 640;

    //font file paths
    const char* TTF_DEJAVUSANS = "fonts/DejaVuSans.ttf";
    const char* TTF_SEGOEPR = "fonts/segoepr.ttf";

    // Colors

    const SDL_Color BG_COL_WINDOW {23, 138, 207, 255};
    const SDL_Color BG_COL_BUTTON {50, 50, 110, 255};

    // chess pieces colors
    const SDL_Color CP_LIGHT {254, 237, 211, 255};
    const SDL_Color CP_DARK {0, 0, 0 , 255};

    // chess board colors
    const SDL_Color CB_LIGHT {214, 187, 141, 255};
    const SDL_Color CB_DARK {198, 130, 66, 255};
    const SDL_Color CB_HIGHLIGHT {100, 255, 100, 50};

    //text colors
    const SDL_Color TXT_LIGHT {235 ,235 ,255 ,255};
    const SDL_Color TXT_DARK {0 ,0 ,0 ,255};

    // chess board string descriptions
    const std::string CB_INIT_DESCR = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";
    const std::string CB_INIT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    const std::string CB_LOOKUPREF = "KQRBNPkqrbnp";

    const char CP_LABELS_BLACK[6] {'k','q','r','b','n','p'};
    const char CP_LABELS_WHITE[6] {'K','Q','R','B','N','P'};

} // namespace Constants
