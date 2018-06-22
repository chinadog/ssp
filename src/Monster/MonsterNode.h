#ifndef MONSTERNODE_H
#define MONSTERNODE_H

#include <irrlicht.h>
#include "AI.h"
#include "Monster/Enemy.h"
#include <list>
#include "Monster/MonsterFMS.h"

using namespace irr;


class MonsterNode : public Enemy
{
public:
    MonsterNode(GamePark* gamePark);
    ~MonsterNode();

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
private:
    AnimationEndCallback m_endCallback;
    f32 m_drawFinishedLevel = 0.0;

protected:


    f32 m_walkSpeed = 7.5;
    f32 m_woundSpeed = 1.5;
    f32 m_atackSpeed = 1.5;
    f32 m_drawSpeed = 0.5;
};



#endif // MONSTERNODE_H
