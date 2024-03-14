#include "Constants.h"

namespace Constants
{
    // window dimensions
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    // chess board size
    extern const int CB_SIZE = 640;

    // chess pieces colors
    const SDL_Color CP_LIGHT {254, 237, 211, 255};
    const SDL_Color CP_DARK {0, 0, 0 , 255};

    // chess board colors
    const SDL_Color CB_LIGHT {214, 187, 141, 255};
    const SDL_Color CB_DARK {198, 130, 66, 255};
    const SDL_Color CB_HIGHLIGHT {100, 255, 100, 50};

    // chess board string descriptions
    const std::string CB_INIT_DESCR = "rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR";
    const std::string CB_INIT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

} // namespace Constants
