#include "LadderSceneNode.h"
#include "GamePark.h"
#include <iostream>
#include "Common/Common.h"
#include "Common/Collision.h"

LadderSceneNode::LadderSceneNode(GamePark* gamePark) : m_gamePark(gamePark)
{
    initNode();
    enter.connect_member(m_gamePark->player(), &Player::ladder);
    leave.connect_member(m_gamePark->player(), &Player::ladderOut);
}

scene::ISceneNode *LadderSceneNode::node()
{
    return m_node;
}

void LadderSceneNode::draw()
{
    core::vector3df startPos = m_gamePark->player()->camera()->getAbsolutePosition();
    core::vector3df endPos = m_node->getAbsolutePosition();
    startPos.Y += 0;
    endPos.Y = startPos.Y;

    core::line3d<f32> ray;
    core::line3d<f32> playerRay;

    ray.start = startPos;
    ray.end = endPos;

    playerRay.start = startPos;
    playerRay.start.Y += 0.1;
    playerRay.end = playerRay.start + (endPos - playerRay.start).normalize()*2 ;

    if(ray.getLength() <= 35)
    {
        core::vector3df point;
        core::triangle3df triangle;
        scene::ISceneNode* node;
        bool col = false;

        col = cmgr->getCollisionPoint(playerRay,m_boxSelector, point, triangle, node);

        if(col == true)
        {
            if(m_intersects == false)
            {
                m_intersects = true;
//                std::cout << Log::curTimeC() << " IN" << std::endl;
                enter.Emit();
            }
        }

        core::vector3d<f32>* edges = new core::vector3d<f32>[8]; //Bounding BOX edges
        m_node->getTransformedBoundingBox().getEdges( edges );

        if((startPos.Y <= (edges[0].Y+3) || startPos.Y >= (edges[1].Y+2)) && m_climb == true)
        {
//            std::cout << Log::curTimeC() << " OUT" << std::endl;
            leave.Emit();
            m_climb = false;
            m_intersects = false;
        }
        else if(m_climb == false)
        {
            m_climb = true;
        }
//        std::cout << m_gamePark->player()->camera()->getPosition().Y << std::endl;
    }

    video::SMaterial m;
    m.Lighting=false;
    m_gamePark->driver()->setMaterial(m);
    m_gamePark->driver()->setTransform(video::ETS_WORLD, core::matrix4());
    m_gamePark->driver()->draw3DLine(ray.start,ray.end,video::SColor(255,255,255,255));
    m_gamePark->driver()->draw3DLine(playerRay.start,playerRay.end,video::SColor(255,255,0,0));

//    core::vector3d<f32>* edges = new core::vector3d<f32>[8]; //Bounding BOX edges
//    m_node->getTransformedBoundingBox().getEdges( edges );
//    for(int i=0;i<8;i++)
//    {
//        for(int j=0;j<8;j++)
//        {
//            m_gamePark->driver()->draw3DLine(edges[i],edges[j],video::SColor(255,255,0,0));
//        }
//    }


//    core::vector3d<f32>* edges2 = new core::vector3d<f32>[8]; //Bounding BOX edges
//    m_emptyNode->getTransformedBoundingBox().getEdges( edges2 );
//    for(int i=0;i<8;i++)
//    {
//        for(int j=0;j<8;j++)
//        {
//            m_gamePark->driver()->draw3DLine(edges2[i],edges2[j],video::SColor(255,255,255,0));
//        }
//    }


    core::aabbox3df box = m_node->getBoundingBox();
    m_gamePark->driver()->setTransform(video::ETS_WORLD, m_node->getAbsoluteTransformation());
    m_gamePark->driver()->draw3DBox(box,video::SColor(255,0,255,0));


}

void LadderSceneNode::initNode()
{
    m_node = m_gamePark->smgr()->addEmptySceneNode();
    m_node->setScale(core::vector3df(2,13,0.5));
    m_node->setPosition(core::vector3df(861, 23.65, 216));
    m_node->setRotation(core::vector3df(0,41,0));
    m_node->setDebugDataVisible(irr::scene::EDS_BBOX);

//    m_emptyNode = m_gamePark->smgr()->addCubeSceneNode(10);
//    m_emptyNode->setPosition( m_node->getPosition() );
//    m_emptyNode->setRotation(core::vector3df(0,90,0));
//    m_emptyNode->setDebugDataVisible(irr::scene::EDS_BBOX);
//    m_node->setScale(core::vector3df(0.1,1,0.01));
//    Collision::setCollision((scene::IMeshSceneNode*)m_node, m_gamePark->player(), m_gamePark->smgr());

    m_boxSelector = m_gamePark->smgr()->createTriangleSelectorFromBoundingBox(m_node);
    cmgr = m_gamePark->smgr()->getSceneCollisionManager();
}
