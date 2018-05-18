#ifndef MESHSCENENODE_H
#define MESHSCENENODE_H

#include <irrlicht.h>

using namespace irr;

class MeshSceneNode : public scene::IMeshSceneNode
{
public:
    //! constructor
    MeshSceneNode(scene::IMesh* mesh, scene::ISceneNode* parent, scene::ISceneManager* mgr,	s32 id,
        const core::vector3df& position = core::vector3df(0,0,0),
        const core::vector3df& rotation = core::vector3df(0,0,0),
        const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

    //! destructor
    virtual ~MeshSceneNode();

    //! frame
    virtual void OnRegisterSceneNode();

    //! renders the node.
    virtual void render();

    //! returns the axis aligned bounding box of this node
    virtual const core::aabbox3d<f32>& getBoundingBox() const;

    //! returns the material based on the zero based index i. To get the amount
    //! of materials used by this scene node, use getMaterialCount().
    //! This function is needed for inserting the node into the scene hirachy on a
    //! optimal position for minimizing renderstate changes, but can also be used
    //! to directly modify the material of a scene node.
    virtual video::SMaterial& getMaterial(u32 i);

    //! returns amount of materials used by this scene node.
    virtual u32 getMaterialCount() const;

    //! Writes attributes of the scene node.
    virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

    //! Reads attributes of the scene node.
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

    //! Returns type of the scene node
    virtual scene::ESCENE_NODE_TYPE getType() const { return scene::ESNT_MESH; }

    //! Sets a new mesh
    virtual void setMesh(scene::IMesh* mesh);

    //! Returns the current mesh
    virtual scene::IMesh* getMesh(void) { return Mesh; }

    //! Creates shadow volume scene node as child of this node
    //! and returns a pointer to it.
    virtual scene::IShadowVolumeSceneNode* addShadowVolumeSceneNode(const scene::IMesh* shadowMesh,
        s32 id, bool zfailmethod=true, f32 infinity=10000.0f);

    //! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
    /* In this way it is possible to change the materials a mesh causing all mesh scene nodes
    referencing this mesh to change too. */
    virtual void setReadOnlyMaterials(bool readonly);

    //! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
    virtual bool isReadOnlyMaterials() const;

    //! Creates a clone of this scene node and its children.
    virtual ISceneNode* clone(ISceneNode* newParent=0, scene::ISceneManager* newManager=0);

    //! Removes a child from this scene node.
    //! Implemented here, to be able to remove the shadow properly, if there is one,
    //! or to remove attached childs.
    virtual bool removeChild(ISceneNode* child);

protected:

    void copyMaterials();

    core::array<video::SMaterial> Materials;
    core::aabbox3d<f32> Box;
    video::SMaterial ReadOnlyMaterial;

    scene::IMesh* Mesh;
    scene::IShadowVolumeSceneNode* Shadow;

    s32 PassCount;
    bool ReadOnlyMaterials;
};

#endif // MESHSCENENODE_H
