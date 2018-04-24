#include "SlowMo.h"
#include "GamePark.h"
#include "Common/Common.h"

SlowMo::SlowMo(GamePark* gamePark) : m_gamePark(gamePark)
{
    m_camera = m_gamePark->smgr()->addCameraSceneNode();
    m_camera->setFarValue(42000.0f/30.0);
    m_camera->setNearValue(0.1);
}

void SlowMo::start(const core::vector3df &pos1, const core::vector3df &pos2)
{
    m_isActive = true;

    m_pos1 = pos1;
    m_pos2 = pos2;

    m_camera->setPosition(m_gamePark->player()->camera()->getPosition());
    m_camera->setTarget(m_cameraTarget);
    m_gamePark->smgr()->setActiveCamera(m_camera);
    m_gamePark->setSpeedOfTime(0.2);

    core::array<core::vector3df> camPoints;
    camPoints.push_back(pos1);
    camPoints.push_back(pos2);

    scene::ISceneNodeAnimator* anim = 0;
    anim = m_gamePark->smgr()->createFollowSplineAnimator(m_gamePark->device()->getTimer()->getTime(), camPoints, m_speed, 0.5, false);
    m_camera->addAnimator(anim);
    anim->drop();
}

void SlowMo::draw()
{
    if(m_isActive == false)
    {
        return;
    }

    if(m_camera->getPosition().getDistanceFrom(m_pos2) < 0.2 && m_toTarget == true)
    {
        m_toTarget = false;
        m_camera->removeAnimators();

        core::array<core::vector3df> camPoints;
        camPoints.push_back(m_pos2);
        camPoints.push_back(m_pos1);

        scene::ISceneNodeAnimator* anim = 0;
        anim = m_gamePark->smgr()->createFollowSplineAnimator(m_gamePark->device()->getTimer()->getTime(), camPoints, m_reverseSpeed, 0.5, false);
        m_camera->addAnimator(anim);
        anim->drop();
    }
    if(m_camera->getPosition().getDistanceFrom(m_pos1) < 0.2 && m_toTarget == false)
    {
        m_camera->removeAnimators();

        m_gamePark->smgr()->setActiveCamera(m_gamePark->player()->camera());
        m_isActive = false;
        m_toTarget = true;
        m_gamePark->setSpeedOfTime(1.0);
    }
}
