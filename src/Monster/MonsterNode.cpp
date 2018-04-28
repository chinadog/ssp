#include "MonsterNode.h"
#include "Common/Common.h"
#include "GamePark.h"

MonsterNode::MonsterNode(GamePark* gamePark) :
    Enemy(gamePark)
{
    init();
    m_speedOfTime = m_gamePark->speedOfTime();
    m_metaTriangleSelector = m_smgr->createMetaTriangleSelector();
    m_boxSelector = m_smgr->createTriangleSelectorFromBoundingBox(m_node);
    m_octreeSelector = m_smgr->createOctreeTriangleSelector(m_node->getMesh(), m_node, 32);
    m_speedOfTimeChangedSignalId = m_gamePark->speedOfTimeChanged.connect_member(this,&MonsterNode::setSpeedOfTime);
}

MonsterNode::~MonsterNode()
{
    Log::log("Destructr monster");
    m_collisionAnimator->drop();
    m_boxSelector->drop();
    m_octreeSelector->drop();
    m_metaTriangleSelector->drop();
    m_gamePark->speedOfTimeChanged.disconnect(m_speedOfTimeChangedSignalId);
    remove();
}

void MonsterNode::draw()
{
    Enemy::draw();
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

void MonsterNode::kill()
{
    m_fms.setState(MonsterSignal::Die);
    setMonsterState(m_fms.currentState());

//    m_node->setLoopMode(false);
//    m_node->setFrameLoop(570, 648);
//    m_node->setAnimationSpeed(25);
//    m_node->setAnimationEndCallback(new DieEndCallBack(this));
//    m_movable = false;
    dustEffect( m_node->getAbsolutePosition() );
}

void MonsterNode::damage(f32 value, const core::vector3df& intersection)
{
    if(health() > value)
    {
        setHealth(health() - value);
        bloodEffect( intersection );
//        s32 rand = m_device->getRandomizer()->rand() % 4;
//        std::cout << Log::curTimeC() << "Rand = " << rand << std::endl;
//        if(rand > 2)
//        {
//            m_fms.setState(MonsterSignal::Wound);
//            setMonsterState(m_fms.currentState());
//        }
    }
    else if(m_movable==true)
    {
        setHealth(health() - value);
        bloodEffect( intersection );
        kill();
    }
}

void MonsterNode::die(DieEndCallBack* callback)
{
    m_life = false;
    m_node->setAnimationEndCallback(0);
    delete callback;
}

void MonsterNode::addCallback(scene::IAnimationEndCallBack *callback)
{
    m_callbackList.push_back( callback );
}

void MonsterNode::remove()
{
    if(m_node)
    {
        m_node->removeAll();
        m_node->removeAnimators();
        m_node->remove();
        m_node = nullptr;
    }
}

void MonsterNode::dustEffect(const core::vector3df& pos)
{

    scene::IParticleSystemSceneNode* ps =
        m_smgr->addParticleSystemSceneNode(false);

    scene::IParticleEmitter* em = ps->createBoxEmitter(
        core::aabbox3d<f32>(-70/60.0,0,-70/60.0,70/60.0,10/60.0,70/60.0), // emitter size
        core::vector3df(0.0f,0.0005f,0.0f),   // initial direction
        8,10,                             // emit rate
        video::SColor(0,255,255,255),       // darkest color
        video::SColor(0,255,255,255),       // brightest color
        800,2000,0,                         // min and max age, angle
        core::dimension2df(100.f/60.0,100.f/60.0),         // min size
        core::dimension2df(200.f/60.0,200.f/60.0));        // max size

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

//    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setPosition(pos);
    ps->setScale(core::vector3df(2,2,2));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, m_driver->getTexture(Common::texture("dust2.png")));
    ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    scene::ISceneNodeAnimator* sna = m_smgr->createDeleteAnimator(1800);
    ps->addAnimator(sna);
    sna->drop();
}

void MonsterNode::bloodEffect(const core::vector3df &pos)
{
    bool small = false;

    scene::IParticleSystemSceneNode* ps =
        m_smgr->addParticleSystemSceneNode(false);

    if(small)
    {
        scene::IParticleEmitter* em = ps->createSphereEmitter(core::vector3df(0,0,0),0.2);
        em->setMaxLifeTime(300);
        em->setMinLifeTime(200);
        em->setMinParticlesPerSecond(25);
        em->setMaxParticlesPerSecond(25);
        em->setMinStartSize(core::dimension2df(0.5,1));
        em->setMaxStartSize(core::dimension2df(1,1.5));
        em->setDirection(core::vector3df(0,0.0010,0));
        em->setMinStartColor(video::SColor(0,109,30,30));
        em->setMaxStartColor(video::SColor(0,109,90,30));
        em->setMaxStartColor(video::SColor(0,109,30,30));

        ps->setEmitter(em);
        em->drop();

        scene::IParticleAffector* paf = ps->createScaleParticleAffector(core::dimension2df(14,-5));
        scene::IParticleAffector* paf2 = ps->createFadeOutParticleAffector(video::SColor(0,100,70,30),300);
        scene::IParticleAffector* paf3 = ps->createGravityAffector(core::vector3df(0,-0.019,0));
        scene::IParticleAffector* paf4 = ps->createRotationAffector(core::vector3df(0.2,0.2,0.2));

        ps->addAffector(paf);
        ps->addAffector(paf2);
        ps->addAffector(paf3);
    //    ps->addAffector(paf4);

        paf->drop();
        paf2->drop();
        paf3->drop();
        paf4->drop();

        ps->setPosition(core::vector3df(200,15,900));
        ps->setMaterialFlag(video::EMF_LIGHTING, false);
        ps->setMaterialTexture(0, m_driver->getTexture(Common::texture("blood_bw.tga")));
        ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        ps->setPosition(pos);

        scene::ISceneNodeAnimator* sna = m_smgr->createDeleteAnimator(300);
        ps->addAnimator(sna);
        sna->drop();
    }
    else
    {
        scene::IParticleEmitter* em = ps->createSphereEmitter(core::vector3df(0,0,0),0.2);
        em->setMaxLifeTime(300 / m_speedOfTime);
        em->setMinLifeTime(200 / m_speedOfTime);
        em->setMinParticlesPerSecond(55 * m_speedOfTime);
        em->setMaxParticlesPerSecond(55 * m_speedOfTime);
        em->setMinStartSize(core::dimension2df(0.5,1));
        em->setMaxStartSize(core::dimension2df(1,1.5));
        em->setDirection(core::vector3df(0,0.0000,0));
        em->setMinStartColor(video::SColor(0,109,30,30));
        em->setMaxStartColor(video::SColor(0,109,90,30));
        em->setMaxStartColor(video::SColor(0,109,30,30));

        ps->setEmitter(em);
        em->drop();

        scene::IParticleAffector* paf = ps->createScaleParticleAffector(core::dimension2df(3,3));
        scene::IParticleAffector* paf2 = ps->createFadeOutParticleAffector(video::SColor(0,100,70,30),300 / m_speedOfTime);
        scene::IParticleAffector* paf3 = ps->createGravityAffector(core::vector3df(0,-0.019,0));

        ps->addAffector(paf);
//        ps->addAffector(paf2);
        ps->addAffector(paf3);

        paf->drop();
        paf2->drop();
        paf3->drop();

        ps->setPosition(core::vector3df(200,15,900));
        ps->setMaterialFlag(video::EMF_LIGHTING, false);
        ps->setMaterialTexture(0, m_driver->getTexture(Common::texture("blood_bw.tga")));
        ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        ps->setPosition(pos);

        scene::ISceneNodeAnimator* sna = m_smgr->createDeleteAnimator(300 / m_speedOfTime);
        ps->addAnimator(sna);
        sna->drop();
    }
}

void MonsterNode::addTriangleSelector(MonsterNode *node)
{
    m_metaTriangleSelector->addTriangleSelector( node->m_boxSelector );
}

void MonsterNode::addTriangleSelector(scene::ITriangleSelector* selector)
{
    m_metaTriangleSelector->addTriangleSelector( selector );
}

void MonsterNode::removeAllTriangleSelector()
{
    m_metaTriangleSelector->removeAllTriangleSelectors();
}

void MonsterNode::removeTriangleSelector(MonsterNode *node)
{
    m_metaTriangleSelector->removeTriangleSelector( node->m_boxSelector );
}

void MonsterNode::updateCollisionAnimator()
{
    m_node->removeAnimator(m_collisionAnimator);
    m_collisionAnimator = m_smgr->createCollisionResponseAnimator(
        m_metaTriangleSelector,
        m_node,ellipsoid(),
        irr::core::vector3df(0,0,0),ellipsoidTranslation());
    m_node->addAnimator(m_collisionAnimator);
}

bool MonsterNode::isHeadshot(core::vector3df point) const
{
    scene::IBoneSceneNode* headBone = m_node->getJointNode((u32)13);
    core::line3d<f32> ray;
    ray.start = headBone->getAbsolutePosition();
    ray.end = point;
    if(ray.getLength() < 0.4)
    {
        std::cout << "HEADSHOT!!!" << std::endl;
        return true;
    }
    return false;
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
        m_afl.setEndCallback(new DieEndCallBack(this));
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

void MonsterNode::atack()
{
    m_fms.setState(MonsterSignal::Atack);
    setMonsterState(m_fms.currentState());
    if(m_distanceToPlayer <= 10)
    {
        std::cout << Log::curTimeC() << "Atack" << std::endl;
        m_player->setHealth(m_player->health()-0.1);
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

DieEndCallBack::DieEndCallBack(MonsterNode *parent) :
    m_parent(parent)
{
}

void DieEndCallBack::OnAnimationEnd(scene::IAnimatedMeshSceneNode */*node*/)
{
    m_parent->die(this);
}


