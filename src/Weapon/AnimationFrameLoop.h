#ifndef ANIMATIONFRAMELOOP_H
#define ANIMATIONFRAMELOOP_H

#include <irrlicht.h>

using namespace irr;

class AnimationFrameLoop
{
public:
    AnimationFrameLoop();

    inline int  start() const {return m_start;}
    inline int  end() const {return m_end;}
    inline int  speed() const {return m_speed;}
    inline bool loop() const {return m_loop;}
    inline scene::IAnimationEndCallBack* endCallback() const {return m_endCallback;}

    inline void setStart(int value) {m_start = value;}
    inline void setEnd(int value) {m_end = value;}
    inline void setSpeed(int value) {m_speed = value;}
    inline void setLoop(bool value) {m_loop = value;}
    inline void setEndCallback(scene::IAnimationEndCallBack* value) {m_endCallback = value;}
private:
    int m_start = 0;
    int m_end   = 0;
    int m_speed = 0;
    bool m_loop = false;
    scene::IAnimationEndCallBack* m_endCallback = nullptr;
};

#endif // ANIMATIONFRAMELOOP_H
