#ifndef REDMONSTERNODENEW_H
#define REDMONSTERNODENEW_H

#include "SceneNode/AnimatedMeshSceneNode.h"
#include <irrlicht.h>
#include "Common/SignalSlot.h"
#include "MonsterFMS.h"
#include "Weapon/AnimationFrameLoop.h"
#include "Weapon/AnimationEndCallback.h"
#include <list>

using namespace irr;

class GamePark;
class Player;
class DieEndCallBackNew;

class RedMonsterNodeNew : public AnimatedMeshSceneNode
{
public:
    RedMonsterNodeNew(GamePark* gamePark);
    ~RedMonsterNodeNew();

    // AI
public:

    f32 m_deltaTime = 0;
    u32 m_prevTime = 0;

    scene::ISceneNodeAnimatorCollisionResponse* m_gravityAnim = nullptr;

    void atack();
    void walk();
    void stopDraw();
    void setTerrain(scene::ITerrainSceneNode* terrain);
    irr::core::vector3df ellipsoid() const;
    irr::core::vector3df ellipsoidTranslation() const;
    void gotoPlayer(f32 timeInSeconds);
//    void showEllipsoid();
    Signal<> layOut;
private:
    void createGravitation();
    void moveNode(const core::vector3df& pos, f32 timeInSeconds);
protected:
    irr::scene::ISceneManager* m_smgr = nullptr;
    irr::video::IVideoDriver* m_driver = nullptr;
    irr::IrrlichtDevice* m_device = nullptr;
    Player* m_player = nullptr;
    scene::ITerrainSceneNode* m_terrain = nullptr;
    bool m_movable = true;
    f32 m_speed = 34.5;
    bool m_isRotated = true;
    bool m_intersects = false;
    f32 m_distanceToPlayer = 0.0;
    bool m_isDraw = true;
    bool m_layOut = false;
    scene::ITriangleSelector* m_octreeSelector = nullptr;
    f32 m_speedOfTime = 1.0;
    AnimationFrameLoop m_afl;
    GamePark* m_gamePark;

    core::vector3df m_aiPosition;
    core::vector3df m_aiRotation;
    f32 m_atackDistance = 5.0;
    // END AI
//===================================================================================
    // Enemy
public:
    inline f32 health() const {return m_health;}
    inline void setHealth(f32 value) {m_health = value;}

    f32 m_health = 1.0;
    // End enemy
//===================================================================================
    // Monster
public:
    void draw();
    void calcPosAndRot();


    void kill();
    void damage(f32 value, const core::vector3df& intersection);
    void die(DieEndCallBackNew* callback);
    inline bool life() {return m_life;}
    inline void setLife(bool value) {m_life = value;}
    void addCallback(scene::IAnimationEndCallBack* callback);

    void dustEffect( const core::vector3df& pos );
    void bloodEffect( const core::vector3df& pos );

    void addTriangleSelector(RedMonsterNodeNew* node);
    void addTriangleSelector(scene::ITriangleSelector *selector);
    void removeAllTriangleSelector();
    void removeTriangleSelector(RedMonsterNodeNew* node);
    void updateCollisionAnimator();
    inline scene::IMetaTriangleSelector* metaTriangleSelector() const {return m_metaTriangleSelector;}
    scene::ISceneNodeAnimatorCollisionResponse* m_collisionAnimator = nullptr;

    scene::ITriangleSelector* m_boxSelector = nullptr;

    bool isHeadshot(core::vector3df point);


    inline void setDrawFinishedLevel(f32 value){m_drawFinishedLevel = value;}

    void setMonsterStateMonster(const MonsterState& state, bool force = false);
    void setMonsterState(const MonsterState& state, bool force = false);
    void woundFinished();
    void atackPlayer();
private:
    bool m_life = true;
    AnimationEndCallback m_endCallback;
    std::list<scene::IAnimationEndCallBack*> m_callbackList;
    f32 m_drawFinishedLevel = 0.0;
protected:
    MonsterState m_currentState = MonsterState::Draw;
    scene::IMetaTriangleSelector *m_metaTriangleSelector = nullptr;
    int m_speedOfTimeChangedSignalId = -1;
    MonsterFMS m_fms;

    f32 m_walkSpeed = 34.5;
//    f32 m_walkSpeed = 7.5;
    f32 m_woundSpeed = 1.5;
    f32 m_atackSpeed = 1.5;
    f32 m_drawSpeed = 0.5;
    // end monster
    //===================================================================================
    // red
    void setSpeedOfTime(f32 speed);
    void fireEffect();

    scene::IParticleEmitter* m_fireEmitter = nullptr;
    scene::IParticleSystemSceneNode* m_ps = nullptr;
    // end red


    // NEW

    void OnAnimate(u32 timeMs);

    u32 fpsCount = 0;
};

class DieEndCallBackNew : public scene::IAnimationEndCallBack
{
public:
    DieEndCallBackNew(RedMonsterNodeNew* parent = 0);
    virtual void OnAnimationEnd(scene::IAnimatedMeshSceneNode* node);
private:
    RedMonsterNodeNew* m_parent = nullptr;
};

#endif // REDMONSTERNODENEW_H
