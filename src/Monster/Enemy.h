#ifndef ENEMY_H
#define ENEMY_H

#include "Monster/AI.h"
#include <irrlicht.h>
#include "Monster/MonsterFMS.h"

using namespace irr;

class GamePark;

class Enemy : public AI
{
public:
    Enemy(GamePark* gamePark);
    ~Enemy();

    inline f32 health() const {return m_health;}

    inline void setHealth(f32 value) {m_health = value;}

    virtual void draw();
    virtual scene::ISceneNode* node() const = 0;

//private:
    f32 m_health = 1.0;

    scene::ITriangleSelector* m_boxSelector = nullptr;


    void addTriangleSelector(Enemy* node);
    void addTriangleSelector(scene::ITriangleSelector *selector);
    void removeAllTriangleSelector();
    void removeTriangleSelector(Enemy* node);
    void updateCollisionAnimator();
    inline scene::IMetaTriangleSelector* metaTriangleSelector() const {return m_metaTriangleSelector;}
    scene::IMetaTriangleSelector *m_metaTriangleSelector = nullptr;
    scene::ISceneNodeAnimatorCollisionResponse* m_collisionAnimator = nullptr;

    void damage(f32 value, const core::vector3df& intersection);

    void dustEffect( const core::vector3df& pos );
    void bloodEffect( const core::vector3df& pos );

    bool isHeadshot(core::vector3df point) const;

    void kill();

    inline bool life() {return m_life;}
    inline void setLife(bool value) {m_life = value;}

    MonsterState m_currentState = MonsterState::Draw;
    int m_speedOfTimeChangedSignalId = -1;
    MonsterFMS m_fms;

    bool m_life = true;

    virtual void setMonsterState(const MonsterState& state, bool force = false) = 0;
};

#endif // ENEMY_H
