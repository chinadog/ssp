#ifndef GREENMONSTERNODE_H
#define GREENMONSTERNODE_H

#include "Monster/MonsterNode.h"
#include <irrlicht.h>

using namespace irr;

class GreenMonsterNode : public MonsterNode
{
public:
    GreenMonsterNode(GamePark* gamePark);
    int init();
    void draw();
    void setSpeedOfTime(f32 speed);
};

#endif // GREENMONSTERNODE_H
