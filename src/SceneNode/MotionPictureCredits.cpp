#include "MotionPictureCredits.h"
#include "GamePark.h"

MotionPictureCredits::MotionPictureCredits(GamePark *gamePark) :
    m_gamePark(gamePark)
{
    initNode();
    finished.connect([this](){m_text->setVisible(false);
        m_gamePark->setSceneMode(SceneMode::MainMenu);});
}

void MotionPictureCredits::draw()
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

    m_curHeight -= 80.0*m_deltaTime;
    core::position2di pos(10,m_curHeight);
    m_text->setRelativePosition(pos);

    m_gamePark->driver()->beginScene(true, true, 0 );
    m_gamePark->env()->drawAll();
    m_gamePark->driver()->endScene();

    if(m_curHeight < -700)
    {
        finished.Emit();
    }
}

void MotionPictureCredits::initNode()
{
    core::dimension2d<u32> size = m_gamePark->device()->getVideoDriver()->getScreenSize();
    const int lwidth = size.Width - 20;
    const int lheight = 16;
    core::rect<int> pos(10, size.Height-lheight-10, 10+lwidth, size.Height*2);
    m_text = m_gamePark->env()->addStaticText(text(),pos,false);
    m_text->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);
    m_curHeight = size.Height-lheight-10;

}

const wchar_t *MotionPictureCredits::text() const
{
    const wchar_t* text =
    L""
    "GAME DESIGN\nEvroLittleBoy\n\n"
    "GAME DIRECTOR & EXECUTIVE PRODUCER\nEvroLittleBoy\n\n"
    "LEAD DESIGNER & WRITER\nTaoScus\nEvroLittleBoy\n\n"
    "LEAD ARTIST\nEvroLittleBoy\n\n"
    "PRODUCTION DIRECTOR\nEvroLittleBoy\n\n"
    "LEAD PRODUCER\nEvroLittleBoy\n\n"
    "TECHNICAL DIRECTOR\nEvroLittleBoy\n\n"
    "LEAD PROGRAMMER\nEvroLittleBoy\n\n"
    "LEAD ANIMATOR\nEvroLittleBoy\n\n"
    "LEAD LEVEL DESIGNER\nEvroLittleBoy\n\n"
    "AUDIO DIRECTOR\nEvroLittleBoy\n\n"
    "PRODUCERS\nEvroLittleBoy\n\n"
    "CONCEPT ART\nEvroLittleBoy\n\n"
    "WORLD ART\nEvroLittleBoy";
    return text;
}
