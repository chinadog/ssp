#ifndef MYEVENTRECEIVER_H
#define MYEVENTRECEIVER_H

#include <irrlicht.h>

using namespace irr;

class GamePark;

class MyEventReceiver : public IEventReceiver
{
public:

    MyEventReceiver(GamePark* gamePark);

    bool OnEvent(const SEvent& event);
private:
    GamePark* m_gamePark;
};


#endif // MYEVENTRECEIVER_H
