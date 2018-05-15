#include "Knife.h"
#include <iostream>
#include "Player/Player.h"

Knife::Knife(GamePark* gamePark) : Weapon(gamePark)
{
    setType(Type::Knife);
    initNode();
    initSounds();
    m_bulletCount = 1;
    m_fireRate = 5.0f;
}

Knife::~Knife()
{

}

AnimationFrameLoop Knife::animationFrameLoop(const WeaponAction &/*action*/)
{
    AnimationFrameLoop afl;
    return afl;
}

AnimationFrameLoop Knife::animationFrameLoop(const PlayerState &action)
{
    AnimationFrameLoop afl;
    switch (action) {
    case PlayerState::Stand :
        afl.setStart(40);
        afl.setEnd(60);
        afl.setSpeed(7);
        afl.setLoop(true);
        break;
    case PlayerState::Duck :
        afl.setStart(0);
        afl.setEnd(0);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::Walk :
        afl.setStart(0);
        afl.setEnd(0);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::Run :
        afl.setStart(0);
        afl.setEnd(0);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::DuckWalk :
        afl.setStart(0);
        afl.setEnd(0);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::StandShoot :
        afl.setStart(60);
        afl.setEnd(100);
        afl.setSpeed(60);
        afl.setLoop(true);
        break;
    case PlayerState::DuckShoot :
        afl.setStart(60);
        afl.setEnd(100);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::WalkShoot :
        afl.setStart(60);
        afl.setEnd(100);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::RunShoot :
        afl.setStart(60);
        afl.setEnd(100);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::DuckWalkShoot :
        afl.setStart(60);
        afl.setEnd(100);
        afl.setSpeed(30);
        afl.setLoop(true);
        break;
    case PlayerState::StandReload :
    case PlayerState::DuckReload :
    case PlayerState::WalkReload :
    case PlayerState::RunReload :
    case PlayerState::DuckWalkReload :
        afl.setStart(0);
        afl.setEnd(0);
        afl.setSpeed(30);
        afl.setLoop(false);
        m_player->fms().setState(PlayerSignal::StopDraw);
        m_player->setPlayerState(m_player->fms().currentState());
        break;
    case PlayerState::StandDraw :
    case PlayerState::DuckDraw :
    case PlayerState::WalkDraw :
    case PlayerState::RunDraw :
    case PlayerState::DuckWalkDraw :
        afl.setStart(0);
        afl.setEnd(40);
        afl.setSpeed(60);
        afl.setLoop(false);
        afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &Knife::drawFinished);
//        m_player->fms().setState(PlayerSignal::StopDraw);
//        m_player->setPlayerState(m_player->fms().currentState());
        break;
    default:
        break;
    }
    return afl;
}

int Knife::shootBreakDelta() const
{
    return 250;
}

int Knife::shootDelta() const
{
    return 20;
}

void Knife::drawFinished()
{
    std::cout << "Draw finished" << std::endl;
    m_player->fms().setState(PlayerSignal::StopDraw);
    m_player->setPlayerState(m_player->fms().currentState());
}

void Knife::reloadFinished()
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

void Knife::initNode()
{
    scene::IAnimatedMeshSceneNode* node;
    scene::IAnimatedMesh* mesh;
    mesh = m_device->getSceneManager()->getMesh("../../media/models/knife.x");
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
        node->setMaterialTexture(0,m_device->getVideoDriver()->getTexture("../../media/textures/plitka.tga"));
    }
    m_node = node;
}

void Knife::initSounds()
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

void Knife::addBullets(int value)
{
    m_bulletCountInClip += value;
}

void Knife::removeBullets(int value)
{
//    m_bulletCount-=value;
}

int Knife::bulletMaxCountInClip() const
{
    return 30;
}

f32 Knife::damage() const
{
    return 1.0;
}
