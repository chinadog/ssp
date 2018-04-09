#include "Player.h"
#include <iostream>

#include <functional>
#include <cmath>
#include "GamePark.h"
#include "Common/Common.h"

#include "SoundEngine/SoundEngine.h"

using namespace irr;
using namespace irrklang;

Player::Player(GamePark* gamePark) :
    m_gamePark(gamePark),
    m_device(gamePark->device())
{
    SKeyMap keyMap[9];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;

    keyMap[8].Action = EKA_JUMP_UP;
    keyMap[8].KeyCode = KEY_SPACE;


    // camera
    m_camera = m_device->getSceneManager()->addCameraSceneNodeFPS(0, 100.0f, 0.01f, -1, keyMap, 9, false, 2.f);
    m_camera->setPosition(core::vector3df(10700*2,5085*2,27600*2)); // light
    m_camera->setTarget(core::vector3df(4897*2,343*2,17800*2));
    m_camera->setFarValue(42000.0f/30.0);
    m_camera->setNearValue(0.1);

    // ---- fire -----

    m_fire = m_device->getSceneManager()->addBillboardSceneNode(camera(), core::dimension2d<f32>(0.740, 0.740));
    m_fire->setPosition(core::vector3df(0.0740,-0.0370,2));
    m_fire->setMaterialFlag(video::EMF_LIGHTING, false);
    m_fire->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    m_fire->setMaterialTexture(0, m_device->getVideoDriver()->getTexture("../../media/textures/shootfire.tga"));
    m_fire->setVisible(true);

    // --------------

    // ---- pricel -----

    pricel = m_device->getSceneManager()->addBillboardSceneNode(camera(), core::dimension2d<f32>(0.1, 0.1));
    pricel->setPosition(core::vector3df(0,0,1));
    pricel->setMaterialFlag(video::EMF_LIGHTING, false);
    pricel->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    pricel->setMaterialTexture(0, m_device->getVideoDriver()->getTexture("../../media/textures/pricel.tga"));

    // --------------

    // ---- map -----

//    m_playerOnMap = m_device->getSceneManager()->addCubeSceneNode(0.3, camera());
//    m_playerOnMap->setPosition(core::vector3df(-1.75,-0.35,1.75));
//    m_playerOnMap->setMaterialFlag(video::EMF_LIGHTING, false);
//    m_playerOnMap->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
//    m_playerOnMap->setMaterialTexture(0, m_device->getVideoDriver()->getTexture("../../media/textures/player_on_map.png"));
//    m_playerOnMap->setScale(core::vector3df(1,1,0));
//    m_playerOnMap->setRotation(core::vector3df(0,-30,0));



    scene::IMeshSceneNode* node;
    scene::IMesh* mesh;
    mesh = m_device->getSceneManager()->getMesh("../../media/models/player_info.b3d");
    if (!mesh)
    {
        m_device->drop();
    }


    node = m_device->getSceneManager()->addMeshSceneNode(mesh,camera() );
    if (node)
    {
        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setPosition(core::vector3df(0,0,0));
        node->setRotation(core::vector3df(0,0,0));

        for(uint i=0;i<node->getMaterialCount();i++)
        {
            node->getMaterial(i).setTexture(0, m_device->getVideoDriver()->getTexture("../../media/textures/player_info.tga"));
            node->getMaterial(i).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
        }
        node->getMaterial(0).setTexture(0, m_device->getVideoDriver()->getTexture("../../media/textures/park-sceme-blue-2.tga"));
    }
    m_playerInfoNode = node;

    initSoundEngine();

}


Player::~Player()
{
//    m_walkSound->drop();
//    m_walkSound = nullptr;
//    m_shotSound->drop();
//    m_shotSound = nullptr;
  m_fire->drop();
  pricel->drop();

  if(m_walkSound != nullptr)
  {
      m_walkSound->stop();
      m_walkSound->drop();
      m_walkSound = nullptr;

  }

  while(m_weaponList.size() > 0)
  {
      auto it = m_weaponList.begin();
      Weapon* w = *it;
      m_weaponList.remove(w);
      delete w;
  }

}

irr::core::vector3df Player::ellipsoid() const
{
    return core::vector3df(2,2,2);
}

core::vector3df Player::ellipsoidTranslation() const
{
    return core::vector3df(0,0,0);
}

irr::scene::ICameraSceneNode* Player::camera() const
{
    return m_camera;
}

irr::scene::IAnimatedMeshSceneNode *Player::node() const
{
    return m_node;
}

irr::scene::ISceneNode *Player::fire() const
{
    return m_fire;
}



void Player::setMouseButtonPressed(EMOUSE_INPUT_EVENT event)
{
    switch(event)
    {
    case EMIE_LMOUSE_PRESSED_DOWN:
        m_leftButton = true;
        break;
    case EMIE_LMOUSE_LEFT_UP:
        m_leftButton = false;
        m_lastFireTime = 0;
        break;
    default:
        // We won't use the wheel
        break;
    }

    if(m_leftButton)
    {
        m_fms.setState(PlayerSignal::Shoot);
    }
    else
    {
        m_fms.setState(PlayerSignal::StopShoot);
    }
    setPlayerState(m_fms.currentState());
}

void Player::setPosition(f32 x, f32 y, f32 z)
{
    camera()->setPosition(core::vector3df(x,y,z));
}


bool Player::animationFire() const
{
    return m_animationFire;
}


void Player::setPlayerState(const PlayerState &state)
{
    if(state == m_currentPlayerState)
    {
        return;
    }
    m_prevPlayerState = m_currentPlayerState;
    m_currentPlayerState = state;

    core::list<scene::ISceneNodeAnimator*>::ConstIterator anims = camera()->getAnimators().begin();
    scene::ISceneNodeAnimatorCameraFPS *anim=(scene::ISceneNodeAnimatorCameraFPS*)*anims;
    Weapon *curWeapon = currentWeapon();

    switch (state) {
    case PlayerState::Stand :
        curWeapon->shootSound(false);
        curWeapon->reloadSound(false);
        curWeapon->drawSound(false);
        stopWalkSound();
        break;
    case PlayerState::Duck :
        currentWeapon()->shootSound(false);
        break;
    case PlayerState::Walk :
        playWalkSound();
        curWeapon->shootSound(false);
        curWeapon->reloadSound(false);
        curWeapon->drawSound(false);
        anim->setMoveSpeed(0.01);
        break;
    case PlayerState::Run :
        curWeapon->shootSound(false);
        curWeapon->reloadSound(false);
        curWeapon->drawSound(false);
        anim->setMoveSpeed(0.15);
        break;
    case PlayerState::DuckWalk :
        anim->setMoveSpeed(0.01);
        break;
    case PlayerState::StandShoot :
        curWeapon->shootSound(true);
        curWeapon->reloadSound(false);
        curWeapon->drawSound(false);
        break;
    case PlayerState::DuckShoot :
        break;
    case PlayerState::WalkShoot :
        curWeapon->shootSound(true);
        curWeapon->reloadSound(false);
        curWeapon->drawSound(false);
        playWalkSound();
        anim->setMoveSpeed(0.01);
        break;
    case PlayerState::RunShoot :
        curWeapon->shootSound(true);
        curWeapon->reloadSound(false);
        curWeapon->drawSound(false);
        anim->setMoveSpeed(0.15);
        break;
    case PlayerState::DuckWalkShoot :
        anim->setMoveSpeed(0.01);
        break;
    case PlayerState::StandReload :
        curWeapon->shootSound(false);
        curWeapon->reloadSound(true);
        curWeapon->drawSound(false);
        currentWeapon()->reloadSound(true);
        break;
    case PlayerState::DuckReload :
        break;
    case PlayerState::WalkReload :
        curWeapon->shootSound(false);
        curWeapon->reloadSound(true);
        curWeapon->drawSound(false);
        anim->setMoveSpeed(0.01);
        break;
    case PlayerState::RunReload :
        curWeapon->shootSound(false);
        curWeapon->reloadSound(true);
        curWeapon->drawSound(false);
        anim->setMoveSpeed(0.15);
        break;
    case PlayerState::DuckWalkReload :
        anim->setMoveSpeed(0.01);
        break;
    case PlayerState::StandDraw :
    case PlayerState::DuckDraw :
    case PlayerState::WalkDraw :
    case PlayerState::RunDraw :
    case PlayerState::DuckWalkDraw :
        curWeapon->shootSound(false);
        curWeapon->reloadSound(false);
        curWeapon->drawSound(true);
        break;
    default:
        break;
    }
    setWeaponAction(state);
}

void Player::addWeapon(Weapon *weapon)
{
    m_weaponList.push_back( weapon );
    m_nodeList.push_back( weapon->node() );
}

void Player::setWeapon(int index)
{
    if(index == m_currentWeaponIndex)
    {
        return;
    }

    m_currentWeaponIndex = index;
    for(uint i=0;i<m_weaponList.size();i++)
    {
        auto it = std::next(m_weaponList.begin(),i);
        Weapon* w = *it;
        scene::IAnimatedMeshSceneNode* node = w->node();
        node->setVisible(false);
    }
    auto it = std::next(m_nodeList.begin(),index);
    scene::IAnimatedMeshSceneNode* node = *it;
    node->setVisible(true);
    m_node = node;

    m_fms.setState(PlayerSignal::Draw);
    setPlayerState(m_fms.currentState());

    updatePlayerInfo();
}

void Player::setWeaponAction(const WeaponAction &action)
{
    if(action == WeaponAction::Shoot && currentWeapon()->bulletCount() == 0)
    {
        return;
    }

    m_weaponAction = action;
    AnimationFrameLoop afl = currentWeapon()->animationFrameLoop(m_weaponAction);
    m_node->setFrameLoop(afl.start(),
                         afl.end());
    m_node->setAnimationSpeed(afl.speed());
    m_node->setLoopMode(afl.loop());
    m_node->setAnimationEndCallback(afl.endCallback());
}

void Player::setWeaponAction(const PlayerState &state)
{
    if((state >= PlayerState::StandDraw && state <= PlayerState::DuckWalkDraw) &&
       (m_prevPlayerState >= PlayerState::StandDraw && m_prevPlayerState <= PlayerState::DuckWalkDraw))
    {
        return;
    }

    if((state >= PlayerState::StandReload && state <= PlayerState::DuckWalkReload) &&
       (m_prevPlayerState >= PlayerState::StandReload && m_prevPlayerState <= PlayerState::DuckWalkReload))
    {
        return;
    }

    if(state >= PlayerState::StandShoot && state <= PlayerState::DuckWalkShoot &&
       currentWeapon()->bulletCount() == 0)
    {
        return;
    }

    AnimationFrameLoop afl = currentWeapon()->animationFrameLoop(state);
    m_node->setFrameLoop(afl.start(),
                         afl.end());
    m_node->setAnimationSpeed(afl.speed());
    m_node->setLoopMode(afl.loop());
    m_node->setAnimationEndCallback(afl.endCallback());
}

WeaponAction Player::weaponAction() const
{
    return m_weaponAction;
}

Weapon* Player::weapon(int index) const
{
    auto it = std::next(m_weaponList.begin(), index);
    return *it;
}

Weapon* Player::currentWeapon() const
{
    return weapon(m_currentWeaponIndex);
}

void Player::gunKick()
{
    s32 rX = m_device->getRandomizer()->rand() % 20 - 10;
    s32 rY = m_device->getRandomizer()->rand() % 20 - 10;
    m_device->getCursorControl()->setPosition(m_device->getCursorControl()->getPosition().X+rX,
                                          m_device->getCursorControl()->getPosition().Y+rY);
}

void Player::playWalkSound()
{
    if(m_walkSound != nullptr)
    {
        return;
    }
    m_walkSound = StaticSoundEngine->play2D(m_walkSoundSource, true ,false, true);
    m_walkSound->setVolume(0.2);
    m_walkSound->drop();
}

void Player::stopWalkSound()
{
    if(m_walkSound == nullptr)
    {
        return;
    }
    m_walkSound->stop();
    m_walkSound = nullptr;
}

void Player::initSoundEngine()
{
    m_walkSoundSource = StaticSoundEngine->getSoundSource("../../media/sounds/walk.ogg");
}

void Player::updateMap()
{
    f32 scaleCoef = 0.25;
    f32 realTerrainSize = 1077.0;
    f32 realMapSize = 1577.0;
    // Считаем разницу между размером карты (картинки) и террейном, нормализуя значение [0..1]
    // Делим на два, чтобы получить разницу только с одного края.
    // f32 diffSizeNormal = (realMapSize - realTerrainSize) / (2.0*realTerrainSize);
    // Считаем позицию на карте. Так как у террейна начало отсчета это правый верхний угол (<v),
    // а у карты левый верхний угол (v>), то необходим будет отразить X координату (1.0-..).
    // Сначала нормализуем значение относительно размера террейна, затем добавляем смещение. После чего
    // делим на размер карты и умножаем на размер террейна, чтобы получить значение относительно террейна,
    // а не всей карты.
    // posOnMap.X = ((1.0-( m_camera->getPosition().X / realTerrainSize ))+diffSizeNormal) / realMapSize*realTerrainSize;
    // posOnMap.Z = ((    ( m_camera->getPosition().Z / realTerrainSize ))+diffSizeNormal) / realMapSize*realTerrainSize;
    // После упрощения
    core::vector3df posOnMap;
    posOnMap.X = 0.5+(realTerrainSize-2.0*m_camera->getPosition().X)/(2.0*realMapSize);
    posOnMap.Z = 0.5+(2.0*m_camera->getPosition().Z-realTerrainSize)/(2.0*realMapSize);

    m_playerInfoNode->getMaterial(0).getTextureMatrix(0).buildTextureTransform(
         (m_camera->getRotation().Y - 180)*M_PI/180.0,
         core::vector2df(-0.5f, -0.5f*scaleCoef),
         core::vector2df(posOnMap.X, posOnMap.Z),
         core::vector2df(scaleCoef, scaleCoef)
                );

    if(std::fabs(m_camera->getPosition().X - 843) < 10 &&
       std::fabs(m_camera->getPosition().Y - 16) < 10 &&
       std::fabs(m_camera->getPosition().Z - 181) < 10 )
    {
        shipPointPassed.Emit();
    }
}

void Player::draw()
{
    u32 now = m_device->getTimer()->getTime();

    setShootIntersection(ShootIntersection());
    if(fire()->isVisible() == true && m_startTimeFire + currentWeapon()->shootDelta() < now)
    {
        fire()->setVisible(false);
    }
    if(m_currentPlayerState >= PlayerState::StandShoot && m_currentPlayerState <= PlayerState::DuckWalkShoot)
    {
        if(now >= m_nextTimeToFire && currentWeapon()->bulletCount()>0)
        {
            m_startTimeFire = now;
            m_nextTimeToFire = now + currentWeapon()->shootBreakDelta();
            fire()->setVisible(true);
            currentWeapon()->removeBullets(1);
            updatePlayerInfo();
            gunKick();
            setShootIntersection(calcShootIntersection());
            std::cout << Log::curTimeC() << " Shoot" << std::endl;
            std::cout << currentWeapon()->bulletCount() << "/" << currentWeapon()->bulletCountInClip() << std::endl;
        }
        if(currentWeapon()->bulletCount()==0)
        {
            m_fms.setState(PlayerSignal::StopShoot);
            setPlayerState(m_fms.currentState());
        }
    }

    updateMap();
}

void Player::updatePlayerInfo()
{
    int bulletCount[2];
    bulletCount[0] = currentWeapon()->bulletCount()/10;
    bulletCount[1] = currentWeapon()->bulletCount()%10;

    int bulletCountInClip[3];
    bulletCountInClip[0] = currentWeapon()->bulletCountInClip()/100;
    bulletCountInClip[1] = currentWeapon()->bulletCountInClip()%100/10;
    bulletCountInClip[2] = currentWeapon()->bulletCountInClip()%10;

    for(int i=0;i<2;i++)
    {
        m_playerInfoNode->getMaterial(2+i).getTextureMatrix(0).buildTextureTransform(
             0,
             core::vector2df(0,0),
             core::vector2df(bulletCount[i]/10.0, 0),
             core::vector2df(1, 1));
    }

    for(int i=0;i<3;i++)
    {
        m_playerInfoNode->getMaterial(4+i).getTextureMatrix(0).buildTextureTransform(
             0,
             core::vector2df(0,0),
             core::vector2df(bulletCountInClip[i]/10.0, 0),
             core::vector2df(1, 1));
    }

    m_playerInfoNode->getMaterial(7).getTextureMatrix(0).buildTextureTransform(
         0,
         core::vector2df(0,0),
         core::vector2df(0.5+currentWeapon()->bulletCount()/60.0, 0),
         core::vector2df(1, 1));

    m_playerInfoNode->getMaterial(8).getTextureMatrix(0).buildTextureTransform(
         0,
         core::vector2df(0,0),
         core::vector2df(0.5+100/60.0, 0),
         core::vector2df(1, 1));

    m_playerInfoNode->getMaterial(9).getTextureMatrix(0).buildTextureTransform(
         0,
         core::vector2df(0,0),
         core::vector2df(m_currentWeaponIndex/4.5, 0),
                core::vector2df(1, 1));
}

ShootIntersection Player::calcShootIntersection()
{
    scene::ISceneCollisionManager*  collMan;
    collMan = m_device->getSceneManager()->getSceneCollisionManager();

    core::line3d<f32> ray;
    ray.start = camera()->getPosition();
    ray.end = ray.start + (camera()->getTarget() - ray.start).normalize() * 100000.0f;

    // переменная под хранение точки пересещения
    core::vector3df intersection;
    // переменная под хранение треугольника с которым пересекся луч
    core::triangle3df hitTriangle;

    scene::ISceneNode * selectedSceneNode =
    collMan->getSceneNodeAndCollisionPointFromRay(
    ray,
    intersection, // точка столкновения
    hitTriangle, // полигон(треугольник) в котором
    // точка столкновения
    IDFlag_IsPickable, // определять столкновения только для
    //нод с идентификатором IDFlag_IsPickable
    0); // проверять относительно всей
    // сцены (оставляем значение по умолчанию)

    return ShootIntersection(intersection, selectedSceneNode, ray);
}

void Player::setShootIntersection(const ShootIntersection &section)
{
    m_shootIntersection = section;
}

void Player::setKeyPressed(EKEY_CODE key, bool pressed)
{
    switch (key) {
    case KEY_KEY_A :
        m_keyA = pressed;
        break;
    case KEY_KEY_D :
        m_keyD = pressed;
        break;
    case KEY_KEY_S :
        m_keyS = pressed;
        break;
    case KEY_KEY_W :
        m_keyW = pressed;
        break;
    case KEY_KEY_R :
        m_keyR = pressed;
        break;
    case 160 : // Shift
        m_keyShift = pressed;
        break;
    default:
        break;
    }

    if(m_keyA || m_keyD || m_keyS || m_keyW)
    {
        if(m_keyShift)
        {
            m_fms.setState(PlayerSignal::Run);
        }
        else
        {
            m_fms.setState(PlayerSignal::Go);
        }
    }
    else
    {
        m_fms.setState(PlayerSignal::Stop);
    }
    if(m_keyR)
    {
        m_fms.setState(PlayerSignal::Reload);
    }

    setPlayerState(m_fms.currentState());
    //std::cout << "state = " << (int)m_fms.currentState() << std::endl;

}

