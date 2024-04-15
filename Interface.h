#pragma once
#include "ChessBoard.h"
#include "Constants.h"

using namespace Constants;

class Interface
{
public:
    Interface(ChessBoard& cb);

    void InitInterfaceRects();

    void SetButtonViewerTexID(std::string textureId);
    void SetButtonSimulatorTexID(std::string textureId);
    void SetButtonStartTexID(std::string textureId);
    void SetButtonStopTexID(std::string textureId);
    void ResetAllButtonsTexID();

    auto GetRectTextFEN() -> SDL_Rect*;
    auto GetRectButtonViewer() -> SDL_Rect*;
    auto GetRectButtonSimulator() -> SDL_Rect*;
    auto GetRectSliderKnob() -> SDL_Rect*;
    auto GetRectWindow() -> SDL_Rect*;


    void DrawTitle();
    void DrawModeToggleButtons();
    void DrawWindowBackground();
    void DrawFENDescription();
    void DrawStatistics();
    void DrawSlider();

    auto IsButtonClicked(const SDL_Rect* rect, int xUp, int yUp) const -> bool;

    auto IsButtonOnFocus(const SDL_Rect* rect) const -> bool;
    void SetMouseDownCoords(int x, int y);
    void UpdateBtnTexturesOnFocus();

    void setMouseOffset(const int val);
    auto getMouseOffset() const -> int;



private:

    ChessBoard& m_chessBoard;

    int m_mouseDownX{}, m_mouseDownY{};
    int m_offsetX = -1;

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


