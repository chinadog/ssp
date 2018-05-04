#ifndef ENEMY_H
#define ENEMY_H

#include "Monster/AI.h"
#include <irrlicht.h>

using namespace irr;

class GamePark;

class Enemy : public AI
{
public:
    Enemy(GamePark* gamePark);
    ~Enemy();

    inline f32 health() const {return m_health;}

    inline void setHealth(f32 value) {m_health = value;}

    void draw();
    virtual scene::ISceneNode* node() const = 0;

//private:
    f32 m_health = 1.0;
};

#endif // ENEMY_H
