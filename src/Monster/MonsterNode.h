#ifndef MONSTERNODE_H
#define MONSTERNODE_H

#include <irrlicht.h>
#include "AI.h"
#include <list>
#include "Monster/MonsterFMS.h"

using namespace irr;

class DieEndCallBack;

class MonsterNode : public AI
{
public:
    MonsterNode(irr::IrrlichtDevice* device, Player* player);
    ~MonsterNode();

    int init();
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
    scene::ITriangleSelector* m_octreeSelector = nullptr;

    void setMonsterState(const MonsterState& state, bool force = false);
    void woundFinished();
    void atack();
    void walk();
    void stopDraw();
    void atackPlayer();
private:
    bool m_life = true;
    MonsterFMS m_fms;
    MonsterState m_currentState = MonsterState::Draw;
    AnimationEndCallback m_endCallback;
    std::list<scene::IAnimationEndCallBack*> m_callbackList;
    scene::IMetaTriangleSelector *m_metaTriangleSelector = nullptr;

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
