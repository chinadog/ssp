#include "AK.h"
#include <iostream>
#include "Player/Player.h"

AK::AK(GamePark* gamePark) : Weapon(gamePark)
{
    setType(Type::AK);
    initNode();
    initSounds();
}

AK::~AK()
{

}

AnimationFrameLoop AK::animationFrameLoop(const WeaponAction &action)
{
    AnimationFrameLoop afl;
    switch (action) {
    case WeaponAction::Stand :
        afl.setStart(0);
        afl.setEnd(0);
        afl.setSpeed(0);
        afl.setLoop(false);
        break;
    case WeaponAction::Walk :
        afl.setStart(10);
        afl.setEnd(30);
        afl.setSpeed(35);
        afl.setLoop(true);
        break;
    case WeaponAction::Run :
        afl.setStart(10);
        afl.setEnd(30);
        afl.setSpeed(35);
        afl.setLoop(true);
        break;
    case WeaponAction::Shoot :
        afl.setStart(30);
        afl.setEnd(50);
        afl.setSpeed(120);
        afl.setLoop(true);
        break;
    case WeaponAction::Draw :
        afl.setStart(20);
        afl.setEnd(30);
        afl.setSpeed(35);
        afl.setLoop(false);
        afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &AK::drawFinished);
        break;
    default:
        break;
    }
    return afl;
}

AnimationFrameLoop AK::animationFrameLoop(const PlayerState &action)
{
    AnimationFrameLoop afl;
    switch (action) {
    case PlayerState::Stand :
        afl.setStart(0);
        afl.setEnd(0);
        afl.setSpeed(0);
        afl.setLoop(false);
        break;
    case PlayerState::Duck :
        afl.setStart(0);
        afl.setEnd(0);
        afl.setSpeed(0);
        afl.setLoop(false);
        break;
    case PlayerState::Walk :
        afl.setStart(10);
        afl.setEnd(30);
        afl.setSpeed(35);
        afl.setLoop(true);
        break;
    case PlayerState::Run :
        afl.setStart(10);
        afl.setEnd(30);
        afl.setSpeed(35);
        afl.setLoop(true);
        break;
    case PlayerState::DuckWalk :
        afl.setStart(10);
        afl.setEnd(30);
        afl.setSpeed(35);
        afl.setLoop(true);
        break;
    case PlayerState::StandShoot :
        afl.setStart(30);
        afl.setEnd(50);
        afl.setSpeed(120);
        afl.setLoop(true);
        break;
    case PlayerState::DuckShoot :
        afl.setStart(30);
        afl.setEnd(50);
        afl.setSpeed(120);
        afl.setLoop(true);
        break;
    case PlayerState::WalkShoot :
        afl.setStart(30);
        afl.setEnd(50);
        afl.setSpeed(120);
        afl.setLoop(true);
        break;
    case PlayerState::RunShoot :
        afl.setStart(30);
        afl.setEnd(50);
        afl.setSpeed(120);
        afl.setLoop(true);
        break;
    case PlayerState::DuckWalkShoot :
        afl.setStart(10);
        afl.setEnd(30);
        afl.setSpeed(35);
        afl.setLoop(true);
        break;
    case PlayerState::StandReload :
    case PlayerState::DuckReload :
    case PlayerState::WalkReload :
    case PlayerState::RunReload :
    case PlayerState::DuckWalkReload :
        afl.setStart(10);
        afl.setEnd(30);
        afl.setSpeed(35);
        afl.setLoop(false);
        afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &AK::reloadFinished);
        break;
    case PlayerState::StandDraw :
    case PlayerState::DuckDraw :
    case PlayerState::WalkDraw :
    case PlayerState::RunDraw :
    case PlayerState::DuckWalkDraw :
        afl.setStart(20);
        afl.setEnd(30);
        afl.setSpeed(15);
        afl.setLoop(false);
        afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &AK::drawFinished);
        break;
    default:
        break;
    }
    return afl;
}

int AK::shootBreakDelta() const
{
    return 150;
}

int AK::shootDelta() const
{
    return 20;
}

void AK::drawFinished()
{
    std::cout << "Draw finished" << std::endl;

    m_player->fms().setState(PlayerSignal::StopDraw);
    m_player->setPlayerState(m_player->fms().currentState());

    m_player->updatePlayerInfo();
}

void AK::reloadFinished()
{
    std::cout << "Reload finished" << std::endl;

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

void AK::initNode()
{
    scene::IAnimatedMeshSceneNode* node;
    scene::IAnimatedMesh* mesh;
    mesh = m_device->getSceneManager()->getMesh("../../media/models/ak12.x");
    if (!mesh)
    {
        m_device->drop();
    }
    node = m_device->getSceneManager()->addAnimatedMeshSceneNode(mesh,m_player->camera() );
    if (node)
    {
        node->setScale(core::vector3df(0.2,0.2,0.2));
        node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setPosition(core::vector3df(0.1,-0.6133333,0.4533333));
        node->setRotation(core::vector3df(0,180,0));
        node->setMaterialTexture( 0, m_device->getVideoDriver()->getTexture("../../media/textures/plitka.tga") );
    }
    m_node = node;
}

void AK::initSounds()
{
    m_shootSound = StaticSoundEngine->play2D("../../media/sounds/shoots.ogg", true, false, true);
    m_shootSound->setIsPaused(true);
    m_shootSound->setPlayPosition(0);
    m_shootSound->setVolume(0.1);

    m_reloadSound = StaticSoundEngine->play2D("../../media/sounds/cz805_reload.ogg", false, false, true);
    m_reloadSound->setIsPaused(true);
    m_reloadSound->setPlayPosition(0);
    m_reloadSound->setVolume(1.5);

    m_drawSound = StaticSoundEngine->play2D("../../media/sounds/cz805_draw.ogg", false, false, true);
    m_drawSound->setIsPaused(true);
    m_drawSound->setPlayPosition(0);
    m_drawSound->setVolume(1.5);
}

void AK::addBullets(int value)
{
    m_bulletCountInClip += value;
}

void AK::removeBullets(int value)
{
    m_bulletCount-=value;
}

int AK::bulletMaxCountInClip() const
{
    return 30;
}



