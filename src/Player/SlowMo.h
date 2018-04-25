#ifndef SLOWMO_H
#define SLOWMO_H

#include <irrlicht.h>
#include <list>

using namespace irr;

class GamePark;

class SlowMo
{
public:
    SlowMo(GamePark* gamePark);
    void start(const core::vector3df& pos1, const core::vector3df& pos2);
    void draw();

    inline void setSpeed(f32 value) {m_speed = value;}
    inline void setReverseSpeed(f32 value) {m_reverseSpeed = value;}
    inline void setCameraTarget(const core::vector3df& value) {m_cameraTarget = value;}
    inline f32 speed() const {return m_speed;}
    inline f32 reverseSpeed() const {return m_reverseSpeed;}
    inline core::vector3df cameraTarget() const {return m_cameraTarget;}
    inline bool isActive() const {return m_isActive;}
    inline core::vector3df startCameraPos() const {return m_startCameraPos;}
private:
    GamePark* m_gamePark;
    scene::ICameraSceneNode* m_camera = nullptr;
    core::vector3df m_pos1;
    core::vector3df m_pos2;
    f32 m_speed = 0.2;
    f32 m_reverseSpeed = 1.0;
    bool m_toTarget = true;
    bool m_isActive = false;
    core::vector3df m_cameraTarget;
    std::list <SEvent> m_slowmoEvents;

    core::vector3df m_startCameraPos;
    core::vector3df m_startCameraTarget;

    void disablePlayer();
};

#endif // SLOWMO_H
