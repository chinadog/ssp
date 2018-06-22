#include "Turt.h"
#include "Common/Common.h"
#include "GamePark.h"
#include "cmath"
#include "Common/Logger.h"

Turt::Turt(GamePark *gamePark) :
    Enemy(gamePark)
{
    init();
    m_speedOfTime = m_gamePark->speedOfTime();
    m_metaTriangleSelector = m_smgr->createMetaTriangleSelector();
    m_boxSelector = m_smgr->createTriangleSelectorFromBoundingBox(m_node);
    m_octreeSelector = m_smgr->createOctreeTriangleSelector(m_node->getMesh(), m_node, 32);
    m_speedOfTimeChangedSignalId = m_gamePark->speedOfTimeChanged.connect_member(this,&Turt::setSpeedOfTime);
    m_atackDistance = 10.0;
    m_walkSpeed = m_gamePark->m_config.monster().walkSpeed()*2;
    m_atackSpeed = 0.0;
    m_startRot = 180.0;
}

Turt::~Turt()
{
    Log::log("Destructr turt");
    m_collisionAnimator->drop();
    m_boxSelector->drop();
    m_octreeSelector->drop();
    m_metaTriangleSelector->drop();
    m_gamePark->speedOfTimeChanged.disconnect(m_speedOfTimeChangedSignalId);
    m_smgr->addToDeletionQueue(m_node);
}

void Turt::draw()
{
    u32 now = m_device->getTimer()->getTime();
    if(m_prevTime == 0)
    {
        m_prevTime = now;
    }
    m_deltaTime = (f32)(now - m_prevTime) / 1000.f;
    m_prevTime = now;

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

scene::ISceneNode *Turt::node() const
{
    return m_node;
}

void Turt::setPosition(const core::vector3df pos)
{
    Enemy::setPosition(pos);
}

int Turt::init()
{
    scene::IAnimatedMesh* mesh;
//    mesh = m_smgr->getMesh(Common::modelsPath()+"turt.b3d");
    mesh = m_smgr->getMesh(Common::modelsPath()+"torto.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    m_node = m_smgr->addAnimatedMeshSceneNode( mesh );
    if (m_node)
    {
//        m_node->setMaterialTexture(0, m_driver->getTexture(Common::texture("turt3.png")));
        m_node->setMaterialTexture(0, m_driver->getTexture(Common::texture("torto.png")));
//        m_node->setMaterialTexture(0, texture("plitka.tga"));
        m_node->setScale(core::vector3df(0.5,0.5,0.5));
        m_node->setMaterialFlag(video::EMF_LIGHTING, true);
        m_node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        m_node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
//        m_node->getMaterial(0).Shininess=1.0;
//        m_node->getMaterial(0).AmbientColor = video::SColor(255,255,0,0);
//        m_node->getMaterial(0).DiffuseColor = video::SColor(0,0,0,0);
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

core::vector3df Turt::ellipsoid() const
{
    return core::vector3df(1,1,1);
}

core::vector3df Turt::ellipsoidTranslation() const
{
    return core::vector3df(0,0,0);
}

void Turt::setSpeedOfTime(f32 speed)
{
    m_speedOfTime = speed;
    m_node->setAnimationSpeed(m_afl.speed()*m_speedOfTime);
}

void Turt::setMonsterState(const MonsterState &state, bool force)
{
    if(state == m_currentState && force == false)
    {
        return;
    }
    m_currentState = state;
    TINFO() << (int)m_currentState;

    s32 rand = 0;
    switch (state) {
    case MonsterState::Walk :
        m_afl.setStart(30);
        m_afl.setEnd(69);
        m_afl.setSpeed(45);
        m_afl.setLoop(true);
        m_speed = m_walkSpeed*m_speedOfTime;
        break;
    case MonsterState::Run :
        break;
    case MonsterState::Atack :
        m_afl.setStart(30);
        m_afl.setEnd(69);
        m_afl.setSpeed(25);
        m_afl.setLoop(false);
        m_afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &Turt::atackPlayer);
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
        m_endCallback.setFunc(this, &Turt::woundFinished);
        m_speed = m_woundSpeed*m_speedOfTime;
        break;
    case MonsterState::Die :
//        m_afl.setStart(200);
//        m_afl.setEnd(340);
        m_afl.setStart(115);
        m_afl.setEnd(160);
        m_afl.setSpeed(25);
        m_afl.setLoop(false);
        m_afl.setEndCallback(&m_endCallback);
        m_endCallback.setFunc(this, &Turt::dieFinished);
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

void Turt::woundFinished()
{
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}

void Turt::dieFinished()
{
    m_life = false;
}

void Turt::atack()
{
    m_fms.setState(MonsterSignal::Atack);
    setMonsterState(m_fms.currentState());
    if(m_distanceToPlayer <= m_atackDistance)
    {
        std::cout << Log::curTimeC() << "Atack" << std::endl;
        m_player->setHealth(m_player->health()-0.0);
    }
}

void Turt::walk()
{
    m_fms.setState(MonsterSignal::Go);
    setMonsterState(m_fms.currentState());
}

void Turt::stopDraw()
{
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}

void Turt::atackPlayer()
{
    m_intersects = false;
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}
