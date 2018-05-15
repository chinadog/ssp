#include "SlowMo.h"
#include "GamePark.h"
#include "Common/Common.h"

SlowMo::SlowMo(GamePark* gamePark) : m_gamePark(gamePark)
{
    m_camera = m_gamePark->smgr()->addCameraSceneNode();
    m_camera->setFarValue(42000.0f/30.0);
    m_camera->setNearValue(0.1);
}

SlowMo::~SlowMo()
{
    m_camera->drop();
}

void SlowMo::start(const core::vector3df &pos1, const core::vector3df &pos2)
{
    m_isActive = true;

    m_pos1 = pos1;
    m_pos2 = pos2;

    m_camera->setPosition(m_gamePark->player()->camera()->getPosition());
    m_camera->setTarget(m_cameraTarget);




    m_gamePark->setSpeedOfTime(0.1);
    disablePlayer();
    m_gamePark->smgr()->setActiveCamera(m_camera);

    core::array<core::vector3df> camPoints;
    camPoints.push_back(pos1);
    camPoints.push_back(pos2);

    std::cout << "Distance slow = " << pos1.getDistanceFrom(pos2) << std::endl;

    f32 t = 20.0;
    f32 camSpeed = 1/camPoints[0].getDistanceFrom(camPoints[1])* t;

    scene::ISceneNodeAnimator* anim = 0;
    anim = m_gamePark->smgr()->createFollowSplineAnimator(m_gamePark->device()->getTimer()->getTime(), camPoints, camSpeed, 0.5, false);
    m_camera->addAnimator(anim);
    anim->drop();
}

void SlowMo::draw()
{
    if(m_isActive == false && m_slowmoEvents.size() == 0)
    {
        return;
    }
    if(m_isActive == false)
    {
        auto i = std::begin(m_slowmoEvents);
        SEvent event = *i;
        m_gamePark->smgr()->postEventFromUser(event);
        m_slowmoEvents.pop_front();
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

        m_gamePark->player()->camera()->setInputReceiverEnabled(true);

        m_gamePark->player()->fms().setState(PlayerSignal::Stop);
        m_gamePark->player()->setPlayerState(m_gamePark->player()->fms().currentState());


        SEvent event;
        event.EventType = irr::EET_KEY_INPUT_EVENT;
        event.KeyInput.PressedDown = m_gamePark->receiver->m_keyW;
        event.KeyInput.Key = KEY_KEY_W;
        m_slowmoEvents.push_back(event);
        event.KeyInput.PressedDown = m_gamePark->receiver->m_keyA;
        event.KeyInput.Key = KEY_KEY_A;
        m_slowmoEvents.push_back(event);
        event.KeyInput.PressedDown = m_gamePark->receiver->m_keyS;
        event.KeyInput.Key = KEY_KEY_S;
        m_slowmoEvents.push_back(event);
        event.KeyInput.PressedDown = m_gamePark->receiver->m_keyD;
        event.KeyInput.Key = KEY_KEY_D;
        m_slowmoEvents.push_back(event);
        event.KeyInput.PressedDown = m_gamePark->receiver->m_keyShift;
        event.KeyInput.Key = (irr::EKEY_CODE)160;
        m_slowmoEvents.push_back(event);

        m_gamePark->player()->m_bulletNode->setPosition(core::vector3df(0.2,-0.2,3));
        m_gamePark->player()->m_bulletNode->setVisible(false);

    }
}

void SlowMo::disablePlayer()
{
    m_gamePark->player()->setMouseButtonPressed(EMIE_LMOUSE_LEFT_UP);
    m_gamePark->player()->camera()->setInputReceiverEnabled(false);

}
