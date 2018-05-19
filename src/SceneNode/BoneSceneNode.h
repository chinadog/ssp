#ifndef BONESCENENODE_H
#define BONESCENENODE_H

// Used with SkinnedMesh and IAnimatedMeshSceneNode, for boned meshes

#include "IBoneSceneNode.h"
#include <irrlicht.h>
using namespace irr;


class BoneSceneNode : public scene::IBoneSceneNode
{
public:

    //! constructor
    BoneSceneNode(ISceneNode* parent, scene::ISceneManager* mgr,
        s32 id=-1, u32 boneIndex=0, const c8* boneName=0);

    //! Returns the index of the bone
    virtual u32 getBoneIndex() const;

    //! Sets the animation mode of the bone. Returns true if successful.
    virtual bool setAnimationMode(scene::E_BONE_ANIMATION_MODE mode);

    //! Gets the current animation mode of the bone
    virtual scene::E_BONE_ANIMATION_MODE getAnimationMode() const;

    //! returns the axis aligned bounding box of this node
    virtual const core::aabbox3d<f32>& getBoundingBox() const;

    /*
    //! Returns the relative transformation of the scene node.
    //virtual core::matrix4 getRelativeTransformation() const;
    */

    virtual void OnAnimate(u32 timeMs);

    virtual void updateAbsolutePositionOfAllChildren();

    //! Writes attributes of the scene node.
    virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

    //! Reads attributes of the scene node.
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

    //! How the relative transformation of the bone is used
    virtual void setSkinningSpace( scene::E_BONE_SKINNING_SPACE space )
    {
        SkinningSpace=space;
    }

    virtual scene::E_BONE_SKINNING_SPACE getSkinningSpace() const
    {
        return SkinningSpace;
    }

private:
    void helper_updateAbsolutePositionOfAllChildren(ISceneNode *Node);

    u32 BoneIndex;

    core::aabbox3d<f32> Box;

    scene::E_BONE_ANIMATION_MODE AnimationMode;
    scene::E_BONE_SKINNING_SPACE SkinningSpace;
};


#endif // BONESCENENODE_H
