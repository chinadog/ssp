#include "GameOverCredits.h"
#include "GamePark.h"

GameOverCredits::GameOverCredits(GamePark *gamePark) :
    m_gamePark(gamePark)
{
    initNode();
    finished.connect([this](){m_text->setVisible(false);
        m_gamePark->setSceneMode(SceneMode::MainMenu);});
}

void GameOverCredits::draw()
{
    // delta time
    u32 now = m_gamePark->device()->getTimer()->getTime();
    if(m_prevTime == 0)
    {
        m_prevTime = now;
    }
    m_deltaTime = (f32)(now - m_prevTime) / 1000.f;
    m_prevTime = now;
    // end delta time

//    m_curHeight -= 80.0*m_deltaTime;
//    core::position2di pos(10,m_curHeight);
//    m_text->setRelativePosition(pos);

    m_gamePark->driver()->beginScene(true, true, 0 );
    m_gamePark->env()->drawAll();
    m_gamePark->driver()->endScene();

    if(now - m_startTime > 3000)
    {
        finished.Emit();
    }
}

void GameOverCredits::initNode()
{
    core::dimension2d<u32> size = m_gamePark->device()->getVideoDriver()->getScreenSize();
    const int lwidth = size.Width - 20;
    const int lheight = 16;
    core::rect<int> pos(0,0, size.Width, size.Height);
    m_text = m_gamePark->env()->addStaticText(text(),pos,true);
    m_text->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);
    m_curHeight = size.Height-lheight-10;
    m_startTime = m_gamePark->device()->getTimer()->getTime();
}

const wchar_t *GameOverCredits::text() const
{
    const wchar_t* text =
    L""
    "GAME OVER";
    return text;
}
