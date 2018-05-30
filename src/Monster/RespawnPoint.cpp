#include "RespawnPoint.h"
#include "GamePark.h"
#include "RedMonsterNode.h"
#include "GreenMonsterNode.h"
#include "RedMonsterNodeNew.h"
#include "RedMonsterNodeNewTest.h"
#include "Common/Logger.h"

RespawnPoint::RespawnPoint(GamePark* gamePark, const core::vector3df& pos) :
    m_gamePark(gamePark),
    m_pos(pos)
{
    init();
    m_speedOfTimeChangedSignalId = m_gamePark->speedOfTimeChanged.connect_member(this,&RespawnPoint::setSpeedOfTime);
}

RespawnPoint::~RespawnPoint()
{
    m_gamePark->speedOfTimeChanged.disconnect(m_speedOfTimeChangedSignalId);
    m_node->drop();
}

void RespawnPoint::createMonster()
{
    s32 rand = m_gamePark->device()->getRandomizer()->rand() % 2;

    MonsterNode* node = 0;

    if(rand == 0)
    {
        node = new RedMonsterNode(m_gamePark);
    }
    else
    {
        node = new RedMonsterNode(m_gamePark);
    }

//    MonsterNode* node = new MonsterNode(m_gamePark);
    core::vector3df monsterPos = m_pos;
    monsterPos.Y -= 100;
    node->setPosition( monsterPos );
    node->setDrawFinishedLevel(m_node->getPosition().Y + 0.8);
    node->setTerrain(m_gamePark->m_terrain);
    m_gamePark->m_aiNode.push_back(node);
    m_gamePark->updateMonsterCollision();
    node->layOut.connect_member(this,&RespawnPoint::draw);
}

void RespawnPoint::createMonsterNew()
{
//    s32 rand = m_gamePark->device()->getRandomizer()->rand() % 2;

//    RedMonsterNodeNew* node = 0;

//    if(rand == 0)
//    {
//        node = new RedMonsterNodeNew(m_gamePark);
//    }
//    else
//    {
//        node = new RedMonsterNodeNew(m_gamePark);
//    }

//    core::vector3df monsterPos = m_pos;
//    monsterPos.Y -= 20;
//    node->setPosition( monsterPos );
//    node->setDrawFinishedLevel(m_node->getPosition().Y + 0.8);
//    node->setTerrain(m_gamePark->m_terrain);
//    m_gamePark->m_aiNodeNew.push_back(node);
//    m_gamePark->updateMonsterCollisionNew();
//    node->layOut.connect_member(this,&RespawnPoint::draw);
}

void RespawnPoint::init()
{
//    scene::ISceneNode* node = m_gamePark->smgr()->addCubeSceneNode();
//    node->setPosition(m_pos);

    scene::IAnimatedMesh* mesh = m_gamePark->smgr()->getMesh("../../media/models/earth_destroy.x");
    if (!mesh)
    {
        m_gamePark->device()->drop();
    }
    m_node = m_gamePark->smgr()->addAnimatedMeshSceneNode(mesh);

    m_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    m_node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    m_node->setPosition(m_pos);
    m_node->setFrameLoop(0,0);

    for(u32 i=0;i<m_node->getMaterialCount();i++)
    {
        m_node->getMaterial(i).setTexture(0, m_gamePark->device()->getVideoDriver()->getTexture("../../media/textures/grass_dirty.tga"));
    }
}

void RespawnPoint::draw()
{
    m_node->setFrameLoop(0,120);
    m_node->setAnimationSpeed(180*m_speedOfTime);
    m_node->setLoopMode(false);

    scene::IParticleSystemSceneNode* ps =
        m_gamePark->smgr()->addParticleSystemSceneNode(false);

    scene::IParticleEmitter* em = ps->createBoxEmitter(
        core::aabbox3d<f32>(-70/60.0,0,-70/60.0,70/60.0,10/60.0,70/60.0), // emitter size
        core::vector3df(0.0f,0.0005f,0.0f),   // initial direction
        8,10,                             // emit rate
        video::SColor(0,255,255,255),       // darkest color
        video::SColor(0,255,255,255),       // brightest color
        800,2000,0,                         // min and max age, angle
        core::dimension2df(4,4),         // min size
        core::dimension2df(6,6));        // max size

    em->setMinStartColor(video::SColor(0,125,125,125));
    em->setMaxStartColor(video::SColor(0,255,255,255));

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

//    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    core::vector3df pos = m_pos;
    pos.Y += 0.8;
    ps->setPosition(pos);
    ps->setScale(core::vector3df(4,4,4));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0,  m_gamePark->device()->getVideoDriver()->getTexture("../../media/textures/dust2.png"));
    ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    scene::ISceneNodeAnimator* sna = m_gamePark->smgr()->createDeleteAnimator(1800);
    ps->addAnimator(sna);
    sna->drop();
}

void RespawnPoint::setSpeedOfTime(f32 speed)
{
    m_speedOfTime = speed;
    m_node->setAnimationSpeed(180*m_speedOfTime);
}
