#ifndef WEAPON_H
#define WEAPON_H

#include <irrlicht.h>
#include "Weapon/AnimationFrameLoop.h"
#include <list>
#include "Weapon/WeaponAction.h"
#include "Weapon/AnimationEndCallback.h"
#include "Player/FMS.h"
#include "SoundEngine/SoundEngine.h"

class Player;
class GamePark;

using namespace irr;

class Weapon
{
public:

    enum class Type
    {
        Undefined,
        AK,
        CZ805,
        Knife
    };

    Weapon(GamePark* gamePark = 0);
    virtual ~Weapon();

    Type type() const;
    std::list<AnimationFrameLoop> frameLoops();
    virtual AnimationFrameLoop animationFrameLoop(const WeaponAction& /*action*/){return AnimationFrameLoop();}
    virtual AnimationFrameLoop animationFrameLoop(const PlayerState& /*state*/){return AnimationFrameLoop();}
    virtual int shootBreakDelta() const {return 0;}
    virtual int shootDelta() const {return 0;}
    virtual scene::IAnimatedMeshSceneNode* node() const {return m_node;}
    virtual void addBullets(int /*value*/){}
    virtual void removeBullets(int /*value*/){}
    virtual int bulletCount() const {return m_bulletCount;}
    virtual int bulletCountInClip() const {return m_bulletCountInClip;}
    virtual int bulletMaxCountInClip() const {return 0;}
    virtual f32 damage() const{return 0.2;}
    f32 fireRate() const {return m_fireRate;}

    virtual void shootSound(bool /*play*/);
    virtual void reloadSound(bool /*play*/);
    virtual void drawSound(bool /*play*/);
    void playSound(bool play, ISound* sound);
//    void playSound(bool play, ISound* sound, ISoundSource* source = nullptr, bool playLooped = false,
//                   bool startPaused = false, bool track = false, f32 volume = 1.0);

    void setType(const Type& type);

protected:
    GamePark* m_gamePark = nullptr;
    IrrlichtDevice* m_device = nullptr;
    Player* m_player = nullptr;
    int m_bulletCount = 0;
    int m_bulletCountInClip = 0;
    f32 m_fireRate = 100000.0; // Дальность стрельбы

    ISound* m_shootSound = nullptr;
    ISound* m_reloadSound = nullptr;
    ISound* m_drawSound = nullptr;


    Type m_type = Type::Undefined;
    std::list<AnimationFrameLoop> m_animationFrameLoopList;
    AnimationEndCallback m_endCallback;
    scene::IAnimatedMeshSceneNode* m_node;

    void setAnimationFrameLoopList(const Type& type);

};

#endif // WEAPON_H
