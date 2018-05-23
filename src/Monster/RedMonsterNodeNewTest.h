#ifndef REDMONSTERNODENEWTEST_H
#define REDMONSTERNODENEWTEST_H

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
class DieEndCallBackNewTest;

class RedMonsterNodeNewTest : public AnimatedMeshSceneNode
{
public:
    RedMonsterNodeNewTest(GamePark* gamePark);
    ~RedMonsterNodeNewTest();

    Signal<> layOut;

    scene::ISceneNodeAnimatorCollisionResponse* m_collisionAnimator = nullptr;
    scene::ITriangleSelector* m_boxSelector = nullptr;
    scene::ITriangleSelector* m_octreeSelector = nullptr;
    scene::IMetaTriangleSelector *m_metaTriangleSelector = nullptr;
    int m_speedOfTimeChangedSignalId = -1;
    void setTerrain(scene::ITerrainSceneNode *terrain);
    void createGravitation();
    irr::core::vector3df ellipsoid() const;
    irr::core::vector3df ellipsoidTranslation() const;
    void gotoPlayer(f32 timeInSeconds);
    void moveNode(const core::vector3df &pos, f32 timeInSeconds);
    void atack();
    void walk();
    void setMonsterState(const MonsterState &state, bool force = false);
    void setMonsterStateMonster(const MonsterState &state, bool force = false);
    void atackPlayer();
    void stopDraw();
    void die(DieEndCallBackNewTest *callback);
    void woundFinished();
    void draw();
    void kill();
    void dustEffect(const core::vector3df &pos);
    void addCallback(scene::IAnimationEndCallBack *callback);
    void damage(f32 value, const core::vector3df &intersection);
    inline f32 health() const {return m_health;}
    inline void setHealth(f32 value) {m_health = value;}
    void bloodEffect(const core::vector3df &pos);
    void addTriangleSelector(RedMonsterNodeNewTest *node);
    void addTriangleSelector(scene::ITriangleSelector *selector);
    void removeAllTriangleSelector();
    void removeTriangleSelector(RedMonsterNodeNewTest *node);
    void updateCollisionAnimator();
    bool isHeadshot(core::vector3df point);
    void setSpeedOfTime(f32 speed);
    void fireEffect();
    void OnAnimate(u32 timeMs);
private:
    irr::scene::ISceneManager* m_smgr = nullptr;
    irr::video::IVideoDriver* m_driver = nullptr;
    irr::IrrlichtDevice* m_device = nullptr;
    Player* m_player = nullptr;
    GamePark* m_gamePark;
    scene::ITerrainSceneNode* m_terrain = nullptr;
    scene::ISceneNodeAnimatorCollisionResponse* m_gravityAnim = nullptr;
    f32 m_distanceToPlayer = 0.0;
    f32 m_atackDistance = 5.0;
    bool m_movable = true;
    f32 m_speed = 7.5;
    f32 m_speedOfTime = 1.0;
    bool m_intersects = false;
    bool m_isRotated = true;
    MonsterFMS m_fms;
    MonsterState m_currentState = MonsterState::Draw;
    AnimationFrameLoop m_afl;
    f32 m_walkSpeed = 7.5;
    AnimationEndCallback m_endCallback;
    f32 m_atackSpeed = 1.5;
    f32 m_drawSpeed = 0.5;
    f32 m_woundSpeed = 1.5;
    scene::IParticleEmitter* m_fireEmitter = nullptr;
    scene::IParticleSystemSceneNode* m_ps = nullptr;
    bool m_life = true;
    f32 m_deltaTime = 0;
    u32 m_prevTime = 0;
    f32 m_health = 1.0;
    bool m_isDraw = true;
    bool m_layOut = false;
    f32 m_drawFinishedLevel = 0.0;
    std::list<scene::IAnimationEndCallBack*> m_callbackList;
};

class DieEndCallBackNewTest : public scene::IAnimationEndCallBack
{
public:
    DieEndCallBackNewTest(RedMonsterNodeNewTest* parent = 0);
    virtual void OnAnimationEnd(scene::IAnimatedMeshSceneNode* node);
private:
    RedMonsterNodeNewTest* m_parent = nullptr;
};

#endif // REDMONSTERNODENEWTEST_H
