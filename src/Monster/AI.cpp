#include "AI.h"
#include "Common/Common.h"
#include "cmath"
#include "Common/Collision.h"

AI::AI(irr::IrrlichtDevice* device, Player* player) :
    m_smgr(device->getSceneManager()),
    m_driver(device->getVideoDriver()),
    m_device(device),
    m_player(player)
{

}

AI::~AI()
{
//    m_node->removeAnimators();
//    m_node->remove();
//    m_node = nullptr;
    std::cout << "Destruuuctor ai" << std::endl;
}

void AI::draw()
{
    u32 now = m_device->getTimer()->getTime();
    if(m_prevTime == 0)
    {
        m_prevTime = now;
    }
    m_deltaTime = (f32)(now - m_prevTime) / 1000.f;
    m_prevTime = now;

    gotoPlayer(m_deltaTime);
}

void AI::setPosition(const core::vector3df &pos)
{
    m_node->setPosition(pos);
}

void AI::setTerrain(scene::ITerrainSceneNode *terrain)
{
    m_terrain = terrain;
    createGravitation();
}

void AI::gotoPlayer(f32 timeInSeconds)
{
    if(m_node == nullptr || m_movable == false)
    {
        return;
    }

    moveNode( m_player->camera()->getAbsolutePosition(), timeInSeconds );
}

void AI::showEllipsoid()
{
    core::vector3df pos = m_node->getAbsolutePosition()-ellipsoidTranslation();

    m_driver->draw3DLine(core::vector3df(pos.X,pos.Y+ellipsoid().Y/2,pos.Z),
                         core::vector3df(pos.X,pos.Y-ellipsoid().Y/2,pos.Z),
                         video::SColor(255,255,255,255));
    m_driver->draw3DLine(core::vector3df(pos.X+ellipsoid().X/2,pos.Y,pos.Z),
                         core::vector3df(pos.X-ellipsoid().X/2,pos.Y,pos.Z),
                         video::SColor(255,255,255,255));
    m_driver->draw3DLine(core::vector3df(pos.X,pos.Y,pos.Z+ellipsoid().Z/2),
                         core::vector3df(pos.X,pos.Y,pos.Z-ellipsoid().Z/2),
                         video::SColor(255,255,255,255));
}

void AI::createGravitation()
{
    // создаем аниматор столкновений с селектором и прикрепляем его к игроку
    scene::ISceneNodeAnimator* anim = m_smgr->createCollisionResponseAnimator(
        m_terrain->getTriangleSelector(), m_node, ellipsoid(),
        core::vector3df(0,-8,0),
        ellipsoidTranslation());
    m_node->addAnimator(anim);
    anim->drop();

//    Collision::setCollision( m_node,m_player,m_smgr);
}

void AI::moveNode(const core::vector3df &pos, f32 timeInSeconds)
{
    f32 x = m_node->getPosition().X;
    f32 y = m_node->getPosition().Y;
    f32 z = m_node->getPosition().Z;
    //считаем дистанцию (длину от точки А до точки Б). формула длины вектора
    f32 distance = std::sqrt((pos.X - x)*(pos.X - x) + (pos.Z - z)*(pos.Z - z));
    if(distance > 10)
    {
        x += timeInSeconds*450/60.0*(pos.X - x) / distance;//идем по иксу с помощью вектора нормали
        z += timeInSeconds*450/60.0*(pos.Z - z) / distance;//идем по игреку так же
        m_node->setPosition(core::vector3df(x,y,z));
        core::vector3df r = pos - m_node->getAbsolutePosition();
        f32 arc = atan2(r.X, r.Z);

        f32 newX = arc*180/M_PI;


        r.set( 0, newX, 0);

        m_node->setRotation( r );

    }
}
