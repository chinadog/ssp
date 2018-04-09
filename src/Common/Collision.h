#ifndef COLLISION_H
#define COLLISION_H

#include <irrlicht.h>
#include "Player/Player.h"

class Collision
{
public:
    Collision();

    static void setCollision(irr::scene::IAnimatedMeshSceneNode *node, Player *player,
                             irr::scene::ISceneManager* smgr)
    {
        irr::scene::ITriangleSelector* selector = smgr->createOctreeTriangleSelector(node->getMesh(),node,128);
        node->setTriangleSelector(selector);
        irr::scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
            selector,
            player->camera(),player->ellipsoid(),
            irr::core::vector3df(0,0,0),player->ellipsoidTranslation());
        selector->drop();
        player->camera()->addAnimator(anim);
        anim->drop();
    }

    static void setCollision(irr::scene::IMeshSceneNode *node, Player *player,
                             irr::scene::ISceneManager* smgr)
    {
        irr::scene::ITriangleSelector* selector = smgr->createOctreeTriangleSelector(node->getMesh(),node,128);
        node->setTriangleSelector(selector);
        irr::scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
            selector,
            player->camera(),player->ellipsoid(),
            irr::core::vector3df(0,0,0),player->ellipsoidTranslation());
        selector->drop();
        player->camera()->addAnimator(anim);
        anim->drop();
    }


    static void addAnimator(Player *player, scene::IMetaTriangleSelector* metaTriangle,
                            scene::ISceneManager *smgr);
    static void setMetaCollision(irr::scene::IAnimatedMeshSceneNode *node,
                             irr::scene::ISceneManager* smgr, scene::IMetaTriangleSelector* metaTriangle);
    static void setMetaCollision(irr::scene::IMeshSceneNode *node,
                             irr::scene::ISceneManager* smgr, scene::IMetaTriangleSelector* metaTriangle);
};



#endif // COLLISION_H
