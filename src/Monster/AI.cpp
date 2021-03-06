#include "AI.h"
#include "Common/Common.h"
#include "cmath"
#include "Common/Collision.h"
#include "GamePark.h"

#ifdef _WIN32
    const double M_PI = 3.1415926535897932384626433832795;
#endif

AI::AI(GamePark* gamePark) :
    m_gamePark(gamePark)
{
    m_smgr = m_gamePark->device()->getSceneManager();
    m_driver = m_gamePark->device()->getVideoDriver();
    m_device = m_gamePark->device();
    m_player = m_gamePark->player();
}

AI::~AI()
{
//    m_node->removeAnimators();
//    m_node->remove();
//    m_node = nullptr;
    std::cout << "Destruuuctor ai" << std::endl;
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
    if(/*m_node == nullptr ||*/ m_movable == false || m_player->health() <= 0.0 /*|| m_health <= 0.0*/)
    {
        return;
    }

    moveNode( m_player->camera()->getAbsolutePosition(), timeInSeconds );
}

//void AI::showEllipsoid()
//{
//    core::vector3df pos = m_node->getAbsolutePosition()-ellipsoidTranslation();

//    m_driver->draw3DLine(core::vector3df(pos.X,pos.Y+ellipsoid().Y/2,pos.Z),
//                         core::vector3df(pos.X,pos.Y-ellipsoid().Y/2,pos.Z),
//                         video::SColor(255,255,255,255));
//    m_driver->draw3DLine(core::vector3df(pos.X+ellipsoid().X/2,pos.Y,pos.Z),
//                         core::vector3df(pos.X-ellipsoid().X/2,pos.Y,pos.Z),
//                         video::SColor(255,255,255,255));
//    m_driver->draw3DLine(core::vector3df(pos.X,pos.Y,pos.Z+ellipsoid().Z/2),
//                         core::vector3df(pos.X,pos.Y,pos.Z-ellipsoid().Z/2),
//                         video::SColor(255,255,255,255));
//}

void AI::createGravitation()
{
    // создаем аниматор столкновений с селектором и прикрепляем его к игроку
    m_gravityAnim = m_smgr->createCollisionResponseAnimator(
        m_terrain->getTriangleSelector(), m_node, ellipsoid(),
        core::vector3df(0,-8,0),
        ellipsoidTranslation());
    m_node->addAnimator(m_gravityAnim);
//    m_gravityAnim->drop();

//    Collision::setCollision( m_node,m_player,m_smgr);
}

void AI::moveNode(const core::vector3df &pos, f32 timeInSeconds)
{
    f32 x = m_node->getPosition().X;
    f32 y = m_node->getPosition().Y;
    f32 z = m_node->getPosition().Z;
    //считаем дистанцию (длину от точки А до точки Б). формула длины вектора
    m_distanceToPlayer = m_node->getPosition().getDistanceFrom(pos);

    if(m_distanceToPlayer > m_atackDistance)
    {
        x += timeInSeconds*m_speed*m_speedOfTime*(pos.X - x) / m_distanceToPlayer;//идем по иксу с помощью вектора нормали
        z += timeInSeconds*m_speed*m_speedOfTime*(pos.Z - z) / m_distanceToPlayer;//идем по игреку так же
//        y += 0.02*m_speed*m_speedOfTime;
        m_node->setPosition(core::vector3df(x,y,z));
        m_aiPosition = core::vector3df(x,y,z); // new
        if(m_intersects == true)
        {
            m_intersects = false;
            walk();
        }
    }
    else
    {
        if(m_intersects == false )
        {
            m_intersects = true;
            atack();
        }
    }
    core::vector3df r = pos - m_node->getAbsolutePosition();
    f32 arc = atan2(r.X, r.Z);

    f32 newX = arc*180/M_PI;

    r.set( 0, newX, 0);

    if(m_isRotated)
    {
        m_node->setRotation( r );
        m_aiRotation = r; // new
    }


// NEW (OPTIMIZED) ! BUG TODO (Прыгает при приближении черепах)
//    core::vector3df targetPos = pos - m_node->getPosition();
//    targetPos.Y = m_node->getPosition().Y;

//    if(targetPos.getLength() > m_atackDistance)  // if distanecToPlayer > atackDistan
//    {
//        targetPos.normalize() *= timeInSeconds*m_speed*m_speedOfTime;
//TDEBUG() << "MOVE = " << targetPos.getLength() << targetPos;
//        m_node->setPosition(m_node->getPosition() + targetPos);
//        if(m_intersects == true) {m_intersects = false; walk();}
//    }
//    else
//    {
//        if(m_intersects == false ) {m_intersects = true; atack();}
//    }

//    f32 rot = atan2(targetPos.X, targetPos.Z)*180/M_PI + m_startRot;
//    if(m_isRotated)
//    {
//        m_node->setRotation(core::vector3df(0,rot,0));
//    }
}
