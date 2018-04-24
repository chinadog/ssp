#include "RespawnPoint.h"
#include "GamePark.h"

RespawnPoint::RespawnPoint(GamePark* gamePark, const core::vector3df& pos) :
    m_gamePark(gamePark),
    m_pos(pos)
{
    init();
}

void RespawnPoint::createMonster()
{
    MonsterNode* node = new MonsterNode(m_gamePark);
    core::vector3df monsterPos = m_pos;
    monsterPos.Y -= 100;
    node->setPosition( monsterPos );
    node->setTerrain(m_gamePark->m_terrain);
    m_gamePark->m_aiNode.push_back(node);
    m_gamePark->updateMonsterCollision();

}

void RespawnPoint::init()
{
    scene::ISceneNode* node = m_gamePark->smgr()->addCubeSceneNode();
    node->setPosition(m_pos);
}
