#include "CZ805.h"
#include <iostream>
#include "Player/Player.h"

CZ805::CZ805(GamePark* gamePark) : Weapon(gamePark)
{
    setType(Type::CZ805);
    initNode();
    initSounds();
}

CZ805::~CZ805()
{

}

AnimationFrameLoop CZ805::animationFrameLoop(const WeaponAction &action)
{
    AnimationFrameLoop afl;
    switch (action) {
    case WeaponAction::Stand :
        afl.setStart(136);
        afl.setEnd(256);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case WeaponAction::Walk :
        afl.setStart(256);
        afl.setEnd(291);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case WeaponAction::Run :
        afl.setStart(256);
        afl.setEnd(291);
        afl.setSpeed(60);
        afl.setLoop(true);
        break;
    case WeaponAction::Shoot :
        afl.setStart(120);
        afl.setEnd(136);
        afl.setSpeed(80);
        afl.setLoop(true);
        break;
    case WeaponAction::Reload :
        afl.setStart(36);
        afl.setEnd(120);
        afl.setSpeed(40);
        afl.setLoop(false);
        afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &CZ805::reloadFinished);
        break;
    case WeaponAction::Draw :
        afl.setStart(0);
        afl.setEnd(35);
        afl.setSpeed(40);
        afl.setLoop(false);
        afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &CZ805::drawFinished);
        break;
    default:
        break;
    }
    return afl;
}

AnimationFrameLoop CZ805::animationFrameLoop(const PlayerState &action)
{
    AnimationFrameLoop afl;
    switch (action) {
    case PlayerState::Stand :
        afl.setStart(136);
        afl.setEnd(216);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::Duck :
        afl.setStart(136);
        afl.setEnd(256);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::Walk :
        afl.setStart(256);
        afl.setEnd(291);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::Run :
        afl.setStart(256);
        afl.setEnd(291);
        afl.setSpeed(60);
        afl.setLoop(true);
        break;
    case PlayerState::DuckWalk :
        afl.setStart(256);
        afl.setEnd(291);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::StandShoot :
        afl.setStart(120);
        afl.setEnd(136);
        afl.setSpeed(80);
        afl.setLoop(true);
        break;
    case PlayerState::DuckShoot :
        afl.setStart(120);
        afl.setEnd(136);
        afl.setSpeed(80);
        afl.setLoop(true);
        break;
    case PlayerState::WalkShoot :
        afl.setStart(120);
        afl.setEnd(136);
        afl.setSpeed(80);
        afl.setLoop(true);
        break;
    case PlayerState::RunShoot :
        afl.setStart(120);
        afl.setEnd(136);
        afl.setSpeed(80);
        afl.setLoop(true);
        break;
    case PlayerState::DuckWalkShoot :
        afl.setStart(120);
        afl.setEnd(136);
        afl.setSpeed(80);
        afl.setLoop(true);
        break;
    case PlayerState::StandReload :
    case PlayerState::DuckReload :
    case PlayerState::WalkReload :
    case PlayerState::RunReload :
    case PlayerState::DuckWalkReload :
        afl.setStart(36);
        afl.setEnd(120);
        afl.setSpeed(40);
        afl.setLoop(false);
        afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &CZ805::reloadFinished);
        break;
    case PlayerState::StandDraw :
    case PlayerState::DuckDraw :
    case PlayerState::WalkDraw :
    case PlayerState::RunDraw :
    case PlayerState::DuckWalkDraw :
        afl.setStart(0);
        afl.setEnd(35);
        afl.setSpeed(40);
        afl.setLoop(false);
        afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &CZ805::drawFinished);
        break;
    default:
        break;
    }
    return afl;
}

int CZ805::shootBreakDelta() const
{
    return 150;
}

int CZ805::shootDelta() const
{
    return 20;
}

void CZ805::drawFinished()
{
    m_player->fms().setState(PlayerSignal::StopDraw);
    m_player->setPlayerState(m_player->fms().currentState());
}

void CZ805::reloadFinished()
{
    m_player->fms().setState(PlayerSignal::StopDraw);
    m_player->setPlayerState(m_player->fms().currentState());

    int currentCountAll = m_bulletCount+m_bulletCountInClip;
    if(currentCountAll > bulletMaxCountInClip())
    {
        m_bulletCount = bulletMaxCountInClip();
        m_bulletCountInClip -= bulletMaxCountInClip();
    }
    else
    {
        m_bulletCount = currentCountAll;
        m_bulletCountInClip = 0;
    }
    m_player->updatePlayerInfo();

}

void CZ805::initNode()
{
    scene::IAnimatedMeshSceneNode* node;
    scene::IAnimatedMesh* mesh;
    mesh = m_device->getSceneManager()->getMesh("../../media/models/cz805.b3d");
    if (!mesh)
    {
        m_device->drop();
    }
    node = m_device->getSceneManager()->addAnimatedMeshSceneNode(mesh,m_player->camera() );
    if (node)
    {
        node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setPosition(core::vector3df(0.1, -0.083, -0.1));
        node->setRotation(core::vector3df(0,180,0));
        node->setScale(core::vector3df(0.1,0.1,0.1));

//        node->getMaterial(0).setTexture( 0, m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/ANPEQ-2.tga") );
//        node->getMaterial(1).setTexture( 0, m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(2).setTexture( 0, m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(3).setTexture( 0, m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(4).setTexture( 0, m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/f7.tga") );
//        node->getMaterial(5).setTexture( 0, m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/f7.tga") );
//        node->getMaterial(6).setTexture( 0, m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(7).setTexture( 0, m_device->getVideoDriver()->getTexture("../../media/blend/cz805/hands/v_hands.tga") );
//        node->getMaterial(8).setTexture( 0, m_device->getVideoDriver()->getTexture("../../media/blend/cz805/hands/sleeve_diffuse_ts.tga") );
//        node->getMaterial(9).setTexture( 0, m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(10).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/lense.tga") );
//        node->getMaterial(11).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(12).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/Magint.tga") );
//        node->getMaterial(13).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/map.tga") );
//        node->getMaterial(14).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/f7.tga") );
//        node->getMaterial(15).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(16).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(17).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(18).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(19).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
//        node->getMaterial(20).setTexture( 0,m_device->getVideoDriver()->getTexture("../../media/blend/cz805/cz805/CZ805.tga") );
        node->setMaterialTexture(0,m_device->getVideoDriver()->getTexture("../../media/textures/plitka.tga"));

    }
    m_node = node;
}

void CZ805::initSounds()
{
    m_shootSound = StaticSoundEngine->play2D("../../media/sounds/shoots.ogg", true, false, true);
    m_shootSound->setIsPaused(true);
    m_shootSound->setPlayPosition(0);
    m_shootSound->setVolume(0.1);

    m_reloadSound = StaticSoundEngine->play2D("../../media/sounds/cz805_reload.ogg", false, false, true);
    m_reloadSound->setIsPaused(true);
    m_reloadSound->setPlayPosition(0);
    m_reloadSound->setVolume(1.0);
    m_reloadSound->setPlaybackSpeed(1.2);

    m_drawSound = StaticSoundEngine->play2D("../../media/sounds/cz805_draw.ogg", false, false, true);
    m_drawSound->setIsPaused(true);
    m_drawSound->setPlayPosition(0);
    m_drawSound->setVolume(1.5);
    m_drawSound->setPlaybackSpeed(2.5);
}

void CZ805::addBullets(int value)
{
    m_bulletCountInClip += value;
}

void CZ805::removeBullets(int value)
{
    m_bulletCount-=value;
}

int CZ805::bulletMaxCountInClip() const
{
    return 30;
}
