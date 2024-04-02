//Resources.cpp

#include "Resources.h"
#include "TextureFactory.h"
#include "Constants.h"

using namespace Constants;

void prepFonts()
{
    // Loading fonts
    TextureFactory::instance()->loadFont(TTF_DEJAVUSANS, ID_FONT_DEJAVU, 48);
    TextureFactory::instance()->loadFont(TTF_SEGOEPR, ID_FONT_SEGOE, 72);
    TextureFactory::instance()->loadFont(TTF_SEGOEPR, ID_FONT_SEGOE28, 28);
    TextureFactory::instance()->loadFont(TTF_FREESERIF, ID_FONT_FREESERIF, 72);
}

void prepStaticFontTextures()
{
    // Title textures
    TextureFactory::instance()->textureFromFont(ID_TXT_TITLE_SIMULATOR,ID_FONT_SEGOE,
                                                "Chess Board Simulator",
                                                COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH, 0);
    TextureFactory::instance()->textureFromFont(ID_TXT_TITLE_VIEWER,ID_FONT_SEGOE,
                                                "Chess Board Viewer",
                                                COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH, 0);
}

void prepStaticImageTextures()
{
    // Loading image textures
    TextureFactory::instance()->textureFromImage(IMG_BACKGROUND, ID_BACKGROUND);
    TextureFactory::instance()->textureFromImage(IMG_CHESS_TILE, ID_CHESS_TILE);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_START_UP, ID_BTN_START_UP);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_START_DOWN, ID_BTN_START_DOWN);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_STOP_UP, ID_BTN_STOP_UP);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_STOP_DOWN, ID_BTN_STOP_DOWN);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_VIEWER_UP, ID_BTN_VIEWER_UP);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_VIEWER_DOWN, ID_BTN_VIEWER_DOWN);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_VIEWER_DISABLED, ID_BTN_VIEWER_DISABLED);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_SIMULATOR_UP, ID_BTN_SIMULATOR_UP);
    TextureFactory::instance()->textureFromImage(IMG_BUTTON_SIMULATOR_DOWN, ID_BTN_SIMULATOR_DOWN);
    TextureFactory::instance()->textureFromImage(IMG_SLIDER_SLIT, ID_SLIDER_SLIT);
    TextureFactory::instance()->textureFromImage(IMG_SLIDER_KNOB, ID_SLIDER_KNOB);
}

void prepChessPieceTextures()
{
    // Create textures from font chess characters !Important id's are ASCII char values
    // to avoid overlapping with labels texture id's
    for (int i = 0; i < 12; i++)
    {
        if (i / 6 == 0)
        { // white chess glyphs - K, Q, R, B, N, P
            TextureFactory::instance()->textureFromUnicode(std::to_string(STR_CB_LOOKUPREF[i]),
                                                           ID_FONT_DEJAVU,
                                                           CPB_UNICODE[i % 6].c_str(),
                                                           COL_CP_LIGHT);
            // TextureFactory::instance()->packTexture(std::to_string(STR_CB_LOOKUPREF[i]),
            //                                         "chess_pack");
        }
        else
        { // black chess glyphs - k, q, r, b, n, p
            TextureFactory::instance()->textureFromUnicode(std::to_string(STR_CB_LOOKUPREF[i]),
                                                           ID_FONT_DEJAVU,
                                                           CPB_UNICODE[i % 6].c_str(),
                                                           COL_CP_DARK);
            // TextureFactory::instance()->packTexture(std::to_string(STR_CB_LOOKUPREF[i]),
            //                                         "chess_pack");
        }
    }
}

void prepBoardLabelsTextures()
{
    for(int i = 0; i < 8; i++)
    {
        std::string h_label = std::string(1, ('a' + i));
        TextureFactory::instance()->textureFromFont(h_label,
                                                    ID_FONT_DEJAVU,
                                                    h_label.c_str(),
                                                    (i%2)?COL_CB_DARK : COL_CB_LIGHT,
                                                    64, 32);

        std::string v_label = std::string(1, ('8' - i));
        TextureFactory::instance()->textureFromFont(v_label,
                                                    ID_FONT_DEJAVU,
                                                    v_label.c_str(),
                                                    (i%2)?COL_CB_LIGHT : COL_CB_DARK,
                                                    64, 32);
    }
}
