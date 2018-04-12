#ifndef AI_H
#define AI_H

#include <irrlicht.h>
#include "Player/Player.h"
#include <iostream>

using namespace irr;

class AI
{
public:
    enum
    {
        // I use this ISceneNode ID to indicate a scene node that is
        // not pickable by getSceneNodeAndCollisionPointFromRay()
        ID_IsNotPickable = 0,

        // I use this flag in ISceneNode IDs to indicate that the
        // scene node can be picked by ray selection.
        IDFlag_IsPickable = 1 << 0,

        // I use this flag in ISceneNode IDs to indicate that the
        // scene node can be highlighted.  In this example, the
        // homonids can be highlighted, but the level mesh can't.
        IDFlag_IsHighlightable = 1 << 1
    };

    AI(irr::IrrlichtDevice* device, Player* player);
    virtual ~AI();

    virtual void draw();


    f32 m_deltaTime = 0;
    u32 m_prevTime = 0;






    virtual void setPosition(const core::vector3df& pos);
    void setTerrain(scene::ITerrainSceneNode* terrain);
    virtual irr::core::vector3df ellipsoid() const = 0;
    virtual irr::core::vector3df ellipsoidTranslation() const = 0;
    void gotoPlayer(f32 timeInSeconds);
    void showEllipsoid();
    inline irr::video::ITexture* texture(const irr::io::path &filename)
           {return m_driver->getTexture(irr::core::stringc("../../media/textures/")+filename);}
    inline scene::IAnimatedMeshSceneNode* node() {return m_node;}
    inline f32 health() const {return m_health;}
    inline void setHealth(f32 value) {m_health = value;}
private:
    void createGravitation();
    void moveNode(const core::vector3df& pos, f32 timeInSeconds);
protected:
    irr::scene::ISceneManager* m_smgr = nullptr;
    irr::video::IVideoDriver* m_driver = nullptr;
    irr::IrrlichtDevice* m_device = nullptr;
    Player* m_player = nullptr;
    scene::IAnimatedMeshSceneNode* m_node = nullptr;
    scene::ITerrainSceneNode* m_terrain = nullptr;
    bool m_movable = true;
    f32 m_health = 1.0;
};

#endif // AI_H
