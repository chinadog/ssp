#include "Enemy.h"
#include "Common/Common.h"
#include "Common/Logger.h"

Enemy::Enemy(GamePark* gamePark) :
    AI(gamePark)
{

}

Enemy::~Enemy()
{

}

void Enemy::draw()
{
    if(m_health > 0.0)
    {
        m_aiPosition = node()->getPosition();
        m_aiRotation = node()->getRotation();
//        AI::draw();
        node()->setPosition(m_aiPosition);
        node()->setRotation(m_aiRotation);
    }
}

void Enemy::addTriangleSelector(Enemy *node)
{
    m_metaTriangleSelector->addTriangleSelector( node->m_boxSelector );
}

void Enemy::addTriangleSelector(scene::ITriangleSelector *selector)
{
    m_metaTriangleSelector->addTriangleSelector( selector );
}

void Enemy::removeAllTriangleSelector()
{
    m_metaTriangleSelector->removeAllTriangleSelectors();
}

void Enemy::removeTriangleSelector(Enemy *node)
{
    m_metaTriangleSelector->removeTriangleSelector( node->m_boxSelector );
}

void Enemy::updateCollisionAnimator()
{
    m_node->removeAnimator(m_collisionAnimator);
    m_collisionAnimator = m_smgr->createCollisionResponseAnimator(
        m_metaTriangleSelector,
        m_node,ellipsoid(),
        irr::core::vector3df(0,0,0),ellipsoidTranslation());
    m_node->addAnimator(m_collisionAnimator);
}

void Enemy::damage(f32 value, const core::vector3df &intersection)
{
    TERROR() << "enemy cur state = " << (int)m_fms.currentState();
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

void Enemy::dustEffect(const core::vector3df &pos)
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

void Enemy::bloodEffect(const core::vector3df &pos)
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

bool Enemy::isHeadshot(core::vector3df point) const
{
    scene::IBoneSceneNode* headBone = m_node->getJointNode("Bip01 Head");
    if(headBone == 0)
    {
        return false;
    }
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

void Enemy::kill()
{
    TDEBUG() << (int)m_fms.currentState();
    TDEBUG() << "send signal die";
    m_fms.setState(MonsterSignal::Die);
    TDEBUG() << (int)m_fms.currentState();
    MonsterState state = m_fms.currentState();
    setMonsterState(state);

    dustEffect( m_node->getAbsolutePosition() );


    ISound* music2;
    int rand = m_device->getRandomizer()->rand() % 2;
    if(rand == 0)
    {
        music2 = StaticSoundEngine->play3D("../../media/sounds/monster/die1.ogg",
                                              m_node->getPosition(), false, false, true);
    }
    else
    {
        music2 = StaticSoundEngine->play3D("../../media/sounds/monster/die2.ogg",
                                              m_node->getPosition(), false, false, true);
    }

    if (music2)
    {
       music2->setMinDistance(1);
       music2->setMaxDistance(150);
    }
    music2->drop();
}


