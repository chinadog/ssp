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
    scene::ISceneNode* node();
    void draw();

    Signal<> enter;
    Signal<> leave;
private:
    void initNode();

    scene::ISceneNode* m_node;
    scene::ISceneNode* m_emptyNode;
    GamePark* m_gamePark;
    scene::ITriangleSelector* m_boxSelector = nullptr;
    scene::ISceneCollisionManager* cmgr;
    bool m_intersects = false;
    bool m_climb = false;
};

#endif // LADDERSCENENODE_H
