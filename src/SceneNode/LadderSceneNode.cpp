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

        core::vector3d<f32>* edges = new core::vector3d<f32>[8]; //Bounding BOX edges
        m_node->getTransformedBoundingBox().getEdges( edges );

        int dif = m_gamePark->player()->ellipsoid().X*2;

        col = cmgr->getCollisionPoint(playerRay,m_boxSelector, point, triangle, node);
        if(col == true &&
           ((m_gamePark->player()->keyW() == true && startPos.Y <= (edges[0].Y+dif)) ||
            (startPos.Y >= (edges[0].Y+dif))))
        {
            if(m_intersects == false)
            {
                m_intersects = true;
                std::cout << Log::curTimeC() << " IN" << std::endl;
                enter.Emit();
            }
        }

        if((startPos.Y <= (edges[0].Y+dif) || startPos.Y >= (edges[1].Y+dif)) && m_climb == true)
        {
            std::cout << Log::curTimeC() << " OUT" << std::endl;
            leave.Emit();
            m_climb = false;
            m_intersects = false;
        }
        else if(m_climb == false && m_intersects == true &&
                (startPos.Y >= (edges[0].Y+dif) && startPos.Y <= (edges[1].Y+dif)))
        {
            m_climb = true;
        }
        delete edges;
    }

    video::SMaterial m;
    m.Lighting=false;
    m_gamePark->driver()->setMaterial(m);
//    m_gamePark->driver()->setTransform(video::ETS_WORLD, core::matrix4());
//    m_gamePark->driver()->draw3DLine(ray.start,ray.end,video::SColor(255,255,255,255));
//    m_gamePark->driver()->draw3DLine(playerRay.start,playerRay.end,video::SColor(255,255,0,0));


    core::aabbox3df box = m_node->getBoundingBox();
    m_gamePark->driver()->setTransform(video::ETS_WORLD, m_node->getAbsoluteTransformation());
    m_gamePark->driver()->draw3DBox(box,video::SColor(255,0,255,0));
}

void LadderSceneNode::setPosition(const core::vector3df &value)
{
    m_node->setPosition(value);
}

void LadderSceneNode::setRotation(const core::vector3df &value)
{
    m_node->setRotation(value);
}

void LadderSceneNode::setScale(const core::vector3df &value)
{
    m_node->setScale(value);
}

void LadderSceneNode::initNode()
{
    m_node = m_gamePark->smgr()->addEmptySceneNode();

    m_boxSelector = m_gamePark->smgr()->createTriangleSelectorFromBoundingBox(m_node);
    cmgr = m_gamePark->smgr()->getSceneCollisionManager();
}
