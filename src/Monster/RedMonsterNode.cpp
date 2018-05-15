#include "RedMonsterNode.h"
#include "Common/Common.h"
#include "GamePark.h"
#include <cmath>

RedMonsterNode::RedMonsterNode(GamePark *gamePark) :
    MonsterNode(gamePark)
{
    init();
    m_speedOfTime = m_gamePark->speedOfTime();
    m_metaTriangleSelector = m_smgr->createMetaTriangleSelector();
    m_boxSelector = m_smgr->createTriangleSelectorFromBoundingBox(m_node);
    m_octreeSelector = m_smgr->createOctreeTriangleSelector(m_node->getMesh(), m_node, 32);
    m_speedOfTimeChangedSignalId = m_gamePark->speedOfTimeChanged.connect_member(this,&RedMonsterNode::setSpeedOfTime);
    m_atackDistance = 10.0;
    m_walkSpeed = m_gamePark->m_config.monster().walkSpeed();
    m_atackSpeed = m_gamePark->m_config.monster().atackSpeed();
}

RedMonsterNode::~RedMonsterNode()
{

}

int RedMonsterNode::init()
{
    scene::IAnimatedMesh* mesh;
    mesh = m_smgr->getMesh(Common::modelsPath()+"monster_red.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    m_node = m_smgr->addAnimatedMeshSceneNode( mesh );
    if (m_node)
    {
        f32 rand = m_device->getRandomizer()->rand() % 20 / 100;
        m_node->setMaterialTexture(0, m_driver->getTexture(Common::texture("monster/red.tga")));
//        m_node->setMaterialTexture(0, texture("plitka.tga"));
        m_node->setScale(core::vector3df(0.1+rand,0.1+rand,0.1+rand));
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

    scene::IBoneSceneNode* headBone = m_node->getJointNode("Bip01 Head");
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


    return 0;
}

void RedMonsterNode::draw()
{
    MonsterNode::draw();

    if(m_fireEmitter != 0)
    {
        core::vector3df direction = m_player->camera()->getPosition() - m_node->getPosition();
        direction.normalize();
        direction /= 130;

        m_fireEmitter->setDirection(core::vector3df(direction.X,-0.0015,direction.Z));

    }
}

void RedMonsterNode::setSpeedOfTime(f32 speed)
{
    MonsterNode::setSpeedOfTime(speed);
}

void RedMonsterNode::atack()
{
    m_fms.setState(MonsterSignal::Atack);
    setMonsterState(m_fms.currentState());
    if(m_distanceToPlayer <= m_atackDistance)
    {
        std::cout << Log::curTimeC() << "Atack" << std::endl;
        m_player->setHealth(m_player->health()-0.0);
        fireEffect();
    }
}

void RedMonsterNode::fireEffect()
{
    m_fireEmitter->setMinParticlesPerSecond(150);
    m_fireEmitter->setMaxParticlesPerSecond(180);
}

void RedMonsterNode::setMonsterState(const MonsterState &state, bool force)
{
    MonsterNode::setMonsterState(state, force);

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

