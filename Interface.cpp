#include "Interface.h"
#include "TextureFactory.h"

Interface::Interface(ChessBoard& cb) : m_chessBoard(cb) {
    initInterfaceRects();
}

void Interface::initInterfaceRects()
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

void Interface::setButtonViewerTexID(std::string texture_id)
{
    m_buttonViewerTexID = texture_id;
}

void Interface::setButtonSimulatorTexID(std::string texture_id)
{
    m_buttonSimulatorTexID = texture_id;
}

void Interface::setButtonStartTexID(std::string texture_id)
{
    m_buttonStartTexID = texture_id;
}

void Interface::setButtonStopTexID(std::string texture_id)
{
    m_buttonStopTexID = texture_id;
}

void Interface::resetAllButtonsTexID()
{
    setButtonViewerTexID(ID_BTN_VIEWER_UP);
    setButtonSimulatorTexID(ID_BTN_SIMULATOR_UP);
    setButtonStartTexID(ID_BTN_START_UP);
    setButtonStopTexID(ID_BTN_STOP_UP);

}

auto Interface::getRectTextFEN() -> SDL_Rect*
{
    return &m_rectTextFEN;
}

auto Interface::getRectButtonViewer() -> SDL_Rect*
{
    return &m_rectButtonViewer;
}

auto Interface::getRectButtonSimulator() -> SDL_Rect*
{
    return &m_rectButtonSimulator;
}


auto Interface::getRectSliderKnob() -> SDL_Rect*
{
    return &m_rectSliderKnob;
}

auto Interface::getRectWindow() -> SDL_Rect*
{
    return &m_rectWindow;
}

void Interface::drawTitle()
{
    if(m_chessBoard.isViewing())
        TextureFactory::instance()->drawTexture(ID_TXT_TITLE_VIEWER, NULL, &m_rectTextTitle);
    else
        TextureFactory::instance()->drawTexture(ID_TXT_TITLE_SIMULATOR, NULL, &m_rectTextTitle);

}

void Interface::drawModeToggleButtons()
{
    if (!m_chessBoard.isViewing())
    {
        if (m_chessBoard.isSimulating())
        {
            TextureFactory::instance()->drawTexture(m_buttonStopTexID, NULL, &m_rectButtonSimulator);
            TextureFactory::instance()->drawTexture(ID_BTN_VIEWER_DISABLED, NULL, &m_rectButtonViewer);
        }
        else
        {
            TextureFactory::instance()->drawTexture(m_buttonStartTexID, NULL, &m_rectButtonSimulator);
            TextureFactory::instance()->drawTexture(m_buttonViewerTexID, NULL, &m_rectButtonViewer);
        }
    }
    else
    {
        TextureFactory::instance()->drawTexture(m_buttonSimulatorTexID, NULL, &m_rectButtonViewer);
    }

}

void Interface::drawWindowBackground()
{
    TextureFactory::instance()->drawTexture(ID_BACKGROUND, NULL, &m_rectWindow);
}

void Interface::drawFENDescription()
{
    std::string dynamic_fen {};
    if (!m_chessBoard.isViewing())
    {
        dynamic_fen = m_chessBoard.getMutableDescriptionsQueue().back().FEN;
    }
    else
    {
        m_chessBoard.setBoardDescriptionFromVector();
        dynamic_fen = m_chessBoard.getCurrentDescription().FEN;
    }
    TextureFactory::instance()->textureFromFont(ID_TXT_FEN,
                                                ID_FONT_SEGOE28,
                                                dynamic_fen.c_str(),
                                                Constants::COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH, 0);

    TextureFactory::instance()->drawTexture(ID_TXT_FEN,
                                            NULL, &m_rectTextFEN);

    TextureFactory::instance()->destroyTexture(ID_TXT_FEN);

}

void Interface::drawStatistics()
{
    // Statistics for the simulation time
    std::string dynamic_text = m_chessBoard.getSimulationSummary();

    TextureFactory::instance()->textureFromFont(ID_TXT_STATS,
                                                ID_FONT_SEGOE28,
                                                dynamic_text.c_str(),
                                                Constants::COL_TXT_LIGHT,
                                                DIM_WINDOW_WIDTH / 2, 0);

    TextureFactory::instance()->drawTexture(ID_TXT_STATS,
                                            NULL, &m_rectTextStats);

    TextureFactory::instance()->destroyTexture(ID_TXT_STATS);
}

void Interface::drawSlider()
{
    if(!m_chessBoard.isViewing())
    {
        TextureFactory::instance()->drawTexture(ID_SLIDER_SLIT, NULL, &m_rectSliderSlit);
        TextureFactory::instance()->drawTexture(ID_SLIDER_KNOB, NULL, &m_rectSliderKnob);
        if (m_offsetX >= 0)
        {
            int msx, msy;
            SDL_GetMouseState(&msx, &msy);
            m_rectSliderKnob.x = msx - m_offsetX;
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

        m_chessBoard.setPiecesToRemove(32-val);

        SDL_Rect label = m_rectSliderSlit;
        label.h /= 2;
        label.y = m_rectSliderSlit.y - label.h;
        label.w /= 2;

        std::string label_text = "Set chess pieces: ";

        (val < 10)? label_text += " " : label_text;

        label_text.append(std::to_string(val));

        TextureFactory::instance()->textureFromFont(
            "label", Constants::ID_FONT_SEGOE,
            label_text.c_str(),
            Constants::COL_TXT_LIGHT, 1280, 0);
        TextureFactory::instance()->drawTexture("label", NULL, &label);
        TextureFactory::instance()->destroyTexture("label");
    }
}


bool Interface::isButtonClicked(const SDL_Rect* r, int xUp, int yUp) const
{
    if(((m_mouseDownX > r->x) && (m_mouseDownX < r->x +r->w)) &&
        ((xUp > r->x) && (xUp < r->x +r->w))&&
        ((m_mouseDownY > r->y) && (m_mouseDownY < r->y +r->h)) &&
        ((yUp > r->y) && (yUp < r->y +r->h)))
    {
        return true; //click coordinates inside  SDL_Rect r
    }
    return false; //click coordinates outside inside  SDL_Rect r
}

auto Interface::buttonFocus(const SDL_Rect* r) const -> bool
{
    if(((m_mouseDownX > r->x) && (m_mouseDownX < r->x +r->w)) &&
        ((m_mouseDownY > r->y) && (m_mouseDownY < r->y +r->h)))
    {
        return true; //click coordinates inside  SDL_Rect r
    }
    return false; //click coordinates outside inside  SDL_Rect r
}

void Interface::setMouseDownCoords(int x, int y)
{
    m_mouseDownX = x;
    m_mouseDownY = y;
}

void Interface::updateBtnTexturesOnFocus()
{

    if(buttonFocus(getRectSliderKnob()))
    {
        m_offsetX = m_mouseDownX - getRectSliderKnob()->x;
    }

    if(buttonFocus(getRectButtonViewer()))
    {
        setButtonSimulatorTexID(Constants::ID_BTN_SIMULATOR_DOWN);
        setButtonViewerTexID(Constants::ID_BTN_VIEWER_DOWN);
    }

    if(buttonFocus(getRectButtonSimulator()))
    {
        setButtonStartTexID(Constants::ID_BTN_START_DOWN);
        setButtonStopTexID(Constants::ID_BTN_STOP_DOWN);
    }
}
