#ifndef REDMONSTERNODE_H
#define REDMONSTERNODE_H

#include "Monster/MonsterNode.h"
#include <irrlicht.h>

using namespace irr;

class RedMonsterNode : public MonsterNode
{
public:
    RedMonsterNode(GamePark* gamePark);
    ~RedMonsterNode();
    int init();
    void draw();
    void setSpeedOfTime(f32 speed);
    void atack();
    void fireEffect();
    void setMonsterState(const MonsterState& state, bool force = false);

    scene::IParticleEmitter* m_fireEmitter = nullptr;
    scene::IParticleSystemSceneNode* m_ps = nullptr;

};

#endif // REDMONSTERNODE_H
