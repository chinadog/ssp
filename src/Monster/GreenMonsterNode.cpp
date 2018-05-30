#include "GreenMonsterNode.h"
#include "Common/Common.h"
#include "GamePark.h"

GreenMonsterNode::GreenMonsterNode(GamePark *gamePark) :
    MonsterNode(gamePark)
{
    init();
    m_speedOfTime = m_gamePark->speedOfTime();
    m_metaTriangleSelector = m_smgr->createMetaTriangleSelector();
    m_boxSelector = m_smgr->createTriangleSelectorFromBoundingBox(m_node);
    m_octreeSelector = m_smgr->createOctreeTriangleSelector(m_node->getMesh(), m_node, 32);
    m_speedOfTimeChangedSignalId = m_gamePark->speedOfTimeChanged.connect_member(this,&GreenMonsterNode::setSpeedOfTime);
    m_atackDistance = 6.0;
}

int GreenMonsterNode::init()
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
//        m_node->setTriangleSelector(selector);
        selector->drop();
    }


    setMonsterState(m_currentState,true);

    return 0;
}

void GreenMonsterNode::draw()
{
    MonsterNode::draw();
}

void GreenMonsterNode::setSpeedOfTime(f32 speed)
{
    MonsterNode::setSpeedOfTime(speed);
}
