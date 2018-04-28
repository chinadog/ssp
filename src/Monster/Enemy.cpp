#include "Enemy.h"

Enemy::Enemy(GamePark* gamePark) :
    AI(gamePark)
{

}

Enemy::~Enemy()
{

}

void Enemy::draw()
{
    if(m_health > 0.0)
    {
        AI::draw();
        m_node->setPosition(m_position);
        m_node->setRotation(m_rotation);
    }
}
