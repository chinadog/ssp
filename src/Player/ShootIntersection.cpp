#include "ShootIntersection.h"

ShootIntersection::ShootIntersection(core::vector3df intersection,
                                     scene::ISceneNode *node,
                                     core::line3d<f32> ray) :
    m_intersection(intersection), m_node(node), m_ray(ray)
{

}


bool ShootIntersection::isValid()
{
    return !!m_node;
}

