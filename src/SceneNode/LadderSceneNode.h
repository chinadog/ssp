#ifndef LADDERSCENENODE_H
#define LADDERSCENENODE_H

#include <irrlicht.h>
#include <Common/SignalSlot.h>

using namespace irr;

class GamePark;

class LadderSceneNode
{
public:
    LadderSceneNode(GamePark* gamePark);
    ~LadderSceneNode(){m_node->drop();}
    scene::ISceneNode* node();
    void draw();
    void setPosition(const core::vector3df& value);
    void setRotation(const core::vector3df& value);
    void setScale(const core::vector3df& value);

    Signal<> enter;
    Signal<> leave;
private:
    void initNode();

    scene::ISceneNode* m_node;
    GamePark* m_gamePark;
    scene::ITriangleSelector* m_boxSelector = nullptr;
    scene::ISceneCollisionManager* cmgr;
    bool m_intersects = false;
    bool m_climb = false;
};

#endif // LADDERSCENENODE_H
