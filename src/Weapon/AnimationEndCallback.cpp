#include "AnimationEndCallback.h"
#include <iostream>

AnimationEndCallback::AnimationEndCallback()
{

}

void AnimationEndCallback::OnAnimationEnd(scene::IAnimatedMeshSceneNode *node)
{
    node->setAnimationEndCallback(0);
    finished.Emit();
    finished.disconnect_all();
}
