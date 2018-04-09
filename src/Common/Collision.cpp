#include "Common/Collision.h"

Collision::Collision()
{

}

void Collision::addAnimator(Player *player, scene::IMetaTriangleSelector* metaTriangle,
                            scene::ISceneManager *smgr)
{
    irr::scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
        metaTriangle,
        player->camera(),player->ellipsoid(),
        irr::core::vector3df(0,0,0),player->ellipsoidTranslation());
    player->camera()->addAnimator(anim);
    anim->drop();
}

void Collision::setMetaCollision(scene::IAnimatedMeshSceneNode *node, scene::ISceneManager *smgr,
                                 scene::IMetaTriangleSelector* metaTriangle)
{
    irr::scene::ITriangleSelector* selector = smgr->createTriangleSelectorFromBoundingBox(node);
    metaTriangle->addTriangleSelector( selector );
    selector->drop();
}

void Collision::setMetaCollision(scene::IMeshSceneNode *node, scene::ISceneManager *smgr,
                                 scene::IMetaTriangleSelector* metaTriangle)
{
    irr::scene::ITriangleSelector* selector = smgr->createOctreeTriangleSelector(node->getMesh(),node,128);
    metaTriangle->addTriangleSelector( selector );
    selector->drop();
}
