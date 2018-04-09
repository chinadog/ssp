#include "AnimationEndCallback.h"
#include <iostream>

AnimationEndCallback::AnimationEndCallback()
{

}

void AnimationEndCallback::OnAnimationEnd(scene::IAnimatedMeshSceneNode *node)
{
    std::cout << "YESSS" << node << std::endl;
    node->setAnimationEndCallback(0);
    finished.Emit();
    finished.disconnect_all();

}
