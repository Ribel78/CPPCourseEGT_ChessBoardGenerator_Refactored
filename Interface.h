#pragma once
#include "ChessBoard.h"
#include "Constants.h"

using namespace Constants;

class Interface
{
public:
    Interface(ChessBoard& cb);

    void initInterfaceRects();

    void setButtonViewerTexID(std::string texture_id);
    void setButtonSimulatorTexID(std::string texture_id);
    void setButtonStartTexID(std::string texture_id);
    void setButtonStopTexID(std::string texture_id);
    void resetAllButtonsTexID();

    auto getRectTextFEN() -> SDL_Rect*;
    auto getRectButtonViewer() -> SDL_Rect*;
    auto getRectButtonSimulator() -> SDL_Rect*;
    auto getRectSliderKnob() -> SDL_Rect*;
    auto getRectWindow() -> SDL_Rect*;


    void drawTitle();
    void drawModeToggleButtons();
    void drawWindowBackground();
    void drawFENDescription();
    void drawStatistics();
    void drawSlider();

    bool isButtonClicked(const SDL_Rect* r, int xUp, int yUp) const;
    auto buttonFocus(const SDL_Rect* r) const -> bool;
    void setMouseDownCoords(int x, int y);
    void updateBtnTexturesOnFocus();

    // TODO move as private
    int m_offsetX = -1;

private:

    ChessBoard& m_chessBoard;

    int m_mouseDownX{}, m_mouseDownY{};

    std::string m_buttonViewerTexID = ID_BTN_VIEWER_UP;
    std::string m_buttonSimulatorTexID = ID_BTN_SIMULATOR_UP;
    std::string m_buttonStartTexID = ID_BTN_START_UP;
    std::string m_buttonStopTexID = ID_BTN_STOP_UP;

    SDL_Rect m_rectTextFEN;
    SDL_Rect m_rectTextStats;
    SDL_Rect m_rectButtonSimulator;
    SDL_Rect m_rectButtonViewer;
    SDL_Rect m_rectTextTitle;
    SDL_Rect m_rectWindow;
    SDL_Rect m_rectSliderSlit;
    SDL_Rect m_rectSliderKnob;
};


