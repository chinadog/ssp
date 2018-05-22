#ifndef REDMONSTERNODENEW_H
#define REDMONSTERNODENEW_H

#include "SceneNode/AnimatedMeshSceneNode.h"
#include <irrlicht.h>
#include "Common/SignalSlot.h"
#include "MonsterFMS.h"
#include "Weapon/AnimationFrameLoop.h"
#include "Weapon/AnimationEndCallback.h"

using namespace irr;

class GamePark;
class Player;

class RedMonsterNodeNew : public AnimatedMeshSceneNode
{
public:
    RedMonsterNodeNew(GamePark* gamePark);
    ~RedMonsterNodeNew();

    inline void setDrawFinishedLevel(f32 value){m_drawFinishedLevel = value;}
    void setTerrain(scene::ITerrainSceneNode* terrain);
    void createGravitation();
    core::vector3df ellipsoid() const;
    irr::core::vector3df ellipsoidTranslation() const;

    scene::ISceneNodeAnimatorCollisionResponse* m_gravityAnim = nullptr;
    Signal<> layOut;

    void OnAnimate(u32 timeMs);
    void stopDraw();
    void atackPlayer();
    void walk();
    void atack();
    void gotoPlayer(f32 timeInSeconds);
    void moveNode(const core::vector3df &pos, f32 timeInSeconds);
    void setMonsterState(const MonsterState &state, bool force = false);
    void woundFinished();
    void dieFinished();

    void addTriangleSelector(RedMonsterNodeNew* node);
    void addTriangleSelector(scene::ITriangleSelector *selector);
    void removeAllTriangleSelector();
    void removeTriangleSelector(RedMonsterNodeNew* node);
    void updateCollisionAnimator();
    inline scene::IMetaTriangleSelector* metaTriangleSelector() const {return m_metaTriangleSelector;}
    scene::ISceneNodeAnimatorCollisionResponse* m_collisionAnimator = nullptr;
    scene::ITriangleSelector* m_boxSelector = nullptr;
private:
    GamePark* m_gamePark = nullptr;
    scene::ISceneManager* m_smgr;
    video::IVideoDriver* m_driver;
    IrrlichtDevice* m_device;
    Player* m_player;
    f32 m_drawFinishedLevel = 0.0;
    scene::ITerrainSceneNode* m_terrain = nullptr;
    f32 m_health = 1.0;
    bool m_isDraw = true;
    bool m_layOut = false;
    bool m_intersects = false;
    int m_speedOfTimeChangedSignalId = -1;
    MonsterFMS m_fms;

    bool m_movable = true;
    f32 m_speed = 7.5;
    bool m_isRotated = true;
    f32 m_distanceToPlayer = 0.0;
    scene::ITriangleSelector* m_octreeSelector = nullptr;
    f32 m_speedOfTime = 1.0;
    AnimationFrameLoop m_afl;
    core::vector3df m_aiPosition;
    core::vector3df m_aiRotation;
    f32 m_atackDistance = 5.0;
    bool m_life = true;

    MonsterState m_currentState = MonsterState::Draw;
    scene::IMetaTriangleSelector *m_metaTriangleSelector = nullptr;
    f32 m_walkSpeed = 7.5;
    f32 m_woundSpeed = 1.5;
    f32 m_atackSpeed = 1.5;
    f32 m_drawSpeed = 0.5;
    AnimationEndCallback m_endCallback;

    f32 m_deltaTime = 0;
    u32 m_prevTime = 0;
};

#endif // REDMONSTERNODENEW_H
