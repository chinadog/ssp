#ifndef ATOMICBOOM_H
#define ATOMICBOOM_H

#include <irrlicht.h>
#include <Common/SignalSlot.h>

using namespace irr;

class GamePark;

class AtomicBoom
{
public:
    AtomicBoom();

    void setParent(GamePark* value);
    void start();
    void updateFrame();
    void stop();
    inline bool isBoom() const {return !!m_boomStartTime;}
    Signal<> finished;
private:
    u32 m_boomStartTime = 0;
    GamePark* m_game;
    IrrlichtDevice* m_device = nullptr;
    scene::IBillboardSceneNode* m_boomNode;
    core::stringc m_animTexturePath;

    void setFrame(u32 frame);
};

#endif // ATOMICBOOM_H
