#include "Weapon/Weapon.h"
#include "Player/Player.h"
#include "GamePark.h"

Weapon::Weapon(GamePark* gamePark) :
    m_gamePark(gamePark),
    m_device(gamePark->device()),
    m_player(gamePark->player())
{

}

Weapon::~Weapon()
{
    m_shootSound->drop();
    m_reloadSound->drop();
    m_drawSound->drop();
}

Weapon::Type Weapon::type() const
{
    return m_type;
}

std::list<AnimationFrameLoop> Weapon::frameLoops()
{
    return m_animationFrameLoopList;
}

//void Weapon::playSound(bool play, ISound* sound, ISoundSource *source, bool playLooped, bool startPaused, bool track, f32 volume)
//{
//    if(play)
//    {
//        if(sound != nullptr)
//        {
//            return;
//        }
//        sound = StaticSoundEngine->play2D(source, playLooped ,startPaused, track);
//        sound->setVolume(volume);
//        sound->drop();
//    }
//    else
//    {
//        if(sound == nullptr)
//        {
//            return;
//        }
//        sound->stop();
//        sound = nullptr;
//    }
//}

void Weapon::setType(const Weapon::Type &type)
{
    m_type = type;
}

void Weapon::setAnimationFrameLoopList(const Weapon::Type &/*type*/)
{

}

void Weapon::shootSound(bool play)
{
    playSound(play, m_shootSound);
}

void Weapon::reloadSound(bool play)
{
    playSound(play, m_reloadSound);
}

void Weapon::drawSound(bool play)
{
    playSound(play, m_drawSound);
}

void Weapon::playSound(bool play,ISound* sound)
{
    if(!sound)
    {
        return;
    }
    if(play)
    {
        sound->setPlayPosition(0);
        sound->setIsPaused(false);
    }
    else
    {
        sound->setIsPaused(true);
        sound->setPlayPosition(0);
    }
}
