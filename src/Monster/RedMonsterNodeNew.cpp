#include "RedMonsterNodeNew.h"
#include "GamePark.h"
#include "Common/Common.h"
#include <cmath>
#include "Common/Logger.h"

RedMonsterNodeNew::RedMonsterNodeNew(GamePark* gamePark) :
    AnimatedMeshSceneNode(0,0,0,0),
    m_gamePark(gamePark)
{
    m_smgr = m_gamePark->smgr();
    m_driver = m_gamePark->driver();
    m_device = m_gamePark->device();
    m_player = m_gamePark->player();

    scene::IAnimatedMesh* mesh = m_smgr->getMesh("../../media/models/monster_red.b3d");
//    m_node = m_smgr->addAnimatedMeshSceneNode( mesh );
//    if (m_node)
//    {
//        f32 rand = m_device->getRandomizer()->rand() % 20 / 100;
//        m_node->setMaterialTexture(0, m_driver->getTexture(Common::texture("monster/red.tga")));
//        m_node->setScale(core::vector3df(0.1+rand,0.1+rand,0.1+rand));
//        m_node->setMaterialFlag(video::EMF_LIGHTING, true);
//        m_node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
//        m_node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
//        m_node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
//        m_node->setName("MonsterNode");
//        irr::scene::ITriangleSelector* selector = m_smgr->createOctreeTriangleSelector(m_node->getMesh(),m_node,128);
//        m_node->setTriangleSelector(selector);
//        selector->drop();
//    }

    setMesh(mesh);
    setParent(m_smgr->getRootSceneNode());
    setSceneManager(m_smgr);
    setID(0);
    setScale(core::vector3df(0.1,0.1,0.1));

    setMaterialTexture(0, m_driver->getTexture("../../media/textures/monster/red.tga"));
    setMaterialFlag(video::EMF_LIGHTING, true);
    setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
    mesh->setHardwareMappingHint(irr::scene::EHM_STATIC);
    setName("MonsterNode");

    m_metaTriangleSelector = m_smgr->createMetaTriangleSelector();
    m_boxSelector = m_smgr->createTriangleSelectorFromBoundingBox(this);
    m_octreeSelector = m_smgr->createOctreeTriangleSelector(this->getMesh(), this, 32);
    m_atackDistance = 6.0;
    TINFO() << "INIT POS=" << getPosition();
}

RedMonsterNodeNew::~RedMonsterNodeNew()
{

}

void RedMonsterNodeNew::setTerrain(scene::ITerrainSceneNode *terrain)
{
    m_terrain = terrain;
    createGravitation();
}

void RedMonsterNodeNew::createGravitation()
{
    m_gravityAnim = m_smgr->createCollisionResponseAnimator(
        m_terrain->getTriangleSelector(), this, ellipsoid(),
        core::vector3df(0,-8,0),
        ellipsoidTranslation());
    addAnimator(m_gravityAnim);
}

core::vector3df RedMonsterNodeNew::ellipsoid() const
{
    return core::vector3df(2,2,2);
}

core::vector3df RedMonsterNodeNew::ellipsoidTranslation() const
{
    return core::vector3df(0,-2,0);
}

void RedMonsterNodeNew::OnAnimate(u32 timeMs)
{
    AnimatedMeshSceneNode::OnAnimate(timeMs);

    if(m_prevTime == 0)
    {
        m_prevTime = timeMs;
    }
    m_deltaTime = (f32)(timeMs - m_prevTime) / 1000.f;
    m_prevTime = timeMs;

    if(m_health > 0.0)
    {
        if(m_isDraw == false)
        {
            gotoPlayer(m_deltaTime);
        }
        else
        {
            core::vector3df pos = getPosition();
            if(m_drawFinishedLevel > pos.Y-2.0)
            {
                m_gravityAnim->setGravity(core::vector3df(0,0,0));
                pos.Y += 25*m_deltaTime*m_speedOfTime; TDEBUG() << "NEW Y=" << pos.Y << getPosition() ;
                setPosition(pos);
                if(std::fabs(m_drawFinishedLevel - pos.Y) < 3.5 && m_layOut == false)
                {
                    m_layOut = true;
                    layOut.Emit();
                    layOut.disconnect_all();
                }
            }
            else
            {
                m_gravityAnim->setGravity(core::vector3df(0,-8,0));
                m_isDraw = false;
                stopDraw();
            }
        }
    }
    TDEBUG() << "POS = " << getPosition();
}

void RedMonsterNodeNew::setMonsterState(const MonsterState &state, bool force)
{
    if(state == m_currentState && force == false)
    {
        return;
    }
    m_currentState = state;

    s32 rand = 0;
    switch (state) {
    case MonsterState::Walk :
        m_afl.setStart(0);
        m_afl.setEnd(240);
        m_afl.setSpeed(25);
        m_afl.setLoop(true);
        m_speed = m_walkSpeed*m_speedOfTime;
        break;
    case MonsterState::Run :
        break;
    case MonsterState::Atack :
        m_afl.setStart(472);
        m_afl.setEnd(500);
        m_afl.setSpeed(25);
        m_afl.setLoop(false);
        m_afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &RedMonsterNodeNew::atackPlayer);
        m_speed = m_atackSpeed*m_speedOfTime;
        break;
    case MonsterState::Wound :
        m_afl.setStart(472);
        m_afl.setEnd(500);
        rand = m_device->getRandomizer()->rand() % 4;
        if(rand > 2)
        {
            m_afl.setStart(325);
            m_afl.setEnd(400);
        }
        m_afl.setSpeed(25);
        m_afl.setLoop(false);
        m_afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &RedMonsterNodeNew::woundFinished);
        m_speed = m_woundSpeed*m_speedOfTime;
        break;
    case MonsterState::Die :
        m_afl.setStart(570);
        m_afl.setEnd(648);
        rand = m_device->getRandomizer()->rand() % 4;
        if(rand > 1)
        {
            m_afl.setStart(500);
            m_afl.setEnd(563);
        }
        m_afl.setSpeed(25);
        m_afl.setLoop(false);
        m_afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &RedMonsterNodeNew::dieFinished);
        m_speed = 0;
        m_isRotated = false;
        break;
    case MonsterState::Draw :
        m_afl.setStart(0);
        m_afl.setEnd(240);
        m_afl.setSpeed(25);
        m_afl.setLoop(true);
        m_speed = m_drawSpeed*m_speedOfTime;
        m_isRotated = true;
        break;
    default:
        break;
    }

    setFrameLoop(m_afl.start(), m_afl.end());
    setAnimationSpeed(m_afl.speed()*m_speedOfTime);
    setLoopMode(m_afl.loop());
    setAnimationEndCallback(m_afl.endCallback());
}

void RedMonsterNodeNew::woundFinished()
{
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}

void RedMonsterNodeNew::dieFinished()
{
    m_life = false;
}

void RedMonsterNodeNew::addTriangleSelector(RedMonsterNodeNew *node)
{
    m_metaTriangleSelector->addTriangleSelector( m_boxSelector );
}

void RedMonsterNodeNew::addTriangleSelector(scene::ITriangleSelector *selector)
{
    m_metaTriangleSelector->addTriangleSelector( selector );
}

void RedMonsterNodeNew::removeAllTriangleSelector()
{
    m_metaTriangleSelector->removeAllTriangleSelectors();
}

void RedMonsterNodeNew::removeTriangleSelector(RedMonsterNodeNew *node)
{
    m_metaTriangleSelector->removeTriangleSelector( m_boxSelector );
}

void RedMonsterNodeNew::updateCollisionAnimator()
{
    removeAnimator(m_collisionAnimator);
    m_collisionAnimator = m_smgr->createCollisionResponseAnimator(
        m_metaTriangleSelector,
        this,ellipsoid(),
        irr::core::vector3df(0,0,0),ellipsoidTranslation());
    addAnimator(m_collisionAnimator);
}

void RedMonsterNodeNew::atack()
{
    m_fms.setState(MonsterSignal::Atack);
    setMonsterState(m_fms.currentState());
    if(m_distanceToPlayer <= m_atackDistance)
    {
        std::cout << Log::curTimeC() << "Atack" << std::endl;
        m_player->setHealth(m_player->health()-0.0);
    }
}

void RedMonsterNodeNew::walk()
{
    m_fms.setState(MonsterSignal::Go);
    setMonsterState(m_fms.currentState());
}

void RedMonsterNodeNew::stopDraw()
{
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}

void RedMonsterNodeNew::atackPlayer()
{
    m_intersects = false;
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}

void RedMonsterNodeNew::gotoPlayer(f32 timeInSeconds)
{
    if(/*m_node == nullptr ||*/ m_movable == false || m_player->health() <= 0.0 /*|| m_health <= 0.0*/)
    {
        return;
    }

    moveNode( m_player->camera()->getAbsolutePosition(), timeInSeconds );
}

void RedMonsterNodeNew::moveNode(const core::vector3df &pos, f32 timeInSeconds)
{
    core::vector3df nodePos = getPosition();
    //считаем дистанцию (длину от точки А до точки Б)
    m_distanceToPlayer = nodePos.getDistanceFrom(pos);

    if(m_distanceToPlayer > m_atackDistance)
    {
        nodePos.X += timeInSeconds*m_speed*m_speedOfTime*(pos.X - nodePos.X) / m_distanceToPlayer;//идем по иксу с помощью вектора нормали
        nodePos.Z += timeInSeconds*m_speed*m_speedOfTime*(pos.Z - nodePos.Z) / m_distanceToPlayer;//идем по игреку так же
        nodePos.Y += 0.02*m_speed*m_speedOfTime;
        setPosition(nodePos);
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
    core::vector3df r = pos - nodePos;
    f32 arc = atan2(r.X, r.Z);
    f32 newX = arc*180/M_PI;
    r.set( 0, newX, 0);

    if(m_isRotated)
    {
        setRotation( r );
    }
}
