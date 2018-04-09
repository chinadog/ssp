#include "AtomicBoom.h"
#include "GamePark.h"

AtomicBoom::AtomicBoom()
{
    m_animTexturePath = "../../media/textures/atomic/tr_anim_";
}

void AtomicBoom::setParent(GamePark *value)
{
    m_game = value;
    m_device = m_game->device();
    for(int i=0;i<7;i++)
    {
        core::stringc animName = m_animTexturePath;
        animName = animName + core::stringc(i) + ".png";
        m_device->getVideoDriver()->getTexture(animName);
    }
}

void AtomicBoom::start()
{
    if(!m_device)
    {
        return;
    }

     m_boomStartTime = m_device->getTimer()->getTime();
     m_boomNode = m_device->getSceneManager()->addBillboardSceneNode(0, core::dimension2d<f32>(150, 150));
     m_boomNode->setPosition(core::vector3df(480.5, 51, 948));
     m_boomNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

}

void AtomicBoom::updateFrame()
{
    u32 currentTimer = m_device->getTimer()->getTime();
    u32 delta = currentTimer - m_boomStartTime;
    u32 frame = delta / 100 *1.2;
    setFrame( frame );
}

void AtomicBoom::setFrame(u32 frame)
{
    if(!m_device)
    {
        return;
    }

    u32 animNum = frame / 9;
    core::stringc animName = m_animTexturePath;
    animName = animName + core::stringc(animNum) + ".png";

    m_boomNode->setMaterialTexture(0, m_device->getVideoDriver()->getTexture(animName));

    f32 scaleCoef = 0.333333;
    u32 localFrame = frame % 9;
    u32 posX = localFrame % 3;
    u32 posY = localFrame / 3;
    core::vector2df pos(posX*0.333333,posY*0.333333);

    m_boomNode->getMaterial(0).getTextureMatrix(0).buildTextureTransform(
         0,
         core::vector2df(0,0),
         core::vector2df(pos.X, pos.Y),
         core::vector2df(scaleCoef, scaleCoef)
        );

    if(frame >= 61)
    {
        stop();
        m_game->setSceneMode(SceneMode::Game);
    }
}

void AtomicBoom::stop()
{
    m_boomStartTime = 0;
    m_boomNode->setVisible(false);
    m_boomNode->removeAll();
    m_boomNode->removeAnimators();
    m_boomNode->remove();

    m_boomNode = nullptr;
}
