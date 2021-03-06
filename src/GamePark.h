#ifndef GAMEPARK_H
#define GAMEPARK_H

#include <irrlicht.h>
#include <stdio.h>
#include <iostream>
#include "Config/Config.h"
#include "Player/Player.h"
#include "EventReceiver/EventReceiver.h"
#include <list>
#include "UI/SceneMode.h"
#include "UI/InOutFader.h"
#include "UI/MessageBox.h"
#include "SceneNode/LadderSceneNode.h"
#include "SceneNode/MotionPictureCredits.h"
#include "Monster/MonsterNode.h"
#include "SceneNode/NuclearBoomSceneNode.h"
#include "Monster/RespawnPoint.h"
#include "SceneNode/GameOverCredits.h"
#include "SceneNode/DropNode.h"
#include "Screen/Screen.h"
#include "Screen/ScreenEndGame.h"
#include "Screen/ScreenGameOver.h"
#include "Screen/ScreenLoading.h"
#include "Screen/ScreenSurvival.h"
#include "Screen/ScreenMainMenu.h"


using namespace irr;

enum
{
    // I use this ISceneNode ID to indicate a scene node that is
    // not pickable by getSceneNodeAndCollisionPointFromRay()
    ID_IsNotPickable = 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be picked by ray selection.
    IDFlag_IsPickable = 1 << 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be highlighted.  In this example, the
    // homonids can be highlighted, but the level mesh can't.
    IDFlag_IsHighlightable = 1 << 1
};



//========================================================================================

class GamePark
{
public:
    GamePark();
    ~GamePark();

    void exit();
    int run();

    void switchTerrainMaterial(video::E_MATERIAL_FLAG material);
    void switchTerrainDetailMap();
    void switchSkybox(bool blue);
    void updateEnvironment(const core::stringw& str);

    template<typename T>
    void log(T string)
         {std::cout << string << std::flush << std::endl;}

    inline video::IVideoDriver *driver() const {return m_device->getVideoDriver();}
    inline scene::ISceneManager *smgr() const {return m_device->getSceneManager();}
    inline gui::IGUIEnvironment *env() const {return m_device->getGUIEnvironment();}
    inline IrrlichtDevice *device() const {return m_device;}

    inline SceneMode sceneMode() const {return m_sceneMode;}
    void setSceneMode(const SceneMode& mode);



    scene::ISceneNode* m_mainMenuNode;
    scene::ISceneNode* m_menuNewGameNode;


    scene::ICameraSceneNode*  camera = nullptr;
    scene::ICameraSceneNode*  camera2 = nullptr;
    scene::ICameraSceneNode*  m_mainMenuCamera = nullptr;

    scene::ITerrainSceneNode* m_terrain = nullptr;

    io::IReadFile *heightmap = nullptr;
    Config m_config;

    bool shadows = true;
    bool freefly = false;
    bool gameMode = false;

    bool walking = false;
    bool isFire = false;
    bool usl_exit = false;


    bool key_w = false;
    bool key_a = false;
    bool key_s = false;
    bool key_d = false;

std::list<Enemy*> m_aiNode;
void updateMonsterCollision();


    Screen* m_screen = nullptr;
    ScreenLoading*  m_screenLoading  = nullptr;
    ScreenGameOver* m_screenGameOver = nullptr;
    ScreenEndGame*  m_screenEndGame  = nullptr;
    ScreenSurvival* m_screenSurvival = nullptr;
    ScreenMainMenu* m_screenMainMenu = nullptr;


    void setSpeedOfTime(f32 speed);
    f32 speedOfTime() const;
    Signal<f32> speedOfTimeChanged;

    Player *player() const;

    scene::IAnimatedMeshSceneNode *movableNode() const;

    scene::ISceneNodeAnimatorCollisionResponse *gravityAnim() const;
    MyEventReceiver* receiver = nullptr;
private:
    irrklang::ISoundEngine* m_soundEngine;
    IrrlichtDevice* m_device = nullptr;
    Player* m_player = nullptr;
    scene::ISceneNode* skyboxBlue = nullptr;
    scene::ISceneNode* skyboxRed = nullptr;

    gui::IGUIStaticText* m_controlText = nullptr;
    gui::IGUIStaticText* m_fpsText = nullptr;
    const int m_forestSize = 5;
    scene::IMeshSceneNode* m_forest[5];
    scene::ITriangleSelector* m_forestSelector[5];
    scene::IAnimatedMeshSceneNode* m_monsterNode = nullptr;
    scene::IMeshSceneNode *m_roadNode = nullptr;
    scene::ISceneNode* m_cubeNode;
    scene::ISceneNodeAnimatorCollisionResponse* resp;
    scene::IMeshSceneNode* m_whiteBoxNode;
    scene::ITriangleSelector* m_whiteBoxSelector;
    LadderSceneNode* m_ladder;
    scene::ISceneNodeAnimatorCollisionResponse* m_gravityAnim = nullptr;
    MotionPictureCredits* m_credits = nullptr;
    GameOverCredits* m_gameOverCredits = nullptr;

    gui::IGUIImage* image;
    gui::IGUIStaticText* statusText;

    irr::scene::ISceneNodeAnimator* m_testCol = nullptr;
    irr::scene::IMetaTriangleSelector* m_metaTriangleSelector;

    scene::IAnimatedMeshSceneNode* m_movableNode = nullptr;
    unsigned short m_checkLodCounter = 0;
    unsigned short m_checkFpsCounter= 0;

    f32 m_speedOfTime = 1.0;

    SceneMode m_sceneMode = SceneMode::MainMenu;

    InOutFader* addInOutFader(const core::rect<s32>* rectangle=0, gui::IGUIElement* parent=0, s32 id=-1);

    void menuFlyCamera(const SceneMode& mode);
    void boom(u32 startTime);
    void setBoomFrame(u32 frame);
    u32 m_boom = 0;
    scene::IBillboardSceneNode* m_boomNode;
    NuclearBoomSceneNode* m_nuclearBoom = nullptr;
    std::list<NuclearBoomSceneNode*> m_testNucl;


    std::list<RespawnPoint*> m_respPoints;

    Signal<int> loadProgressbarChanged;

    InOutFader* m_fader = nullptr;




    void finishGame();
    void gameOver();
    bool m_finish = false;

    void forestLOD(core::vector3df pos);
    int initDriver();
    void initCamera();
    void initEnvironment();
    void initPlayer();
    int initWater();
    int initFountain();
    int initChildSquare();
    int initAttractionSquare();
    int initChurch();
    int initHotel();
    int initWorld();
    int initTerrain();
    int initSkybox();
    int initLight();
    int initReceiver();
    int initTestObj();
    int initForest();
    int initShrub();
    int initScam();
    int initBench();
    int initGrass();
    int initRoads();
    int initCurb();
    int initGarbage();
    int initSounds();
    int initAI();
    int initRespawnPoints();
    int initAircraft();
    int initLake();
    int initDecal();
    int initWhiteBox();
    int initLadder();
    int initMenu();
    int initFader();
    int initScreens();
    void updateScreensValues();
    void setCollision(scene::IAnimatedMeshSceneNode* node,
                      Player* player);
    void setCollision(scene::IMeshSceneNode* node,
                      Player* player);
    video::ITexture* texture(const io::path &textureName);
    void moveObj(const core::vector3df& pos);

    friend class ScreenSurvival;
    friend class ScreenLoading;
    friend class ScreenEndGame;
    friend class ScreenGameOver;
    friend class ScreenMainMenu;
};

#endif // GAMEPARK_H
