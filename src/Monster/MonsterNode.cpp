#include "MonsterNode.h"
#include "Common/Common.h"

MonsterNode::MonsterNode(irr::IrrlichtDevice* device, Player* player) :
    AI(device, player)
{
    init();
    m_metaTriangleSelector = m_smgr->createMetaTriangleSelector();
    m_boxSelector = m_smgr->createTriangleSelectorFromBoundingBox(m_node);
    m_octreeSelector = m_smgr->createOctreeTriangleSelector(m_node->getMesh(), m_node, 32);
}

MonsterNode::~MonsterNode()
{
    Log::log("Destructr monster");
    m_collisionAnimator->drop();
    m_boxSelector->drop();
    m_octreeSelector->drop();
    m_metaTriangleSelector->drop();
    remove();
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
        m_node->setMaterialTexture(0, texture("plitka.tga"));
        m_node->setScale(core::vector3df(7.0f/60.0,7.0f/60.0,7.0f/60.0));
        m_node->setMaterialFlag(video::EMF_LIGHTING, true);
        m_node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        m_node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
        m_node->setName("MonsterNode");
        irr::scene::ITriangleSelector* selector = m_smgr->createOctreeTriangleSelector(m_node->getMesh(),m_node,128);
        m_node->setTriangleSelector(selector);
        selector->drop();
    }
    m_node->setFrameLoop(0,240);
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
    m_node->setLoopMode(false);
    m_node->setFrameLoop(570, 648);
    m_node->setAnimationEndCallback(new DieEndCallBack(this));
    m_movable = false;
    dustEffect( m_node->getAbsolutePosition() );
}

void MonsterNode::damage(f32 value, const core::vector3df& intersection)
{
    if(health() > value)
    {
        setHealth(health() - value);
        bloodEffect( intersection );
    }
    else if(m_movable==true)
    {
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
        core::vector3df(0.0f,0.001f,0.0f),   // initial direction
        8,10,                             // emit rate
        video::SColor(0,255,255,255),       // darkest color
        video::SColor(0,255,255,255),       // brightest color
        800,2000,0,                         // min and max age, angle
        core::dimension2df(100.f/60.0,100.f/60.0),         // min size
        core::dimension2df(200.f/60.0,200.f/60.0));        // max size

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setPosition(pos);
    ps->setScale(core::vector3df(2,2,2));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, texture("dust.png"));
    ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    scene::ISceneNodeAnimator* sna = m_smgr->createDeleteAnimator(1800);
    ps->addAnimator(sna);
    sna->drop();
}

void MonsterNode::bloodEffect(const core::vector3df &pos)
{
    scene::IParticleSystemSceneNode* ps =
    m_smgr->addParticleSystemSceneNode(false);

    scene::IParticleEmitter* em = ps->createBoxEmitter(
        core::aabbox3d<f32>(-0.5,0,-0.5,0.5,1,0.5), // emitter size
        core::vector3df(0.0f,0.005f,0.0f),   // initial direction
        15,40,                             // emit rate
        video::SColor(0,0,0,0),       // darkest color
        video::SColor(0,81,19,19),       // brightest color
        800,1000,180,                         // min and max age, angle
        core::dimension2df(0.1f,0.1f),         // min size
        core::dimension2df(0.4f,0.4f));        // max size

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    scene::IParticleAffector* paf = ps->createGravityAffector(core::vector3df(0,-0.01,0),200);
    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setPosition(pos);
    //core::vector3df(8800*2,200*2,29800*2)
    ps->setScale(core::vector3df(2,2,2));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    //ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    //ps->setMaterialTexture(0, driver->getTexture("../media/grass_dirty.jpg"));
    //ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

    scene::ISceneNodeAnimator* sna = m_smgr->createDeleteAnimator(500);
    ps->addAnimator(sna);
    sna->drop();
}

void MonsterNode::addTriangleSelector(MonsterNode *node)
{
    m_metaTriangleSelector->addTriangleSelector( node->m_boxSelector );
}

void MonsterNode::addTriangleSelector(scene::ITriangleSelector* selector)
{
    m_metaTriangleSelector->addTriangleSelector( selector );
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

DieEndCallBack::DieEndCallBack(MonsterNode *parent) :
    m_parent(parent)
{
}

void DieEndCallBack::OnAnimationEnd(scene::IAnimatedMeshSceneNode */*node*/)
{
    m_parent->die(this);
}


