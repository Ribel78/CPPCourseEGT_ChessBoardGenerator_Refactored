//Resources.cpp

#include "Resources.h"
#include "TextureFactory.h"
#include "Constants.h"

using namespace Constants;

void PrepFonts()
{
    // Loading fonts
    TextureFactory::Instance()->LoadFont(TTF_DEJAVUSANS, ID_FONT_DEJAVU, 48);
    TextureFactory::Instance()->LoadFont(TTF_SEGOEPR, ID_FONT_SEGOE, 72);
    TextureFactory::Instance()->LoadFont(TTF_SEGOEPR, ID_FONT_SEGOE28, 28);
    TextureFactory::Instance()->LoadFont(TTF_FREESERIF, ID_FONT_FREESERIF, 72);
}

void PrepStaticFontTextures()
{
    // Title textures
    TextureFactory::Instance()->TextureFromFont(ID_TXT_TITLE_SIMULATOR,ID_FONT_SEGOE,
                                                "Chess Board Simulator",
                                                COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH, 0);
    TextureFactory::Instance()->TextureFromFont(ID_TXT_TITLE_VIEWER,ID_FONT_SEGOE,
                                                "Chess Board Viewer",
                                                COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH, 0);
}

void PrepStaticImageTextures()
{
    // Loading image textures
    TextureFactory::Instance()->TextureFromImage(IMG_BACKGROUND, ID_BACKGROUND);
    TextureFactory::Instance()->TextureFromImage(IMG_CHESS_TILE, ID_CHESS_TILE);
    TextureFactory::Instance()->TextureFromImage(IMG_BUTTON_START_UP, ID_BTN_START_UP);
    TextureFactory::Instance()->TextureFromImage(IMG_BUTTON_START_DOWN, ID_BTN_START_DOWN);
    TextureFactory::Instance()->TextureFromImage(IMG_BUTTON_STOP_UP, ID_BTN_STOP_UP);
    TextureFactory::Instance()->TextureFromImage(IMG_BUTTON_STOP_DOWN, ID_BTN_STOP_DOWN);
    TextureFactory::Instance()->TextureFromImage(IMG_BUTTON_VIEWER_UP, ID_BTN_VIEWER_UP);
    TextureFactory::Instance()->TextureFromImage(IMG_BUTTON_VIEWER_DOWN, ID_BTN_VIEWER_DOWN);
    TextureFactory::Instance()->TextureFromImage(IMG_BUTTON_VIEWER_DISABLED, ID_BTN_VIEWER_DISABLED);
    TextureFactory::Instance()->TextureFromImage(IMG_BUTTON_SIMULATOR_UP, ID_BTN_SIMULATOR_UP);
    TextureFactory::Instance()->TextureFromImage(IMG_BUTTON_SIMULATOR_DOWN, ID_BTN_SIMULATOR_DOWN);
    TextureFactory::Instance()->TextureFromImage(IMG_SLIDER_SLIT, ID_SLIDER_SLIT);
    TextureFactory::Instance()->TextureFromImage(IMG_SLIDER_KNOB, ID_SLIDER_KNOB);
}

//@use_texture - use images or unicode glyphs as chess piece textures
void PrepChessPieceTextures(bool useTexture)
{
    // !Important id's are ASCII char values to avoid overlapping with labels texture id's
    // Texture ID's are the same - use this function only once on initialization

    for (int i = 0; i < 12; i++)
    {
        SDL_Color color[2] = {COL_CP_LIGHT, COL_CP_DARK};

        if(useTexture)
        {
            std::string filePath{};
            filePath.append(IMG_CHESS_FIGURES);
            filePath += std::to_string(i);

            // filepath += STR_CB_LOOKUPREF[i];
            filePath.append(".png");
            TextureFactory::Instance()->TextureFromImage(filePath.c_str(), std::to_string(STR_CB_LOOKUPREF[i]));

        }
        else
        {
            TextureFactory::Instance()->TextureFromUnicode(std::to_string(STR_CB_LOOKUPREF[i]),
                                                           ID_FONT_DEJAVU,
                                                           CPB_UNICODE[i % 6].c_str(),
                                                           color[i/6]);
        }
    }
}

void PrepBoardLabelsTextures()
{
    for(int i = 0; i < 8; i++)
    {
        std::string horizontalLabel = std::string(1, ('a' + i));
        TextureFactory::Instance()->TextureFromFont(horizontalLabel,
                                                    ID_FONT_DEJAVU,
                                                    horizontalLabel.c_str(),
                                                    (i%2)?COL_CB_DARK : COL_CB_LIGHT,
                                                    64, 32);

        std::string verticalLabel = std::string(1, ('8' - i));
        TextureFactory::Instance()->TextureFromFont(verticalLabel,
                                                    ID_FONT_DEJAVU,
                                                    verticalLabel.c_str(),
                                                    (i%2)?COL_CB_LIGHT : COL_CB_DARK,
                                                    64, 32);
    }
}
