#ifndef RESPAWNPOINT_H
#define RESPAWNPOINT_H

#include <irrlicht.h>

using namespace irr;

class GamePark;

class RespawnPoint
{
public:
    RespawnPoint(GamePark* gamePark, const core::vector3df& pos);
    ~RespawnPoint();

    void createMonster();
private:
    void init();
    void draw();
    void setSpeedOfTime(f32 speed);

    GamePark* m_gamePark;
    core::vector3df m_pos;
    irr::scene::IAnimatedMeshSceneNode* m_node;
    int signalId = -1;
    f32 m_speedOfTime = 1.0;
    int m_speedOfTimeChangedSignalId = -1;
};

#endif // RESPAWNPOINT_H
