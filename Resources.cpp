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
