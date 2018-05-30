#ifndef PLAYER_H
#define PLAYER_H

#include <irrlicht.h>
#include <irrKlang.h>
#include <list>
#include "Common/SignalSlot.h"
#include "Weapon/Weapon.h"
#include "Weapon/AK.h"
#include "Weapon/CZ805.h"
#include "Weapon/AnimationFrameLoop.h"
#include "Player/PlayerAction.h"
#include "Weapon/WeaponAction.h"
#include "FMS.h"
#include "ShootIntersection.h"
#include "SlowMo.h"
#include "SceneNode/DropNode.h"

class GamePark;
class AI;

enum class PlayerEventType
{
    Disable = irr::EET_USER_EVENT +1
};

class Player
{
public:
    Player(GamePark* gamePark);
    ~Player();

    irr::core::vector3df ellipsoid() const;
    irr::core::vector3df ellipsoidTranslation() const;
    irr::scene::ICameraSceneNode* camera() const;
    irr::scene::IAnimatedMeshSceneNode *node() const;
    irr::scene::ISceneNode *fire() const;


    void setKeyPressed(irr::EKEY_CODE key, bool pressed);
    void setMouseButtonPressed(irr::EMOUSE_INPUT_EVENT event);
    void setPosition(irr::f32 x, irr::f32 y, irr::f32 z);
    void setRun(bool run);
    bool animationFire() const;
    void setPlayerState(const PlayerState& state);


    void addWeapon(Weapon* weapon);
    void setWeapon(int index);
    void setWeaponAction(const WeaponAction& action);
    void setWeaponAction(const PlayerState& state);
    WeaponAction weaponAction() const;

    Weapon* weapon(int index) const;
    Weapon* currentWeapon() const;
    int m_currentWeaponIndex = -1;

    inline PlayerState currentPlayerState() const {return m_currentPlayerState;}
    inline PlayerState prevPlayerState() const {return m_prevPlayerState;}
    inline FMS& fms() {return m_fms;}

    void setHealth(f32 value);
    inline f32 health() const {return m_health;}

    void gunKick();

    void playWalkSound();
    void stopWalkSound();


    void initSoundEngine();
    void updateMap();

    void draw();
    void updatePlayerInfo();

    void grab(DropNode* node);

    ShootIntersection m_shootIntersection;
    inline ShootIntersection shootIntersection() const {return m_shootIntersection;}
    ShootIntersection calcShootIntersection();
    void setShootIntersection(const ShootIntersection& section);

    Signal<int> say;
    Signal<> shipPointPassed;
    Signal<> die;
    Signal<irr::scene::ISceneNode*> targetHit;

    void setPlayerSpeed(f32 speed);
    void setSpeedOfTime(f32 speed);
    void showSlowMoShoot(AI* aiNode);
    inline SlowMo* slowmo() {return m_slowmo;}

    std::list<irr::scene::IAnimatedMeshSceneNode*> m_nodeList;

    void ladder();
    void ladderOut();

    inline bool keyW() const {return m_keyW;}
    inline bool keyA() const {return m_keyA;}
    inline bool keyS() const {return m_keyS;}
    inline bool keyD() const {return m_keyD;}
    inline bool keyShift() const {return m_keyShift;}

    f32 m_walkSpeed = 0.01;
    f32 m_runSpeed = 0.15;
    irr::scene::IAnimatedMeshSceneNode* m_bulletNode;
    scene::ISceneNodeAnimator* m_dieAnimator = nullptr;
    ISound* m_painSound = nullptr;
private:
    irr::scene::ICameraSceneNode* m_camera;
    GamePark* m_gamePark;
    irr::IrrlichtDevice* m_device;
    irr::scene::ISceneNode *m_fire;
    irr::scene::ISceneNode *pricel;
    irr::scene::ISceneNode *m_playerInfoNode;
    irr::scene::ISceneNode *m_playerOnMap;
    irr::scene::ISceneNode *m_borderOnMap;
    irr::scene::ISceneNode *m_info;
    irr::scene::IAnimatedMeshSceneNode* m_node;

    AI* m_slowMoTargetNode;
    FMS m_fms;
    PlayerState m_currentPlayerState = PlayerState::Stand;
    PlayerState m_prevPlayerState = PlayerState::Stand;
    AnimationFrameLoop m_afl;
    SlowMo* m_slowmo;

    irrklang::ISoundEngine* m_soundEngine = nullptr;
    irrklang::ISound* m_walkSound = nullptr;
    irrklang::ISound* m_shotSound = nullptr;
    irrklang::ISound* m_runSound = nullptr;
    irrklang::ISound* m_reloadSound = nullptr;
    irrklang::ISound* m_drawSound = nullptr;

    irrklang::ISoundSource* m_walkSoundSource = nullptr;

    WeaponAction m_weaponAction = WeaponAction::Undefined;

    irr::u32 m_lastFireTime = 0;
    bool m_animationFire;
    bool m_animationMove;

    f32 m_speedOfTime = 1.0;

    bool m_keyA = false;
    bool m_keyD = false;
    bool m_keyS = false;
    bool m_keyW = false;
    bool m_keyR = false;
    bool m_keyShift = false;
    bool m_leftButton = false;
    bool m_run = false;

    u32 m_startTimeFire = 0;
    u32 m_nextTimeToFire = 0;
    u32 m_prevTime = 0;
    f32 m_deltaTime = 0.0;
    f32 m_health = 1.0;



    std::list<Weapon*> m_weaponList;

};

#endif // PLAYER_H
