#ifndef MONSTERNODE_H
#define MONSTERNODE_H

#include <irrlicht.h>
#include "AI.h"
#include "Monster/Enemy.h"
#include <list>
#include "Monster/MonsterFMS.h"

using namespace irr;

class DieEndCallBack;

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
    void kill();
    void damage(f32 value, const core::vector3df& intersection);
    void die(DieEndCallBack* callback);
    inline bool life() {return m_life;}
    inline void setLife(bool value) {m_life = value;}
    void addCallback(scene::IAnimationEndCallBack* callback);

    void remove();
    void dustEffect( const core::vector3df& pos );
    void bloodEffect( const core::vector3df& pos );

    void addTriangleSelector(MonsterNode* node);
    void addTriangleSelector(scene::ITriangleSelector *selector);
    void removeAllTriangleSelector();
    void removeTriangleSelector(MonsterNode* node);
    void updateCollisionAnimator();
    inline scene::IMetaTriangleSelector* metaTriangleSelector() const {return m_metaTriangleSelector;}
    scene::ISceneNodeAnimatorCollisionResponse* m_collisionAnimator = nullptr;

    scene::ITriangleSelector* m_boxSelector = nullptr;

    bool isHeadshot(core::vector3df point) const;

    void setSpeedOfTime(f32 speed);

    inline void setDrawFinishedLevel(f32 value){m_drawFinishedLevel = value;}

    virtual void setMonsterState(const MonsterState& state, bool force = false);
    void woundFinished();
    void atack();
    void walk();
    void stopDraw();
    void atackPlayer();
private:
    bool m_life = true;


    AnimationEndCallback m_endCallback;
    std::list<scene::IAnimationEndCallBack*> m_callbackList;

    f32 m_drawFinishedLevel = 0.0;



protected:
    MonsterState m_currentState = MonsterState::Draw;
    scene::IMetaTriangleSelector *m_metaTriangleSelector = nullptr;
    int m_speedOfTimeChangedSignalId = -1;
    MonsterFMS m_fms;

    f32 m_walkSpeed = 7.5;
    f32 m_woundSpeed = 1.5;
    f32 m_atackSpeed = 1.5;
    f32 m_drawSpeed = 0.5;
};

//--------------------------------------------------------------------------

class DieEndCallBack : public scene::IAnimationEndCallBack
{
public:
    DieEndCallBack(MonsterNode* parent = 0);
    virtual void OnAnimationEnd(scene::IAnimatedMeshSceneNode* node);
private:
    MonsterNode* m_parent = nullptr;
};


#endif // MONSTERNODE_H
