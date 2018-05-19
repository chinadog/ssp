#include "BoneSceneNode.h"

#include "IrrCompileConfig.h"


//! constructor
BoneSceneNode::BoneSceneNode(ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
    u32 boneIndex, const c8* boneName)
: IBoneSceneNode(parent, mgr, id), BoneIndex(boneIndex),
    AnimationMode(scene::EBAM_AUTOMATIC), SkinningSpace(scene::EBSS_LOCAL)
{
    #ifdef _DEBUG
    setDebugName("BoneSceneNode");
    #endif
    setName(boneName);
}


//! Returns the index of the bone
u32 BoneSceneNode::getBoneIndex() const
{
    return BoneIndex;
}


//! Sets the animation mode of the bone. Returns true if successful.
bool BoneSceneNode::setAnimationMode(scene::E_BONE_ANIMATION_MODE mode)
{
    AnimationMode = mode;
    return true;
}


//! Gets the current animation mode of the bone
scene::E_BONE_ANIMATION_MODE BoneSceneNode::getAnimationMode() const
{
    return AnimationMode;
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& BoneSceneNode::getBoundingBox() const
{
    return Box;
}


/*
//! Returns the relative transformation of the scene node.
core::matrix4 BoneSceneNode::getRelativeTransformation() const
{
    return core::matrix4(); // RelativeTransformation;
}
*/


void BoneSceneNode::OnAnimate(u32 timeMs)
{
    if (IsVisible)
    {
        // animate this node with all animators

        scene::ISceneNodeAnimatorList::Iterator ait = Animators.begin();
        for (; ait != Animators.end(); ++ait)
            (*ait)->animateNode(this, timeMs);

        // update absolute position
        //updateAbsolutePosition();

        // perform the post render process on all children
        scene::ISceneNodeList::Iterator it = Children.begin();
        for (; it != Children.end(); ++it)
            (*it)->OnAnimate(timeMs);
    }
}


void BoneSceneNode::helper_updateAbsolutePositionOfAllChildren(ISceneNode *Node)
{
    Node->updateAbsolutePosition();

    scene::ISceneNodeList::ConstIterator it = Node->getChildren().begin();
    for (; it != Node->getChildren().end(); ++it)
    {
        helper_updateAbsolutePositionOfAllChildren( (*it) );
    }
}


void BoneSceneNode::updateAbsolutePositionOfAllChildren()
{
    helper_updateAbsolutePositionOfAllChildren( this );
}


void BoneSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
    IBoneSceneNode::serializeAttributes(out, options);
    out->addInt("BoneIndex", BoneIndex);
    out->addEnum("AnimationMode", AnimationMode, scene::BoneAnimationModeNames);
}


void BoneSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
    BoneIndex = in->getAttributeAsInt("BoneIndex");
    AnimationMode = (scene::E_BONE_ANIMATION_MODE)in->getAttributeAsEnumeration("AnimationMode", scene::BoneAnimationModeNames);
    // for legacy files (before 1.5)
    const core::stringc boneName = in->getAttributeAsString("BoneName");
    setName(boneName);
    IBoneSceneNode::deserializeAttributes(in, options);
    // TODO: add/replace bone in parent with bone from mesh
}

