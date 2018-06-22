#ifndef SHOOTANIMATIONENDCALLBACK_H
#define SHOOTANIMATIONENDCALLBACK_H

#include <irrlicht.h>
#include "Common/SignalSlot.h"

using namespace irr;

class AnimationEndCallback : public scene::IAnimationEndCallBack
{
public:
    AnimationEndCallback();
    Signal<> finished;

    template <typename T>
    void setFunc(T *inst, void (T::*func)(void))
    {
        finished.connect_member(inst,func);
    }

    void OnAnimationEnd(scene::IAnimatedMeshSceneNode *node) override;

};

#endif // SHOOTANIMATIONENDCALLBACK_H
