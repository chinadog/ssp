#include "RedMonsterNodeNew.h"
#include "GamePark.h"
#include "Common/Common.h"
#include <cmath>
#include "Common/Logger.h"

RedMonsterNodeNew::RedMonsterNodeNew(GamePark* gamePark) :
    AnimatedMeshSceneNode(0,0,0,0),
    m_gamePark(gamePark)
{
    // Ai
    m_smgr = m_gamePark->smgr();
    m_driver = m_gamePark->driver();
    m_device = m_gamePark->device();
    m_player = m_gamePark->player();
    // end ai

    scene::IAnimatedMesh* mesh = m_smgr->getMesh("../../media/models/monster_red.b3d");
    setMesh(mesh);
    setParent(m_smgr->getRootSceneNode());
    setSceneManager(m_smgr);
    setID(0);
    setScale(core::vector3df(0.1,0.1,0.1));
    setMaterialTexture(0, m_driver->getTexture("../../media/textures/monster/red.tga"));
    setMaterialFlag(video::EMF_LIGHTING, true);
    setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
    setName("MonsterNode");

    // monster
    irr::scene::ITriangleSelector* selector = m_smgr->createOctreeTriangleSelector(this->getMesh(),this,128);
    this->setTriangleSelector(selector);
    selector->drop();
    setMonsterState(m_currentState,true);
    // end monster

    //red
    scene::IBoneSceneNode* headBone = this->getJointNode("Bip01 Head");
    m_ps = m_smgr->addParticleSystemSceneNode(false, headBone);
    m_fireEmitter = m_ps->createBoxEmitter(
        core::aabbox3d<f32>(-70/60.0,0,-70/60.0,70/60.0,10/60.0,70/60.0), // emitter size
        core::vector3df(0.0,-0.0015f,0.0),   // initial direction
        38,50,                             // emit rate
        video::SColor(255,255,0,0),       // darkest color
        video::SColor(255,255,255,0),       // brightest color
        800,2000,0,                         // min and max age, angle
        core::dimension2df(0.2,0.2),         // min size
        core::dimension2df(0.4,0.4));        // max size
    m_fireEmitter->setMinParticlesPerSecond(0);
    m_fireEmitter->setMaxParticlesPerSecond(0);
    m_fireEmitter->setMinStartColor(video::SColor(255,255,0,0));
    m_fireEmitter->setMaxStartColor(video::SColor(255,225,194,20));
    m_ps->setEmitter(m_fireEmitter); // this grabs the emitter

    scene::IParticleAffector* paf = m_ps->createFadeOutParticleAffector(video::SColor(0,0,0,0), 700);
    scene::IParticleAffector* paf2 = m_ps->createScaleParticleAffector(core::dimension2df(5.5,4.5));

    m_ps->addAffector(paf); // same goes for the affector
    m_ps->addAffector(paf2); // same goes for the affector
    paf->drop();
    paf2->drop();

    m_ps->setPosition(core::vector3df(-10,3,0));
    m_ps->setScale(core::vector3df(1,1,1));
    m_ps->setMaterialFlag(video::EMF_LIGHTING, false);
    m_ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    m_ps->setMaterialTexture(0, m_driver->getTexture(Common::texture("fire.png")));
    m_ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

    m_speedOfTime = m_gamePark->speedOfTime();
    m_metaTriangleSelector = m_smgr->createMetaTriangleSelector();
    m_boxSelector = m_smgr->createTriangleSelectorFromBoundingBox(this);
    m_octreeSelector = m_smgr->createOctreeTriangleSelector(this->getMesh(), this, 32);
    m_speedOfTimeChangedSignalId = m_gamePark->speedOfTimeChanged.connect_member(this,&RedMonsterNodeNew::setSpeedOfTime);
    m_atackDistance = 10.0;
//    m_walkSpeed = m_gamePark->m_config.monster().walkSpeed();
    m_walkSpeed = 9;
    m_atackSpeed = m_gamePark->m_config.monster().atackSpeed();
    //end red


}

RedMonsterNodeNew::~RedMonsterNodeNew()
{
    m_collisionAnimator->drop();
    m_boxSelector->drop();
    m_octreeSelector->drop();
    m_metaTriangleSelector->drop();
    m_gamePark->speedOfTimeChanged.disconnect(m_speedOfTimeChangedSignalId);
    remove();
}

// AI --------------------------------------------------------------------------------------

void RedMonsterNodeNew::setTerrain(scene::ITerrainSceneNode *terrain)
{
    m_terrain = terrain;
    createGravitation();
}


void RedMonsterNodeNew::gotoPlayer(f32 timeInSeconds)
{
    if(/*this == nullptr ||*/ m_movable == false || m_player->health() <= 0.0 /*|| m_health <= 0.0*/)
    {
        return;
    }

    moveNode( m_player->camera()->getAbsolutePosition(), timeInSeconds );
}

void RedMonsterNodeNew::createGravitation()
{
    // создаем аниматор столкновений с селектором и прикрепляем его к игроку
    m_gravityAnim = m_smgr->createCollisionResponseAnimator(
        m_terrain->getTriangleSelector(), this, ellipsoid(),
        core::vector3df(0,-0,0),
        ellipsoidTranslation());
    this->addAnimator(m_gravityAnim);
//    m_gravityAnim->drop();

//    Collision::setCollision( this,m_player,m_smgr);
}

void RedMonsterNodeNew::moveNode(const core::vector3df &pos, f32 timeInSeconds)
{
    core::vector3df targetPos = pos - getPosition();
    targetPos.Y = getPosition().Y;

    if(targetPos.getLength() > m_atackDistance)  // if distanecToPlayer > atackDistan
    {
        targetPos.normalize() *= timeInSeconds*m_speed;

        setPosition(getPosition() + targetPos);
        if(m_intersects == true) {m_intersects = false; walk();}
    }
    else
    {
        if(m_intersects == false ) {m_intersects = true; atack();}
    }

    f32 rot = atan2(targetPos.X, targetPos.Z)*180/M_PI;
    if(m_isRotated)
    {
        setRotation(core::vector3df(0,rot,0));
    }
}

void RedMonsterNodeNew::calcPosAndRot()
{
//    m_targetPos =
}

// end ai-------------------------------------------------------------------------------------


// monster-------------------------------------------------------------------------------------

void RedMonsterNodeNew::draw()
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
            core::vector3df pos = this->getPosition();
            if(m_drawFinishedLevel > pos.Y-2.0)
            {
                m_gravityAnim->setGravity(core::vector3df(0,0,0));
                pos.Y += 25*m_deltaTime*m_speedOfTime;
                this->setPosition(pos);
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

    // red
    if(m_fireEmitter != 0)
    {
        core::vector3df direction = m_player->camera()->getPosition() - this->getPosition();
        direction.normalize();
        direction /= 130;

        m_fireEmitter->setDirection(core::vector3df(direction.X,-0.0015,direction.Z));

    }
    // end red
}


core::vector3df RedMonsterNodeNew::ellipsoid() const
{
    return core::vector3df(2,2,2);
}

core::vector3df RedMonsterNodeNew::ellipsoidTranslation() const
{
    return core::vector3df(0,-2,0);
}

void RedMonsterNodeNew::kill()
{
    m_fms.setState(MonsterSignal::Die);
    setMonsterState(m_fms.currentState());

//    this->setLoopMode(false);
//    this->setFrameLoop(570, 648);
//    this->setAnimationSpeed(25);
//    this->setAnimationEndCallback(new DieEndCallBackNew(this));
//    m_movable = false;
    dustEffect( this->getAbsolutePosition() );


    ISound* music2;
    int rand = m_device->getRandomizer()->rand() % 2;
    if(rand == 0)
    {
        music2 = StaticSoundEngine->play3D("../../media/sounds/monster/die1.ogg",
                                              this->getPosition(), false, false, true);
    }
    else
    {
        music2 = StaticSoundEngine->play3D("../../media/sounds/monster/die2.ogg",
                                              this->getPosition(), false, false, true);
    }

    if (music2)
    {
       music2->setMinDistance(1);
       music2->setMaxDistance(150);
    }
    music2->drop();
}

void RedMonsterNodeNew::damage(f32 value, const core::vector3df& intersection)
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

void RedMonsterNodeNew::die(DieEndCallBackNew* callback)
{
    m_life = false;
    this->setAnimationEndCallback(0);
    delete callback;
}

void RedMonsterNodeNew::addCallback(scene::IAnimationEndCallBack *callback)
{
    m_callbackList.push_back( callback );
}


void RedMonsterNodeNew::dustEffect(const core::vector3df& pos)
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

    em->setMinStartColor(video::SColor(0,125,125,125));
    em->setMaxStartColor(video::SColor(0,255,255,255));

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

void RedMonsterNodeNew::bloodEffect(const core::vector3df &pos)
{
    scene::IParticleSystemSceneNode* ps =
        m_smgr->addParticleSystemSceneNode(false);

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

void RedMonsterNodeNew::addTriangleSelector(RedMonsterNodeNew *node)
{
    m_metaTriangleSelector->addTriangleSelector( node->m_boxSelector );
}

void RedMonsterNodeNew::addTriangleSelector(scene::ITriangleSelector* selector)
{
    m_metaTriangleSelector->addTriangleSelector( selector );
}

void RedMonsterNodeNew::removeAllTriangleSelector()
{
    m_metaTriangleSelector->removeAllTriangleSelectors();
}

void RedMonsterNodeNew::removeTriangleSelector(RedMonsterNodeNew *node)
{
    m_metaTriangleSelector->removeTriangleSelector( node->m_boxSelector );
}

void RedMonsterNodeNew::updateCollisionAnimator()
{
    this->removeAnimator(m_collisionAnimator);
    m_collisionAnimator = m_smgr->createCollisionResponseAnimator(
        m_metaTriangleSelector,
        this,ellipsoid(),
        irr::core::vector3df(0,0,0),ellipsoidTranslation());
    this->addAnimator(m_collisionAnimator);
}

bool RedMonsterNodeNew::isHeadshot(core::vector3df point)
{
    scene::IBoneSceneNode* headBone = getJointNode("Bip01 Head");
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

void RedMonsterNodeNew::setSpeedOfTime(f32 speed)
{
    m_speedOfTime = speed;
    this->setAnimationSpeed(m_afl.speed()*m_speedOfTime);
}

void RedMonsterNodeNew::setMonsterStateMonster(const MonsterState &state, bool force)
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
        m_afl.setEndCallback(new DieEndCallBackNew(this));
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

    this->setFrameLoop(m_afl.start(), m_afl.end());
    this->setAnimationSpeed(m_afl.speed()*m_speedOfTime);
    this->setLoopMode(m_afl.loop());
    this->setAnimationEndCallback(m_afl.endCallback());
}

void RedMonsterNodeNew::setMonsterState(const MonsterState &state, bool force)
{
    setMonsterStateMonster(state, force);
    switch (state) {
    case MonsterState::Walk :
        m_fireEmitter->setMaxParticlesPerSecond(0);
        break;
    case MonsterState::Die :
        m_fireEmitter->setMaxParticlesPerSecond(0);
        break;
    default :
        break;
    }
}

void RedMonsterNodeNew::woundFinished()
{
    m_fms.setState(MonsterSignal::Stop);
    setMonsterState(m_fms.currentState());
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

DieEndCallBackNew::DieEndCallBackNew(RedMonsterNodeNew *parent) :
    m_parent(parent)
{
}

void DieEndCallBackNew::OnAnimationEnd(scene::IAnimatedMeshSceneNode */*node*/)
{
    m_parent->die(this);
}

// end monster---------------------------------------------------------------------------------


// red-------------------------------------------------------------------------------------
void RedMonsterNodeNew::fireEffect()
{
    m_fireEmitter->setMinParticlesPerSecond(80);
    m_fireEmitter->setMaxParticlesPerSecond(90);
}

// end red--------------------------------------------------------------------------------------

void RedMonsterNodeNew::OnAnimate(u32 timeMs)
{
    AnimatedMeshSceneNode::OnAnimate(timeMs);
//    fpsCount++;
//    if(fpsCount == 2)
//    {
//        calcPosAndRot();
//        fpsCount = 0;
//    }
    draw();
}

