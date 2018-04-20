#ifndef RESPAWNPOINT_H
#define RESPAWNPOINT_H

#include <irrlicht.h>

using namespace irr;

class GamePark;

class RespawnPoint
{
public:
    RespawnPoint(GamePark* gamePark, const core::vector3df& pos);

    void createMonster();
private:
    void init();

    GamePark* m_gamePark;
    core::vector3df m_pos;
};

#endif // RESPAWNPOINT_H
