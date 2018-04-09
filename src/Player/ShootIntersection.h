#ifndef SHOOTINTERSECTION_H
#define SHOOTINTERSECTION_H

#include <irrlicht.h>
#include <iostream>

using namespace irr;

class ShootIntersection
{
public:
    ShootIntersection(core::vector3df intersection = core::vector3df(0,0,0),
                      scene::ISceneNode* node = nullptr,
                      core::line3d<f32> ray = core::line3d<f32>());


    core::vector3df m_intersection;
    scene::ISceneNode* m_node = nullptr;
    core::line3d<f32> m_ray;
    bool isValid();
};

#endif // SHOOTINTERSECTION_H
