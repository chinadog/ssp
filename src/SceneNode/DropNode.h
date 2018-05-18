#ifndef DROPNODE_H
#define DROPNODE_H

#include <irrlicht.h>
#include "SceneNode/MeshSceneNode.h"
#include "Common/SignalSlot.h"

using namespace irr;

class GamePark;
class DropNode : public MeshSceneNode
{
public:
    enum class Type
    {
        Weapon,
        Count
    };

    //! constructor
    DropNode(GamePark* gamePark,
            scene::IMesh* mesh, ISceneNode* parent, scene::ISceneManager* mgr,	s32 id,
        const core::vector3df& position = core::vector3df(0,0,0),
        const core::vector3df& rotation = core::vector3df(0,0,0),
        const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

    //! destructor
    virtual ~DropNode();

    void OnAnimate(u32 timeMs);

    Signal<DropNode*> active;
    virtual Type type() const {return Type::Weapon;}
protected:
    GamePark* m_gamePark = nullptr;

};

#endif // DROPNODE_H
