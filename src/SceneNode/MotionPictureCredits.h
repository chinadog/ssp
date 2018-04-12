#ifndef MOTIONPICTURECREDITS_H
#define MOTIONPICTURECREDITS_H

#include <irrlicht.h>
#include <Common/SignalSlot.h>

using namespace irr;

class GamePark;

class MotionPictureCredits
{
public:
    MotionPictureCredits(GamePark* gamePark);

    void draw();
    Signal<> finished;
private:
    GamePark* m_gamePark = nullptr;
    gui::IGUIStaticText* m_text;
    f32 m_deltaTime = 0.0;
    u32 m_prevTime = 0;
    f32 m_curHeight = 0.0;

    void initNode();
    const wchar_t* text() const;
};

#endif // MOTIONPICTURECREDITS_H
