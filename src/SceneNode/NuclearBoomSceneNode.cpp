#include "NuclearBoomSceneNode.h"
#include "Common/Common.h"
#include "GamePark.h"

NuclearBoomSceneNode::NuclearBoomSceneNode(scene::ISceneNode *parent, scene::ISceneManager *mgr,
                                           GamePark* gamePark,
                                           s32 id,
                                           const core::vector3df &position, const core::dimension2d<f32> &size,
                                           video::SColor colorTop, video::SColor colorBottom) :
    IBillboardSceneNode(parent,mgr,id,position),
    m_gamePark(gamePark)
{
    #ifdef _DEBUG
    setDebugName("CBillboardSceneNode");
    #endif

    setSize(size);

    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 1;
    indices[3] = 0;
    indices[4] = 3;
    indices[5] = 2;

    vertices[0].TCoords.set(1.0f, 1.0f);
    vertices[0].Color = colorBottom;

    vertices[1].TCoords.set(1.0f, 0.0f);
    vertices[1].Color = colorTop;

    vertices[2].TCoords.set(0.0f, 0.0f);
    vertices[2].Color = colorTop;

    vertices[3].TCoords.set(0.0f, 1.0f);
    vertices[3].Color = colorBottom;


    m_animTexturePath = "../../media/textures/atomic/tr_anim_";
    for(int i=0;i<7;i++)
    {
        core::stringc animName = m_animTexturePath;
        animName = animName + core::stringc(i) + ".png";
        m_gamePark->device()->getVideoDriver()->getTexture(animName);
    }
    setVisible(false);
    setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
}

NuclearBoomSceneNode::~NuclearBoomSceneNode()
{
    removeAll();
    removeAnimators();
    remove();
}

//! pre render event
void NuclearBoomSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

video::SMaterial& NuclearBoomSceneNode::getMaterial(u32 /*i*/)
{
    return Material;
}


//! returns amount of materials used by this scene node.
u32 NuclearBoomSceneNode::getMaterialCount() const
{
    return 1;
}

void NuclearBoomSceneNode::start()
{
    setVisible(true);
    m_boomStartTime = m_gamePark->device()->getTimer()->getTime();
}

void NuclearBoomSceneNode::stop()
{
    setVisible(false);
    m_boomStartTime = 0;
}

void NuclearBoomSceneNode::setSize(const core::dimension2d<f32> &size)
{
    Size = size;

    if (core::equals(Size.Width, 0.0f))
        Size.Width = 1.0f;
    TopEdgeWidth = Size.Width;

    if (core::equals(Size.Height, 0.0f))
        Size.Height = 1.0f;

    const f32 avg = (Size.Width + Size.Height)/6;
    BBox.MinEdge.set(-avg,-avg,-avg);
    BBox.MaxEdge.set(avg,avg,avg);
}

void NuclearBoomSceneNode::setSize(f32 height, f32 bottomEdgeWidth, f32 topEdgeWidth)
{
    Size.set(bottomEdgeWidth, height);
    TopEdgeWidth = topEdgeWidth;

    if (core::equals(Size.Height, 0.0f))
        Size.Height = 1.0f;

    if (core::equals(Size.Width, 0.f) && core::equals(TopEdgeWidth, 0.f))
    {
        Size.Width = 1.0f;
        TopEdgeWidth = 1.0f;
    }

    const f32 avg = (core::max_(Size.Width,TopEdgeWidth) + Size.Height)/6;
    BBox.MinEdge.set(-avg,-avg,-avg);
    BBox.MaxEdge.set(avg,avg,avg);
}

const core::dimension2d<f32> &NuclearBoomSceneNode::getSize() const
{
    return Size;
}

void NuclearBoomSceneNode::getSize(f32 &height, f32 &bottomEdgeWidth, f32 &topEdgeWidth) const
{
    height = Size.Height;
    bottomEdgeWidth = Size.Width;
    topEdgeWidth = TopEdgeWidth;
}

void NuclearBoomSceneNode::setColor(const video::SColor &overallColor)
{
    for(u32 vertex = 0; vertex < 4; ++vertex)
            vertices[vertex].Color = overallColor;
}

void NuclearBoomSceneNode::setColor(const video::SColor &topColor, const video::SColor &bottomColor)
{
    vertices[0].Color = bottomColor;
    vertices[1].Color = topColor;
    vertices[2].Color = topColor;
    vertices[3].Color = bottomColor;
}

void NuclearBoomSceneNode::getColor(video::SColor &topColor, video::SColor &bottomColor) const
{
    bottomColor = vertices[0].Color;
    topColor = vertices[1].Color;
}

void NuclearBoomSceneNode::render()
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();

    if (!camera || !driver)
        return;

    // make billboard look to camera

    core::vector3df pos = getAbsolutePosition();

    core::vector3df campos = camera->getAbsolutePosition();
    core::vector3df target = camera->getTarget();
    core::vector3df up = camera->getUpVector();
    core::vector3df view = target - campos;
    view.normalize();

    core::vector3df horizontal = up.crossProduct(view);
    if ( horizontal.getLength() == 0 )
    {
        horizontal.set(up.Y,up.X,up.Z);
    }
    horizontal.normalize();
    core::vector3df topHorizontal = horizontal * 0.5f * TopEdgeWidth;
    horizontal *= 0.5f * Size.Width;

    // pointing down!
    core::vector3df vertical = horizontal.crossProduct(view);
    vertical.normalize();
    vertical *= 0.5f * Size.Height;

    view *= -1.0f;

    for (s32 i=0; i<4; ++i)
        vertices[i].Normal = view;

    /* Vertices are:
    2--1
    |\ |
    | \|
    3--0
    */
    vertices[0].Pos = pos + horizontal + vertical;
    vertices[1].Pos = pos + topHorizontal - vertical;
    vertices[2].Pos = pos - topHorizontal - vertical;
    vertices[3].Pos = pos - horizontal + vertical;

    // draw

    if (DebugDataVisible & scene::EDS_BBOX)
    {
        driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
        video::SMaterial m;
        m.Lighting = false;
        driver->setMaterial(m);
        driver->draw3DBox(BBox, video::SColor(0,208,195,152));
    }

    driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

    driver->setMaterial(Material);

    driver->drawIndexedTriangleList(vertices, 4, indices, 2);
}

void NuclearBoomSceneNode::OnAnimate(u32 timeMs)
{
    if(isVisible())
    {
        u32 frame = (timeMs - m_boomStartTime) / 100 *1.2;
        setFrame( frame );
    }
    IBillboardSceneNode::OnAnimate(timeMs);
}

void NuclearBoomSceneNode::setFrame(int frame)
{
    if(frame == m_frame)
    {
        return;
    }
    m_frame = frame;
    u32 animNum = frame / 9;
    core::stringc animName = m_animTexturePath;
    animName = animName + core::stringc(animNum) + ".png";

    setMaterialTexture(0, m_gamePark->device()->getVideoDriver()->getTexture(animName));

    f32 scaleCoef = 0.333333;
    u32 localFrame = frame % 9;
    u32 posX = localFrame % 3;
    u32 posY = localFrame / 3;
    core::vector2df pos(posX*scaleCoef,posY*scaleCoef);

    getMaterial(0).getTextureMatrix(0).buildTextureTransform(
         0,
         core::vector2df(0,0),
         core::vector2df(pos.X, pos.Y),
         core::vector2df(scaleCoef, scaleCoef)
        );

    if(frame >= 61)
    {
        stop();
        finished.Emit();
    }
}

const core::aabbox3d<f32> &NuclearBoomSceneNode::getBoundingBox() const
{
    return BBox;
}
