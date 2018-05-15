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
        m_aiPosition = node()->getPosition();
        m_aiRotation = node()->getRotation();
//        AI::draw();
        node()->setPosition(m_aiPosition);
        node()->setRotation(m_aiRotation);
    }
}
