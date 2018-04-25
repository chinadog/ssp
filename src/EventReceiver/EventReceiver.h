#ifndef MYEVENTRECEIVER_H
#define MYEVENTRECEIVER_H

#include <irrlicht.h>
#include <list>

using namespace irr;

class GamePark;

class MyEventReceiver : public IEventReceiver
{
public:

    MyEventReceiver(GamePark* gamePark);

    bool OnEvent(const SEvent& event);
     std::list <SEvent> m_slowmoEvents;
     bool m_keyW = false;
     bool m_keyA = false;
     bool m_keyS = false;
     bool m_keyD = false;
     bool m_keyShift = false;
private:
    GamePark* m_gamePark;



};


#endif // MYEVENTRECEIVER_H
