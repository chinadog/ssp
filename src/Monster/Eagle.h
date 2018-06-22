#ifndef EAGLE_H
#define EAGLE_H

#include <irrlicht.h>
#include "AI.h"
#include "Monster/Enemy.h"
#include <list>
#include "Monster/MonsterFMS.h"

using namespace irr;

class Eagle : public Enemy
{
public:
    Eagle(GamePark* gamePark);
    ~Eagle();

    virtual void draw();

    scene::ISceneNode* node() const;// new
    void setPosition(const core::vector3df pos);

    virtual int init();
    core::vector3df ellipsoid() const;
    core::vector3df ellipsoidTranslation() const;



    void setSpeedOfTime(f32 speed);

    inline void setDrawFinishedLevel(f32 value){m_drawFinishedLevel = value;}

    virtual void setMonsterState(const MonsterState& state, bool force = false);
    void woundFinished();
    void dieFinished();
    void atack();
    void walk();
    void stopDraw();
    void atackPlayer();

    void flyGotoPlayer(f32 deltaTime);
    core::vector3df m_center;
    f32 m_rotationSpeed = -0.5;
    int m_sign = -1;
    f32 m_radius = 50;
    bool m_changeTrj = false;

    f32 m_alpha = 0.0;
    u32 m_startTime = 0.0;
    u32 m_trjChangedTime = 8000;

    f32 m_velocity = 50;
    f32 m_trjChangedAngle = 0.0;
    bool m_trjChangedAngleOk = false;

private:
    AnimationEndCallback m_endCallback;
    f32 m_drawFinishedLevel = 0.0;

    scene::ISceneNodeAnimator* m_flyAnim = nullptr;
    scene::ISceneNodeAnimator* m_rotAnim = nullptr;

protected:
    MonsterState m_currentState = MonsterState::Walk;
    int m_speedOfTimeChangedSignalId = -1;

    f32 m_walkSpeed = 7.5;
    f32 m_woundSpeed = 1.5;
    f32 m_atackSpeed = 1.5;
    f32 m_drawSpeed = 0.5;
};

#endif // EAGLE_H
