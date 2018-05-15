#ifndef AI_H
#define AI_H

#include <irrlicht.h>
#include "Player/Player.h"
#include <iostream>
#include "Weapon/AnimationFrameLoop.h"
#include "Common/SignalSlot.h"

using namespace irr;

class GamePark;

class AI
{
public:

    AI(GamePark* gamePark);
    virtual ~AI();

    void calcPositionAndRotation(core::vector3df& pos, core::vector3df rot);


    f32 m_deltaTime = 0;
    u32 m_prevTime = 0;


scene::ISceneNodeAnimatorCollisionResponse* m_gravityAnim = nullptr;


    virtual void atack(){}
    virtual void walk(){}
    virtual void stopDraw(){}
    virtual void setPosition(const core::vector3df& pos);
    void setTerrain(scene::ITerrainSceneNode* terrain);
    virtual irr::core::vector3df ellipsoid() const = 0;
    virtual irr::core::vector3df ellipsoidTranslation() const = 0;
    void gotoPlayer(f32 timeInSeconds);
//    void showEllipsoid();
    inline scene::IAnimatedMeshSceneNode* node() {return m_node;} 
    Signal<> layOut;
private:
    void createGravitation();
    void moveNode(const core::vector3df& pos, f32 timeInSeconds);
protected:
    irr::scene::ISceneManager* m_smgr = nullptr;
    irr::video::IVideoDriver* m_driver = nullptr;
    irr::IrrlichtDevice* m_device = nullptr;
    Player* m_player = nullptr;
    scene::IAnimatedMeshSceneNode* m_node = nullptr;
    scene::ITerrainSceneNode* m_terrain = nullptr;
    bool m_movable = true;
    f32 m_speed = 7.5;
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
};

#endif // AI_H
