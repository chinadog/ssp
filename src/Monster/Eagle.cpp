#include "Eagle.h"
#include "Common/Common.h"
#include "GamePark.h"
#include "cmath"
#include "Common/Logger.h"

Eagle::Eagle(GamePark *gamePark) :
    Enemy(gamePark)
{
    init();
    m_speedOfTime = m_gamePark->speedOfTime();
    m_metaTriangleSelector = m_smgr->createMetaTriangleSelector();
    m_boxSelector = m_smgr->createTriangleSelectorFromBoundingBox(m_node);
    m_octreeSelector = m_smgr->createOctreeTriangleSelector(m_node->getMesh(), m_node, 32);
    m_speedOfTimeChangedSignalId = m_gamePark->speedOfTimeChanged.connect_member(this,&Eagle::setSpeedOfTime);
    m_atackDistance = 10.0;
    m_walkSpeed = m_gamePark->m_config.monster().walkSpeed()*2;
    m_atackSpeed = 0.0;
}

Eagle::~Eagle()
{
    Log::log("Destructr eagle");
    m_collisionAnimator->drop();
    m_boxSelector->drop();
    m_octreeSelector->drop();
    m_metaTriangleSelector->drop();
    m_gamePark->speedOfTimeChanged.disconnect(m_speedOfTimeChangedSignalId);
    m_smgr->addToDeletionQueue(m_node);
}

void Eagle::draw()
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
        flyGotoPlayer(m_deltaTime);
    }
}

scene::ISceneNode *Eagle::node() const
{
    return m_node;
}

void Eagle::setPosition(const core::vector3df pos)
{
    Enemy::setPosition(pos);

    m_center = pos;
    m_center.X += 50;

//    core::vector3df center = m_node->getPosition();
//    f32 radius = 3900.0f/60.0;
//    f32 speed = 0.05f;
//    m_center = center;

//    m_node->removeAnimator(m_flyAnim);
//    m_node->removeAnimator(m_rotAnim);

//    m_flyAnim = m_smgr->createFlyCircleAnimator(center, radius, (2*core::PI/1000.0f)*speed,
//                                                core::vector3df(0,1,0), 0.7);
//    m_node->addAnimator(m_flyAnim);
//    m_rotAnim = m_smgr->createRotationAnimator(core::vector3df(0,(3.6)*speed,0));
//    m_node->addAnimator(m_rotAnim);

    TDEBUG() << "Set pos = " << pos;
}

int Eagle::init()
{
    scene::IAnimatedMesh* mesh;
//    mesh = m_smgr->getMesh(Common::modelsPath()+"Eagle.b3d");
    mesh = m_smgr->getMesh(Common::modelsPath()+"eagle.md2");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    m_node = m_smgr->addAnimatedMeshSceneNode( mesh );
    if (m_node)
    {
//        m_node->setMaterialTexture(0, m_driver->getTexture(Common::texture("eagle.tga")));
        m_node->setMaterialTexture(0, m_driver->getTexture(Common::texture("eagle.tga")));
//        m_node->setMaterialTexture(0, texture("plitka.tga"));
        m_node->setScale(core::vector3df(0.15,0.15,0.15));
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

//    m_node->setRotation(core::vector3df(0,-90,0));

//    core::vector3df center = m_node->getPosition();
//    f32 radius = 3900.0f/60.0;
//    m_rotationSpeed = 0.05f;
//    m_center = center;

//    m_flyAnim = m_smgr->createFlyCircleAnimator(center, radius, (2*core::PI/1000.0f)*m_rotationSpeed);
//    m_node->addAnimator(m_flyAnim);
//    m_rotAnim = m_smgr->createRotationAnimator(core::vector3df(0,(3.6)*m_rotationSpeed,0));
//    m_node->addAnimator(m_rotAnim);

    return 0;
}

core::vector3df Eagle::ellipsoid() const
{
    return core::vector3df(1,1,1);
}

core::vector3df Eagle::ellipsoidTranslation() const
{
    return core::vector3df(0,0,0);
}

void Eagle::setSpeedOfTime(f32 speed)
{
    m_speedOfTime = speed;
    m_node->setAnimationSpeed(m_afl.speed()*m_speedOfTime);
}

void Eagle::setMonsterState(const MonsterState &state, bool force)
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
        m_afl.setStart(0);
        m_afl.setEnd(15);
        m_afl.setSpeed(12);
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
        m_endCallback.setFunc(this, &Eagle::atackPlayer);
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
        m_endCallback.setFunc(this, &Eagle::woundFinished);
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
        m_endCallback.setFunc(this, &Eagle::dieFinished);
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

void Eagle::woundFinished()
{
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}

void Eagle::dieFinished()
{
    m_life = false;
}

void Eagle::atack()
{
    m_fms.setState(MonsterSignal::Atack);
    setMonsterState(m_fms.currentState());
    if(m_distanceToPlayer <= m_atackDistance)
    {
        std::cout << Log::curTimeC() << "Atack" << std::endl;
        m_player->setHealth(m_player->health()-0.0);
    }
}

void Eagle::walk()
{
    m_fms.setState(MonsterSignal::Go);
    setMonsterState(m_fms.currentState());
}

void Eagle::stopDraw()
{
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}

void Eagle::atackPlayer()
{
    m_intersects = false;
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
}

void Eagle::flyGotoPlayer(f32 deltaTime)
{
    // get angle to player
    core::vector3df playerPos = m_player->camera()->getPosition();
    core::vector3df targetVector = playerPos - m_node->getPosition();
    f32 targetAngle = atan2(targetVector.X, targetVector.Z)*180/M_PI;


    u32 now = m_device->getTimer()->getTime();

    f32 firstAngle = s32(targetAngle + 180) % 360 - 180;
    f32 secondAngle = s32(m_node->getRotation().Y+90 + 180) % 360 - 180;
    TDEBUG() << m_node->getRotation().Y << m_trjChangedAngle << firstAngle << secondAngle;
    if(std::abs(firstAngle - secondAngle) < 2.0 &&
            m_trjChangedAngleOk == false)
    {
        TWARNING() << "Start change trj!!!!--------------------------";
        TDEBUG() << "Cur angle    = " << m_node->getRotation();
        TDEBUG() << "Target angle = " << m_node->getRotation().Y+90*m_sign;
        m_trjChangedAngleOk = true;
        m_trjChangedAngle = m_node->getRotation().Y+90*m_sign;
    }
//    if(now - m_startTime > m_trjChangedTime)
    if(m_trjChangedAngleOk == true &&
            std::abs(cos(m_node->getRotation().Y*M_PI/180.0) - cos(m_trjChangedAngle*M_PI/180.0)) < 0.1 )
    {
        m_trjChangedAngleOk = false;

        TWARNING() << "End change trj!!!!--------------------------";
        TDEBUG() << "Cur angle    = " << m_node->getRotation();
        TDEBUG() << "Target angle = " << m_node->getRotation().Y+90*m_sign;

        m_sign *= -1;
        f32 rand = (m_device->getRandomizer()->rand() % 10 + 5)/ 10.0;
//        f32 rand2 = (m_device->getRandomizer()->rand() % 20 + 5)/ 10.0;
//        m_trjChangedTime = 8000*rand2;
        m_center = (m_node->getPosition() - m_center)*rand + m_node->getPosition();
        m_radius = m_node->getPosition().getDistanceFrom(m_center);
        m_rotationSpeed = m_velocity/m_radius*m_sign;
        if(m_rotationSpeed > 0)
        {
            m_alpha = M_PI + ((m_node->getRotation().Y - 180)*M_PI/180.0);
        }
        else
        {
            m_alpha = M_PI +(m_node->getRotation().Y*M_PI/180.0);
        }

//        TWARNING() << "CHange pos";
//        m_startTime = now;
    }



    core::vector3df pos(m_center.X+m_radius*sin(m_alpha),
                        60,
                        m_center.Z+m_radius*cos(m_alpha));

    core::vector3df r = m_center - pos;
    int sign = (m_rotationSpeed > 0) - (m_rotationSpeed < 0);
    core::vector3df rot;
    if(sign>0)
    {
        rot = core::vector3df(0, atan2(r.X, r.Z)*180/M_PI-180, 0);
    }
    if(sign<0)
    {
        rot = core::vector3df(0, atan2(r.X, r.Z)*180/M_PI, 0);
    }
//    TDEBUG() << "alpha = " << m_alpha;

    m_alpha+=deltaTime*m_rotationSpeed;

    m_node->setPosition(pos);
    m_node->setRotation(rot);
}
