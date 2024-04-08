#include "Interface.h"
#include "TextureFactory.h"

Interface::Interface(ChessBoard& cb) : m_chessBoard(cb) {
    InitInterfaceRects();
}

void Interface::InitInterfaceRects()
{
    //Set up UI Rectangles

    m_rectTextFEN = {DIM_PADDING / 2, DIM_WINDOW_WIDTH / 2 + 10, DIM_WINDOW_WIDTH / 2 - DIM_PADDING, DIM_CB_TILE_SIZE - DIM_PADDING / 2};
    m_rectTextStats = {DIM_WINDOW_WIDTH / 2 + DIM_PADDING, DIM_CB_TILE_SIZE + DIM_PADDING , DIM_WINDOW_WIDTH / 2 - 2*DIM_PADDING, DIM_WINDOW_HEIGHT / 3};
    m_rectButtonSimulator = {DIM_WINDOW_WIDTH / 2 + DIM_PADDING, DIM_WINDOW_WIDTH / 2 - DIM_CB_TILE_SIZE, (DIM_WINDOW_WIDTH / 2 - 3*DIM_PADDING)/2, DIM_CB_TILE_SIZE};
    m_rectButtonViewer = {DIM_WINDOW_WIDTH / 2 + 2*DIM_PADDING + (DIM_WINDOW_WIDTH / 2 - 3*DIM_PADDING)/2, DIM_WINDOW_WIDTH / 2 - DIM_CB_TILE_SIZE, (DIM_WINDOW_WIDTH / 2 - 3*DIM_PADDING)/2, DIM_CB_TILE_SIZE};
    m_rectTextTitle = {DIM_WINDOW_WIDTH / 2 + DIM_PADDING, DIM_PADDING / 2, DIM_WINDOW_WIDTH / 2 - 2*DIM_PADDING, DIM_CB_TILE_SIZE};
    m_rectWindow = {0, 0, DIM_WINDOW_WIDTH, DIM_WINDOW_HEIGHT};
    m_rectSliderSlit = {DIM_WINDOW_WIDTH / 2 + DIM_PADDING, DIM_WINDOW_WIDTH / 2 - (5 * DIM_CB_TILE_SIZE / 2), DIM_WINDOW_WIDTH / 2 - 2*DIM_PADDING, DIM_CB_TILE_SIZE};
    m_rectSliderKnob = {m_rectSliderSlit.x + (DIM_CP_TO_REMOVE - 3) *((m_rectSliderSlit.w)/ 33), m_rectSliderSlit.y, m_rectSliderSlit.h, m_rectSliderSlit.h};
}

void Interface::SetButtonViewerTexID(std::string textureId)
{
    m_buttonViewerTexID = textureId;
}

void Interface::SetButtonSimulatorTexID(std::string textureId)
{
    m_buttonSimulatorTexID = textureId;
}

void Interface::SetButtonStartTexID(std::string textureId)
{
    m_buttonStartTexID = textureId;
}

void Interface::SetButtonStopTexID(std::string textureId)
{
    m_buttonStopTexID = textureId;
}

void Interface::ResetAllButtonsTexID()
{
    SetButtonViewerTexID(ID_BTN_VIEWER_UP);
    SetButtonSimulatorTexID(ID_BTN_SIMULATOR_UP);
    SetButtonStartTexID(ID_BTN_START_UP);
    SetButtonStopTexID(ID_BTN_STOP_UP);

}

auto Interface::GetRectTextFEN() -> SDL_Rect*
{
    return &m_rectTextFEN;
}

auto Interface::GetRectButtonViewer() -> SDL_Rect*
{
    return &m_rectButtonViewer;
}

auto Interface::GetRectButtonSimulator() -> SDL_Rect*
{
    return &m_rectButtonSimulator;
}


auto Interface::GetRectSliderKnob() -> SDL_Rect*
{
    return &m_rectSliderKnob;
}

auto Interface::GetRectWindow() -> SDL_Rect*
{
    return &m_rectWindow;
}

void Interface::DrawTitle()
{
    if(m_chessBoard.IsViewing())
        TextureFactory::Instance()->DrawTexture(ID_TXT_TITLE_VIEWER, NULL, &m_rectTextTitle);
    else
        TextureFactory::Instance()->DrawTexture(ID_TXT_TITLE_SIMULATOR, NULL, &m_rectTextTitle);

}

void Interface::DrawModeToggleButtons()
{
    if (!m_chessBoard.IsViewing())
    {
        if (m_chessBoard.IsSimulating())
        {
            TextureFactory::Instance()->DrawTexture(m_buttonStopTexID, NULL, &m_rectButtonSimulator);
            TextureFactory::Instance()->DrawTexture(ID_BTN_VIEWER_DISABLED, NULL, &m_rectButtonViewer);
        }
        else
        {
            TextureFactory::Instance()->DrawTexture(m_buttonStartTexID, NULL, &m_rectButtonSimulator);
            TextureFactory::Instance()->DrawTexture(m_buttonViewerTexID, NULL, &m_rectButtonViewer);
        }
    }
    else
    {
        TextureFactory::Instance()->DrawTexture(m_buttonSimulatorTexID, NULL, &m_rectButtonViewer);
    }

}

void Interface::DrawWindowBackground()
{
    TextureFactory::Instance()->DrawTexture(ID_BACKGROUND, NULL, &m_rectWindow);
}

void Interface::DrawFENDescription()
{
    std::string dynamicFEN {};
    if (!m_chessBoard.IsViewing())
    {
        dynamicFEN = m_chessBoard.GetMutableDescriptionsQueue().back().m_fenDescription;
    }
    else
    {
        m_chessBoard.SetBoardDescriptionFromVector();
        dynamicFEN = m_chessBoard.GetCurrentDescription().m_fenDescription;
    }
    TextureFactory::Instance()->TextureFromFont(ID_TXT_FEN,
                                                ID_FONT_SEGOE28,
                                                dynamicFEN.c_str(),
                                                Constants::COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH, 0);

    TextureFactory::Instance()->DrawTexture(ID_TXT_FEN,
                                            NULL, &m_rectTextFEN);

    TextureFactory::Instance()->DestroyTexture(ID_TXT_FEN);

}

void Interface::DrawStatistics()
{
    // Statistics for the simulation time
    std::string dynamicText = m_chessBoard.GetSimulationSummary();

    TextureFactory::Instance()->TextureFromFont(ID_TXT_STATS,
                                                ID_FONT_SEGOE28,
                                                dynamicText.c_str(),
                                                Constants::COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH / 2, 0);

    TextureFactory::Instance()->DrawTexture(ID_TXT_STATS,
                                            NULL, &m_rectTextStats);

    TextureFactory::Instance()->DestroyTexture(ID_TXT_STATS);
}

void Interface::DrawSlider()
{
    if(!m_chessBoard.IsViewing())
    {
        TextureFactory::Instance()->DrawTexture(ID_SLIDER_SLIT, NULL, &m_rectSliderSlit);
        TextureFactory::Instance()->DrawTexture(ID_SLIDER_KNOB, NULL, &m_rectSliderKnob);
        if (m_offsetX >= 0)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            m_rectSliderKnob.x = mouseX - m_offsetX;
            if (m_rectSliderKnob.x < m_rectSliderSlit.x)
                m_rectSliderKnob.x = m_rectSliderSlit.x;
            if (m_rectSliderKnob.x > m_rectSliderSlit.x + (m_rectSliderSlit.w - m_rectSliderKnob.w))
                m_rectSliderKnob.x = m_rectSliderSlit.x + (m_rectSliderSlit.w - m_rectSliderKnob.w);
        }
        int  val = (
                      (
                          (m_rectSliderKnob.x - m_rectSliderSlit.x)/
                          ((m_rectSliderSlit.w)/ 33)
                          ) % 34
                      )+3;

        (val > 32)? val = 32 : val;

        m_chessBoard.SetPiecesToRemove(32-val);

        SDL_Rect label = m_rectSliderSlit;
        label.h /= 2;
        label.y = m_rectSliderSlit.y - label.h;
        label.w /= 2;

        std::string labelText = "Set chess pieces: ";

        (val < 10)? labelText += " " : labelText;

        labelText.append(std::to_string(val));

        TextureFactory::Instance()->TextureFromFont(
            "label", Constants::ID_FONT_SEGOE,
            labelText.c_str(),
            Constants::COL_TXT_LIGHT, 1280, 0);
        TextureFactory::Instance()->DrawTexture("label", NULL, &label);
        TextureFactory::Instance()->DestroyTexture("label");
    }
}


auto Interface::IsButtonClicked(const SDL_Rect* rect, int xUp, int yUp) const -> bool
{
    if(((m_mouseDownX > rect->x) && (m_mouseDownX < rect->x + rect->w)) &&
        ((xUp > rect->x) && (xUp < rect->x + rect->w))&&
        ((m_mouseDownY > rect->y) && (m_mouseDownY < rect->y + rect->h)) &&
        ((yUp > rect->y) && (yUp < rect->y + rect->h)))
    {
        return true; //click is inside  SDL_Rect r
    }
    return false; //click is outside SDL_Rect r
}

auto Interface::IsButtonOnFocus(const SDL_Rect* rect) const -> bool
{
    if(((m_mouseDownX > rect->x) && (m_mouseDownX < rect->x +rect->w)) &&
        ((m_mouseDownY > rect->y) && (m_mouseDownY < rect->y +rect->h)))
    {
        return true; //click coordinates inside  SDL_Rect r
    }
    return false; //click coordinates outside inside  SDL_Rect r
}

void Interface::SetMouseDownCoords(int x, int y)
{
    m_mouseDownX = x;
    m_mouseDownY = y;
}

void Interface::UpdateBtnTexturesOnFocus()
{

    if(IsButtonOnFocus(GetRectSliderKnob()))
    {
        m_offsetX = m_mouseDownX - GetRectSliderKnob()->x;
    }

    if(IsButtonOnFocus(GetRectButtonViewer()))
    {
        SetButtonSimulatorTexID(Constants::ID_BTN_SIMULATOR_DOWN);
        SetButtonViewerTexID(Constants::ID_BTN_VIEWER_DOWN);
    }

    if(IsButtonOnFocus(GetRectButtonSimulator()))
    {
        SetButtonStartTexID(Constants::ID_BTN_START_DOWN);
        SetButtonStopTexID(Constants::ID_BTN_STOP_DOWN);
    }
}

void Interface::setMouseOffset(const int val)
{
    m_offsetX = val;
}
auto Interface::getMouseOffset() const -> int
{
    return m_offsetX;
}
