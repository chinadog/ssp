#include "MonsterNode.h"
#include "Common/Common.h"
#include "GamePark.h"
#include "cmath"


MonsterNode::MonsterNode(GamePark* gamePark) :
    Enemy(gamePark)
{

}

MonsterNode::~MonsterNode()
{
    Log::log("Destructr monster");
    m_collisionAnimator->drop();
    m_boxSelector->drop();
    m_octreeSelector->drop();
    m_metaTriangleSelector->drop();
    m_gamePark->speedOfTimeChanged.disconnect(m_speedOfTimeChangedSignalId);
    m_smgr->addToDeletionQueue(m_node);
}

void MonsterNode::draw()
{
    u32 now = m_device->getTimer()->getTime();
    if(m_prevTime == 0)
    {
        m_prevTime = now;
    }
    m_deltaTime = (f32)(now - m_prevTime) / 1000.f;
    m_prevTime = now;

//    scene::IBoneSceneNode* headBone = m_node->getJointNode("Bip01 Head");
//    core::line3d<f32> ray;
//    ray.start = headBone->getAbsolutePosition();
//    ray.end = m_player->node()->getAbsolutePosition();
//    ray.end.X += 0.01;

//    video::SMaterial m;
//    m.Lighting=false;
//    m_gamePark->driver()->setMaterial(m);
//    m_gamePark->driver()->setTransform(video::ETS_WORLD, core::matrix4());
//    m_gamePark->driver()->draw3DLine(ray.start, ray.end);

    if(m_health > 0.0)
    {
        if(m_isDraw == false)
        {
            gotoPlayer(m_deltaTime);
        }
        else
        {
            core::vector3df pos = m_node->getPosition();
            if(m_drawFinishedLevel > pos.Y-2.0)
            {
                m_gravityAnim->setGravity(core::vector3df(0,0,0));
                pos.Y += 25*m_deltaTime*m_speedOfTime;
                m_node->setPosition(pos);
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
}

scene::ISceneNode *MonsterNode::node() const
{
    return m_node;
}

void MonsterNode::setPosition(const core::vector3df pos)
{
    Enemy::setPosition(pos);
}

int MonsterNode::init()
{
    scene::IAnimatedMesh* mesh;
    mesh = m_smgr->getMesh(Common::modelsPath()+"monster.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    m_node = m_smgr->addAnimatedMeshSceneNode( mesh );
    if (m_node)
    {
        m_node->setMaterialTexture(0, m_driver->getTexture(Common::texture("monster/green.tga")));
//        m_node->setMaterialTexture(0, texture("plitka.tga"));
        m_node->setScale(core::vector3df(7.0f/60.0,7.0f/60.0,7.0f/60.0));
        m_node->setMaterialFlag(video::EMF_LIGHTING, true);
        m_node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        m_node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
        m_node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
        m_node->setName("MonsterNode");
        irr::scene::ITriangleSelector* selector = m_smgr->createOctreeTriangleSelector(m_node->getMesh(),m_node,128);
        m_node->setTriangleSelector(selector);
        selector->drop();
    }
    setMonsterState(m_currentState,true);

//    m_node->setFrameLoop(0,240);
//    m_node->setAnimationSpeed(25+m_device->getRandomizer()->rand() % 10);
    return 0;
}

core::vector3df MonsterNode::ellipsoid() const
{
    return core::vector3df(2,2,2);
}

core::vector3df MonsterNode::ellipsoidTranslation() const
{
    return core::vector3df(0,-2,0);
}


void MonsterNode::setSpeedOfTime(f32 speed)
{
    m_speedOfTime = speed;
    m_node->setAnimationSpeed(m_afl.speed()*m_speedOfTime);
}

void MonsterNode::setMonsterState(const MonsterState &state, bool force)
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
        m_endCallback.setFunc(this, &MonsterNode::atackPlayer);
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
        m_endCallback.setFunc(this, &MonsterNode::woundFinished);
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
        m_endCallback.setFunc(this, &MonsterNode::dieFinished);
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

    m_node->setFrameLoop(m_afl.start(), m_afl.end());
    m_node->setAnimationSpeed(m_afl.speed()*m_speedOfTime);
    m_node->setLoopMode(m_afl.loop());
    m_node->setAnimationEndCallback(m_afl.endCallback());
}

void MonsterNode::woundFinished()
{
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}

void MonsterNode::dieFinished()
{
    m_life = false;
}

void MonsterNode::atack()
{
    m_fms.setState(MonsterSignal::Atack);
    setMonsterState(m_fms.currentState());
    if(m_distanceToPlayer <= m_atackDistance)
    {
        std::cout << Log::curTimeC() << "Atack" << std::endl;
        m_player->setHealth(m_player->health()-0.0);
    }
}

void MonsterNode::walk()
{
    m_fms.setState(MonsterSignal::Go);
    setMonsterState(m_fms.currentState());
}

void MonsterNode::stopDraw()
{
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}

void MonsterNode::atackPlayer()
{

    m_intersects = false;
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}



// old delete
//    if(m_node)
//    {
//        m_node->removeAll();
//        m_node->removeAnimators();
//        m_node->remove();
//        m_node = nullptr;
//    }
