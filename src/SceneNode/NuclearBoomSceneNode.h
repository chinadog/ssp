#ifndef NUCLEARBOOMSCENENODE_H
#define NUCLEARBOOMSCENENODE_H

#include <irrlicht.h>
#include "Common/SignalSlot.h"

using namespace irr;

class GamePark;

class NuclearBoomSceneNode : public scene::IBillboardSceneNode
{
public:
    NuclearBoomSceneNode(ISceneNode* parent, scene::ISceneManager* mgr, GamePark* gamePark, s32 id,
                         const core::vector3df& position, const core::dimension2d<f32>& size,
                         video::SColor colorTop=video::SColor(0xFFFFFFFF),
                         video::SColor colorBottom=video::SColor(0xFFFFFFFF));
    ~NuclearBoomSceneNode();


    //! Sets the size of the billboard, making it rectangular.
    void setSize(const core::dimension2d<f32>& size);

    //! Sets the size of the billboard with independent widths of the bottom and top edges.
    /** \param[in] height The height of the billboard.
    \param[in] bottomEdgeWidth The width of the bottom edge of the billboard.
    \param[in] topEdgeWidth The width of the top edge of the billboard.
    */
    void setSize(f32 height, f32 bottomEdgeWidth, f32 topEdgeWidth);

    //! Returns the size of the billboard.
    /** This will return the width of the bottom edge of the billboard.
    Use getWidths() to retrieve the bottom and top edges independently.
    \return Size of the billboard.
    */
    const core::dimension2d<f32>& getSize() const;

    //! Gets the size of the the billboard and handles independent top and bottom edge widths correctly.
    /** \param[out] height The height of the billboard.
    \param[out] bottomEdgeWidth The width of the bottom edge of the billboard.
    \param[out] topEdgeWidth The width of the top edge of the billboard.
    */
    void getSize(f32& height, f32& bottomEdgeWidth, f32& topEdgeWidth) const;

    //! Set the color of all vertices of the billboard
    /** \param[in] overallColor Color to set */
    void setColor(const video::SColor& overallColor);

    //! Set the color of the top and bottom vertices of the billboard
    /** \param[in] topColor Color to set the top vertices
    \param[in] bottomColor Color to set the bottom vertices */
    void setColor(const video::SColor& topColor,
            const video::SColor& bottomColor);

    //! Gets the color of the top and bottom vertices of the billboard
    /** \param[out] topColor Stores the color of the top vertices
    \param[out] bottomColor Stores the color of the bottom vertices */
    void getColor(video::SColor& topColor,
            video::SColor& bottomColor) const;

    //! Renders the node.
    void render();

    //! OnAnimate() is called just before rendering the whole scene.
    /** Nodes may calculate or store animations here, and may do other useful things,
    depending on what they are. Also, OnAnimate() should be called for all
    child scene nodes here. This method will be called once per frame, independent
    of whether the scene node is visible or not.
    \param timeMs Current time in milliseconds. */
    void OnAnimate(u32 timeMs);

    //! Get the axis aligned, not transformed bounding box of this node.
    /** This means that if this node is an animated 3d character,
    moving in a room, the bounding box will always be around the
    origin. To get the box in real world coordinates, just
    transform it with the matrix you receive with
    getAbsoluteTransformation() or simply use
    getTransformedBoundingBox(), which does the same.
    \return The non-transformed bounding box. */
    const core::aabbox3d<f32>& getBoundingBox() const;
    void OnRegisterSceneNode();
    video::SMaterial &getMaterial(u32 i);
    u32 getMaterialCount() const;

    void start();
    void stop();
    Signal<> finished;
    void setFrame(int frame);
private:

    //! Size.Width is the bottom edge width
    core::dimension2d<f32> Size;
    f32 TopEdgeWidth;
    core::aabbox3d<f32> BBox;
    video::SMaterial Material;

    video::S3DVertex vertices[4];
    u16 indices[6];

    GamePark* m_gamePark;
    core::stringc m_animTexturePath;
    u32 m_boomStartTime = 0;
    int m_frame = 0;
};

#endif // NUCLEARBOOMSCENENODE_H
