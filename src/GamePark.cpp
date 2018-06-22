#include "GamePark.h"
#include "EventReceiver/EventReceiver.h"
#include "Config/Inireader.h"
#include "Common/Collision.h"
#include "Common/Common.h"
#include <irrKlang.h>
#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include "SoundEngine/SoundEngine.h"
#include "Weapon/Knife.h"
#include "Common/Logger.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#endif



using namespace irrklang;

GamePark::GamePark() :
    m_checkLodCounter(0)
{
    m_config.load("../../config/config.ini");

    initDriver();
}

GamePark::~GamePark()
{
    delete receiver;
    delete m_player;
    delete m_ladder;
    if(m_credits){delete m_credits;}
    if(m_gameOverCredits){delete m_gameOverCredits;}

    delete m_nuclearBoom;

    while(m_respPoints.size()>0)
    {
        auto it = m_respPoints.begin();
        RespawnPoint* w = *it;
        m_respPoints.remove(w);
        delete w;
    }

//    statusText->drop();
    m_mainMenuCamera->drop();
    m_fpsText->drop();
    m_terrain->drop();
    m_whiteBoxNode->drop();

    delete m_screenEndGame;
    delete m_screenGameOver;
    delete m_screenSurvival;
}

void GamePark::exit()
{
    usl_exit = true;
}

int GamePark::initWorld()
{
    initFader();
    loadProgressbarChanged.Emit(50);
    initTerrain();
    loadProgressbarChanged.Emit(60);
    initWhiteBox();
    initLadder();
    initAI();
    initTestObj();
    initRespawnPoints();
//    initForest();
    TLogger::get().init();
    TDEBUG() << "test" << 2;
    TWARNING() << "test2";
    loadProgressbarChanged.Emit(70);

    initMenu();

    initWater();

    initSkybox();
    initLight();


    return 0;
}


int GamePark::initDriver()
{
    // создаем device c настройками взятыми из файла конифгурации,
    // при необходимости можно добавить больше параметров, проверьте
    // irr::SIrrlichtCreationParameters
    irr::SIrrlichtCreationParameters params = m_config.params();
    params.DriverType = irr::video::EDT_OPENGL;
    m_device = createDeviceEx(params);

    if (m_device == 0)
    {
        // не удалось создать device
        return 1;
    }

    // custom
    smgr()->setShadowColor(irr::video::SColor(150,0,0,0));		//shadow
    smgr()->setAmbientLight(video::SColorf(0.45,0.45,0.45,0.7)); // shadow color
    driver()->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
    if(m_config.fog() == true)
    {
        driver()->setFog(video::SColor(128,40,40,40), video::EFT_FOG_LINEAR, 250/60.0, 18000/60.0, .000f, true, false);
    }
    m_device->getCursorControl()->setVisible(false);

    m_metaTriangleSelector = smgr()->createMetaTriangleSelector();


    return 0;
}

void GamePark::initCamera()
{
    core::array<core::vector3df> points;
    points.push_back(core::vector3df(-931.473755f, 138.300003f, 987.279114f)); // -49873

    m_mainMenuCamera = smgr()->addCameraSceneNode(0, points[0], core::vector3df(0 ,400,0));
}

void GamePark::initEnvironment()
{
    int y = m_config.params().WindowSize.Height;
    // устанавливаем шрифт

    env()->getSkin()->setFont(env()->getFont("../../media/fonts/arial.xml"));
    env()->getSkin()->setColor(gui::EGUI_DEFAULT_COLOR::EGDC_BUTTON_TEXT, video::SColor(255,255,255,255));
    env()->getSkin()->setColor(gui::EGUI_DEFAULT_COLOR::EGDC_3D_FACE, video::SColor(190,20,20,20));

    m_fpsText = env()->addStaticText(L"",
                                     core::rect<s32>(10,y-55,250,y-20),
                                     false, true, 0, -1, true);
    m_fpsText->setVisible(false);
}

void GamePark::initPlayer()
{
    m_player = new Player(this);
    smgr()->setActiveCamera(m_player->camera());

    m_player->setPosition(174,85,994);

    loadProgressbarChanged.Emit(20);
    m_player->addWeapon(new AK(this));
    loadProgressbarChanged.Emit(30);
    m_player->addWeapon(new CZ805(this));
    m_player->addWeapon(new Knife(this));
    loadProgressbarChanged.Emit(40);
    m_player->setWeapon(0);
    m_player->weapon(1)->addBullets(250);
    m_player->weapon(0)->addBullets(1000);

    m_player->die.connect_member(this, &GamePark::gameOver);
}

int GamePark::initWater()
{
    // создаение залива
//    const f32 width = 37120.0f/60.0;
//    const f32 height = 75120.0f/60.0;
//    RealisticWaterSceneNode* water = new RealisticWaterSceneNode(smgr(), width, height);
//    smgr()->getRootSceneNode()->addChild(water);
//    water->setPosition(core::vector3df(52160/60.0,35*2/60.0,34840/60.0));
//    water->setWaterColor(video::SColorf(0,0.5,0.5));
//    water->setWaveHeight(0.3f);
//    water->setWindForce(100.0f/30.0);


//    // создание озер
//    scene::IAnimatedMesh* mesh = smgr()->addHillPlaneMesh( "myHill",
//        core::dimension2d<f32>(200/60.0,460/60.0),
//        core::dimension2d<u32>(60,60), 0, 0,
//        core::dimension2d<f32>(0,0),
//        core::dimension2d<f32>(60,60));
//    scene::ISceneNode* waternode;
//    waternode = smgr()->addWaterSurfaceSceneNode(mesh->getMesh(0), 6.0f/60.0, 300.0f, 100.0f/60.0); //100 / 60
//    waternode->setPosition(core::vector3df(35860/60.0,5,45900/60.0));
//    waternode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
//    waternode->setMaterialTexture(0, texture("dirty2.jpg"));
//    waternode->setMaterialTexture(1, texture("water_dirty.jpg"));

//    waternode->getMaterial(0).getTextureMatrix(0).setTextureScale(0.5, 1);
//    waternode->getMaterial(0).TextureLayer->TextureWrapU = video::ETC_REPEAT;
//    //waternode->getMaterial(1).getTextureMatrix(1).setTextureScale(length/2, 1);
//    //waternode->getMaterial(1).TextureLayer->TextureWrapU = video::ETC_REPEAT;

//    waternode->setMaterialType(video::EMT_REFLECTION_2_LAYER);
//    waternode->setMaterialFlag(video::EMF_FOG_ENABLE, m_config.fog());

//    mesh->drop();
//    //---------------------------------------------

//    mesh = smgr()->addHillPlaneMesh( "myHill2",
//        core::dimension2d<f32>(400/60.0,300/60.0),
//        core::dimension2d<u32>(43,40), 0, 0,
//        core::dimension2d<f32>(0,0),
//        core::dimension2d<f32>(60,60));
//    waternode = smgr()->addWaterSurfaceSceneNode(mesh->getMesh(0), 6.0f/60.0, 300.0f, 100.0f/60.0);
//    waternode->setPosition(core::vector3df(51060/60.0,5,29800/60.0));
//    waternode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
//    waternode->setMaterialTexture(0, texture("dirty2.jpg"));
//    waternode->setMaterialTexture(1, texture("water_dirty.jpg"));

//    waternode->getMaterial(0).getTextureMatrix(0).setTextureScale(1, 1);
//    waternode->getMaterial(0).TextureLayer->TextureWrapU = video::ETC_REPEAT;

//    waternode->setMaterialType(video::EMT_REFLECTION_2_LAYER);
//    waternode->setMaterialFlag(video::EMF_FOG_ENABLE, m_config.fog());
//    m_movableNode = (scene::IAnimatedMeshSceneNode*)waternode;
//    mesh->drop();

//    //----------------------------------------------------
//    // создвание залива (без шейдеров)

    scene::IAnimatedMesh* mesh;
    scene::ISceneNode* waternode;

    mesh = smgr()->addHillPlaneMesh( "myHill3",
        core::dimension2d<f32>(500/60.0,800/60.0),
        core::dimension2d<u32>(90,80), 0, 0,
        core::dimension2d<f32>(0,0),
        core::dimension2d<f32>(60,60));
    waternode = smgr()->addWaterSurfaceSceneNode(mesh->getMesh(0), 6.0f/60.0, 300.0f, 100.0f/60.0);
    waternode->setPosition(core::vector3df(48600/60.0,5,29800/60.0));
    waternode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    waternode->setMaterialTexture(0, texture("dirty2.jpg"));
    waternode->setMaterialTexture(1, texture("water_dirty.jpg"));

    waternode->getMaterial(0).getTextureMatrix(0).setTextureScale(1, 1);
    waternode->getMaterial(0).TextureLayer->TextureWrapU = video::ETC_REPEAT;

    waternode->setMaterialType(video::EMT_REFLECTION_2_LAYER);
    waternode->setMaterialFlag(video::EMF_FOG_ENABLE, m_config.fog());
//    m_movableNode = (scene::IAnimatedMeshSceneNode*)waternode;
    mesh->drop();


    return 0;
}

int GamePark::initTerrain()
{
    // добавление ландшафта
    m_terrain = smgr()->addTerrainSceneNode(
                    "../../media/textures/terrain_heightmap.tga",
                    0,					// parent node
                    -1,					// node id
                    core::vector3df(0.f, 0.f, 0.f),		// position
                    core::vector3df(0.f, 0.f, 0.f),		// rotation
                    core::vector3df(1.0f, 0.08, 1.0f),	// scale 1, 0.05, 1
                    video::SColor ( 255, 255, 255, 255 ),	// vertexColor
                    6,					// maxLOD 6
                    scene::ETPS_17,				// patchSize
                    3					// smoothFactor 3
                    );
    m_terrain->setMaterialType(video::EMT_DETAIL_MAP);
    m_terrain->setMaterialFlag(video::EMF_LIGHTING, false);
//    m_terrain->setMaterialTexture(0, texture("park2.tga"));
    m_terrain->setMaterialTexture(0, texture("park-textura-5.tga"));
    m_terrain->setMaterialTexture(1, texture("grass_dirty.tga"));
    m_terrain->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
    m_terrain->scaleTexture(1.0f, 390.0f);
    if(m_config.fog() == true)
    {
        m_terrain->setMaterialFlag(video::EMF_FOG_ENABLE, true);
    }

    if(m_config.freeFly() == false)
    {
        // создаем trangle selector для ландшафта
        scene::ITriangleSelector* selector
            = smgr()->createTerrainTriangleSelector(m_terrain, 0);
        m_terrain->setTriangleSelector(selector);

        // создаем аниматор столкновений с селектором и прикрепляем его к игроку
        m_gravityAnim = smgr()->createCollisionResponseAnimator(
            selector, m_player->camera(), m_player->ellipsoid(),
            core::vector3df(0,-1.0,0),
            m_player->ellipsoidTranslation());
        m_player->camera()->addAnimator(m_gravityAnim);
//        anim->drop();



//        // создаем аниматор столкновений с селектором и прикрепляем его к игроку
//        scene::ISceneNodeAnimator* anim2 = smgr()->createCollisionResponseAnimator(
//            selector, m_aiNode, core::vector3df(250,400,250),
//            core::vector3df(0,-8,0),
//            core::vector3df(0, -100,0));
//        m_aiNode->addAnimator(anim2);
//        anim2->drop();

//        selector->drop();
    }

    /* Если вам нужен доступ к данным ландшафта, вы также можете сделать это
     *  напрямую с помощью следующего фрагмента кода.
    */
    scene::CDynamicMeshBuffer* buffer = new scene::CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
    m_terrain->getMeshBufferForLOD(*buffer, 0);
    //video::S3DVertex2TCoords* data = (video::S3DVertex2TCoords*)buffer->getVertexBuffer().getData();
    // Работайте с данными или получите IndexBuffer с аналогичным вызовом.
    buffer->drop(); // При завершении снова сбрасываем буфер

    return 0;
}

scene::ISceneNodeAnimatorCollisionResponse *GamePark::gravityAnim() const
{
    return m_gravityAnim;
}

int GamePark::initSkybox()
{
    /*
     * Чтобы пользователь мог переключаться между обычным и каркасным режимами,
     * мы создаем экземпляр приемника событий сверху и позволяем Irrlicht знать
     * об этом. Кроме того, мы добавляем skybox, который мы уже использовали
     * во многих примерах Irrlicht, и skydome, который показан взаимоисключающим
     *  с skybox, нажав «S».
    */

    // создание skybox и skydome
    driver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

    skyboxBlue = smgr()->addSkyBoxSceneNode(texture("skybox/blue/up.png"),
                                            texture("skybox/blue/down.png"),
                                            texture("skybox/blue/left.png"),
                                            texture("skybox/blue/right.png"),
                                            texture("skybox/blue/front.png"),
                                            texture("skybox/blue/back.png"));

    skyboxRed = smgr()->addSkyBoxSceneNode(texture("skybox/storm/up.png"),
                                           texture("skybox/storm/down.png"),
                                           texture("skybox/storm/left.png"),
                                           texture("skybox/storm/right.png"),
                                           texture("skybox/storm/front.png"),
                                           texture("skybox/storm/back.png"));
    skyboxBlue->setVisible(true);
    skyboxRed->setVisible(false);

    driver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    return 0;
}

int GamePark::initLight()
{
    // Добавляем свет, чтобы невыбранные ноды не были полностью черными.
    scene::ILightSceneNode * light = smgr()->addLightSceneNode(0, core::vector3df(10700*2/60.0,2585*2/60.0,32600*2/60.0),
        video::SColorf(0.8f,0.8f,0.8f,1.0f), 20000.0f/60.0);
    light->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    light->setID(ID_IsNotPickable);

//    if(m_config.normals() == true)
//    {
//        scene::ILightSceneNode * light2 = smgr()->addLightSceneNode(0, core::vector3df(370,550,260),
//            video::SColorf(0.8f,0.8f,0.8f,1.0f), 20000.0f/60.0);
//        light2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
//        light2->setID(ID_IsNotPickable);

//        smgr()->addCubeSceneNode(100)->setPosition(light2->getPosition());


//        scene::ILightSceneNode * light3 = smgr()->addLightSceneNode(0, core::vector3df(150,30,760),
//            video::SColorf(1.0f,0.0f,0.0f,1.0f), 20000.0f);
//        light3->setID(ID_IsNotPickable);



//        smgr()->addCubeSceneNode(5)->setPosition(light4->getPosition());
//    }

    return 0;
}

int GamePark::initReceiver()
{
    // создаем event receiver
    receiver = new MyEventReceiver(this);
    m_device->setEventReceiver(receiver);
    return 0;
}

int GamePark::initTestObj()
{
//    scene::IAnimatedMeshSceneNode* node;
//    scene::IAnimatedMesh* mesh;
//    mesh = m_device->getSceneManager()->getMesh("../../media/models/knife.b3d");
//    if (!mesh)
//    {
//        m_device->drop();
//    }
//    node = m_device->getSceneManager()->addAnimatedMeshSceneNode(mesh,m_player->camera() );
//    if (node)
//    {
//        node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
//        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
//        node->setPosition(core::vector3df(0.1, -0.083, -0.1));
//        node->setRotation(core::vector3df(0,180,0));
//        node->setScale(core::vector3df(0.1,0.1,0.1));
//        node->setMaterialTexture(0,m_device->getVideoDriver()->getTexture("../../media/textures/plitka.tga"));
//    }
//    m_node = node;
    return 1;
}

int GamePark::initForest()
{
    core::vector3df pos[m_forestSize];
    pos[0] = core::vector3df(20000/60.0,12,59360/60.0);
    pos[1] = core::vector3df(43630/60.0,12,49760/60.0);
    pos[2] = core::vector3df(15060/60.0,12,33010/60.0);
    pos[3] = core::vector3df(46400/60.0,12,26800/60.0);
    pos[4] = core::vector3df(46400/60.0,12,26800/60.0);
    core::vector3df scale = core::vector3df(70.0f/60.0,70.0f/60.0,70.0f/60.0);

    for(int i=0;i<m_forestSize;i++)
    {
        core::stringc postfix = core::stringc(i+1)+".b3d";
        core::stringc postfixLow = core::stringc(i+1)+"_low.b3d";
        scene::IMesh* mesh = smgr()->getMesh(Common::modelsPath()+"forest_"+postfix);
        if (!mesh)
        {
            m_device->drop();
            return 1;
        }
        scene::IMeshSceneNode* node = smgr()->addMeshSceneNode( mesh );
        if (node)
        {
            node->setScale(scale);
            node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
            node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            node->setPosition(pos[i]);
            node->setRotation(core::vector3df(0,180,0));
            node->setMaterialTexture( 0, texture("tree.jpg") );
            node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
            if(m_config.fog() == true)
            {
                node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
            }
        }
//        setCollision(node, m_player);
        Collision::setMetaCollision(node, smgr(), m_metaTriangleSelector);
        m_forest[i] = node;
        m_forestSelector[i] = smgr()->createOctreeTriangleSelector(m_forest[i]->getMesh(), m_forest[i], 32);

        mesh = smgr()->getMesh(Common::modelsPath()+"forest_"+postfixLow);
        if (!mesh)
        {
            m_device->drop();
            return 1;
        }
        node = smgr()->addMeshSceneNode( mesh );
        if (node)
        {
            node->setScale(scale);
            node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
            node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
            node->setPosition(pos[i]);
            node->setRotation(core::vector3df(0,180,0));
            node->setMaterialTexture( 0, texture("tree_spherical_1_2.png") );

        }
        mesh->drop();
    }
    return 0;
}

int GamePark::initShrub()
{
    scene::IMesh* mesh;
    scene::IMeshSceneNode* node;
    mesh = smgr()->getMesh(Common::modelsPath()+"shrub_1.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    node = smgr()->addMeshSceneNode( mesh );
    if (node)
    {
        node->setScale(core::vector3df(3.2f/60.0,3.2f/60.0,3.2f/60.0));
        node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        node->setPosition(core::vector3df(28540/60.0,0,60340/60.0));
        node->setRotation(core::vector3df(0,180,0));
        //node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        node->setMaterialTexture( 0, texture("textura_kust_2.png") );
        //node->getMaterial(0).NormalizeNormals = true;
        node->getMaterial(0).TextureLayer[0].BilinearFilter = true;
        node->setMaterialFlag(video::EMF_FOG_ENABLE, m_config.fog());
        //node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);

    }
    return 0;
}

int GamePark::initScam()
{
    scene::IMesh* mesh = smgr()->getMesh(Common::modelsPath()+"scam_1_2.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    scene::IMeshSceneNode* node = smgr()->addMeshSceneNode( mesh );
    if (node)
    {
        node->setScale(core::vector3df(147.0f/60.0,147.0f/60.0,147.0f/60.0));
        node->setRotation(core::vector3df(0,180,0));
        node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setPosition(core::vector3df(13060/60.0,140/60.0,58320/60.0));
        node->setMaterialTexture( 0, texture("scam.jpg") );
        node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
        node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
        node->setMaterialFlag(video::EMF_FOG_ENABLE, m_config.fog());

    }
    setCollision(node,m_player);
    mesh->drop();
    return 0;
}

int GamePark::initBench()
{
    scene::IMesh* mesh = smgr()->getMesh(Common::modelsPath()+"bench.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    scene::IMeshSceneNode* node = smgr()->addMeshSceneNode( mesh );
    if (node)
    {
        node->setScale(core::vector3df(51.0f,51.0f,51.0f));
        node->setRotation(core::vector3df(0,180,0));
        node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setPosition(core::vector3df(14070,389,56270));
        node->setMaterialTexture( 0, texture("bench.jpg") );
        node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
        node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
        node->setMaterialFlag(video::EMF_FOG_ENABLE, m_config.fog());

    }
    setCollision(node,m_player);
    mesh->drop();
    return 0;
}

int GamePark::initRoads()
{
    scene::IMesh* mesh = smgr()->getMesh(Common::modelsPath()+"road.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    scene::IMeshSceneNode* node = smgr()->addMeshSceneNode( mesh );
    if (node)
    {
//        node->setScale(core::vector3df(123.4f/60.0,123.4f/60.0,123.4f/60.0));
        node->setRotation(core::vector3df(0,180,0));
//        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
//        node->setPosition(core::vector3df(9730*2/60.0,150*2/60.0,26555*2/60.0));
        node->setPosition(core::vector3df(133,12.5,974));
        node->setMaterialTexture( 0, texture("plitka.jpg") );
        node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
        node->getMaterial(0).getTextureMatrix(0).setTextureScale(400,400);
        //node->setMaterialType(video::EMT_DETAIL_MAP);
        node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
        node->setMaterialFlag(video::EMF_FOG_ENABLE, m_config.fog());

    }
    m_roadNode = node;
    setCollision(node,m_player);
    mesh->drop();
    return 0;
}

int GamePark::initCurb()
{
    scene::IMesh* mesh = smgr()->getMesh(Common::modelsPath()+"curb.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    scene::IMeshSceneNode* node = smgr()->addMeshSceneNode( mesh );
    if (node)
    {
        node->setScale(core::vector3df(123.4f/60.0,123.4f/60.0,123.4f/60.0));
        node->setRotation(core::vector3df(0,180,0));
//        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setPosition(core::vector3df(9730*2/60.0,150*2/60.0,26555*2/60.0));
        //node->addShadowVolumeSceneNode(0,-1,false,5000.0f);
        node->setMaterialTexture( 0, texture("curb2.jpg") );
//        node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
        node->getMaterial(0).getTextureMatrix(0).setTextureScale(80,80);
        node->getMaterial(1).getTextureMatrix(0).setTextureScale(40,40);
        node->getMaterial(2).getTextureMatrix(0).setTextureScale(40,40);
        //node->setMaterialType(video::EMT_DETAIL_MAP);
        node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
        node->setMaterialFlag(video::EMF_FOG_ENABLE, m_config.fog());

        for(u32 i=0;i<node->getMaterialCount();i++)
        {
            node->getMaterial(i).TextureLayer[0].AnisotropicFilter = 16;
        }

//        node->setMaterialTexture(1, texture("grass_dirty.jpg"));
//        node->getMaterial(0).getTextureMatrix(1).setTextureScale(100,100);
//        node->setMaterialType(video::EMT_DETAIL_MAP);

//        node->getMaterial(0).getTextureMatrix(0).setTextureScale(200, 200);

    }
//    setCollision(node,m_player);
    mesh->drop();
    return 0;
}

int GamePark::initGarbage()
{
    core::vector3df pos(14790.0f, 459.0f, 46960.0f);
    scene::IMesh* mesh = smgr()->getMesh(Common::modelsPath()+"garbage_ground.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    scene::IMeshSceneNode* node = smgr()->addMeshSceneNode( mesh );
    if (node)
    {
        node->setScale(core::vector3df(86.0f,86.0f,86.0f));
        node->setRotation(core::vector3df(0,180,0));
        node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setPosition(pos);
        //node->addShadowVolumeSceneNode(0,-1,false,5000.0f);
        node->setMaterialTexture( 0, texture("garbage_ground.png") );
//        node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
//        node->getMaterial(0).getTextureMatrix(0).setTextureScale(80,80);
//        node->getMaterial(1).getTextureMatrix(0).setTextureScale(40,40);
//        node->getMaterial(2).getTextureMatrix(0).setTextureScale(40,40);
        //node->setMaterialType(video::EMT_DETAIL_MAP);
        node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
        node->setMaterialFlag(video::EMF_FOG_ENABLE, m_config.fog());


//        node->setMaterialTexture(1, texture("grass_dirty.jpg"));
//        node->getMaterial(0).getTextureMatrix(1).setTextureScale(100,100);
//        node->setMaterialType(video::EMT_DETAIL_MAP);

//        node->getMaterial(0).getTextureMatrix(0).setTextureScale(200, 200);

    }
    setCollision(node,m_player);
    mesh->drop();



    mesh = smgr()->getMesh(Common::modelsPath()+"garbage.b3d");
        if (!mesh)
        {
            m_device->drop();
            return 1;
        }
        node = smgr()->addMeshSceneNode( mesh );
        if (node)
        {
            node->setScale(core::vector3df(86.0f,86.0f,86.0f));
            node->setRotation(core::vector3df(0,180,0));
            node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
            node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            node->setPosition(pos);
            //node->addShadowVolumeSceneNode(0,-1,false,5000.0f);
            node->setMaterialTexture( 0, texture("garbage_brick.jpg") );
    //        node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
    //        node->getMaterial(0).getTextureMatrix(0).setTextureScale(80,80);
    //        node->getMaterial(1).getTextureMatrix(0).setTextureScale(40,40);
    //        node->getMaterial(2).getTextureMatrix(0).setTextureScale(40,40);
            //node->setMaterialType(video::EMT_DETAIL_MAP);
            node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
            node->setMaterialFlag(video::EMF_FOG_ENABLE, m_config.fog());


    //        node->setMaterialTexture(1, texture("grass_dirty.jpg"));
    //        node->getMaterial(0).getTextureMatrix(1).setTextureScale(100,100);
    //        node->setMaterialType(video::EMT_DETAIL_MAP);

    //        node->getMaterial(0).getTextureMatrix(0).setTextureScale(200, 200);

        }


    return 0;
}

int GamePark::initSounds()
{
    StaticSoundEngine->play2D("../../media/sounds/nature.ogg", true);


    vec3df pos(32300.0f, 380.0f, 56080.0f);

    ISound* music = StaticSoundEngine->play3D("../../media/sounds/lake_waves.ogg",
                                              pos, true, false, true);
    if (music)
    {
       music->setMinDistance(300.0f);
       music->setVolume(1.0f);
    }

    return 0;
}

int GamePark::initAI()
{
    for(int i=0;i<0;i++)
    {
        int randomValue = 33 + (std::rand() % static_cast<int>((10000 - 2000 + 1)/60.0));
        MonsterNode* node = new MonsterNode(this);
        core::vector3df pos(92+randomValue, -100.0, 459+i*randomValue/2);
        node->setPosition( pos );
        node->setTerrain(m_terrain);
        m_aiNode.push_back( node );
    }
    updateMonsterCollision();
    return 0;
}

int GamePark::initRespawnPoints()
{
//    RespawnPoint* p = new RespawnPoint(this,core::vector3df(200,10,920));
//    m_respPoints.push_back(p);

//    p = new RespawnPoint(this,core::vector3df(200,10,850));
//    m_respPoints.push_back(p);

//    p = new RespawnPoint(this,core::vector3df(400,10,900));
//    m_respPoints.push_back(p);

//    p = new RespawnPoint(this,core::vector3df(400,10,1000));
//    m_respPoints.push_back(p);

    for(int i=0;i<m_config.respawn().count();i++)
    {
        m_respPoints.push_back( new RespawnPoint(this,core::vector3df(m_config.respawn().points()[i].m_x,
                                                                      m_config.respawn().points()[i].m_y,
                                                                      m_config.respawn().points()[i].m_z)) );
    }





    scene::IMesh* mesh = m_device->getSceneManager()->getMesh("../../media/models/bullet.b3d");
    DropNode* node = new DropNode(this, mesh, smgr()->getRootSceneNode(), smgr(), -1);
    node->setScale(core::vector3df(1,1,1));
    node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    node->setPosition(core::vector3df(200,12,850));
    node->setMaterialTexture( 0, m_device->getVideoDriver()->getTexture("../../media/textures/bullet.tga") );
    node->drop();



    return 0;
}

void GamePark::updateMonsterCollision()
{
    for(u32 i=0;i<m_aiNode.size();i++)
    {
        std::list<Enemy*>::iterator it = std::next(m_aiNode.begin(), i);
        Enemy* monster = *it;
        monster->removeAllTriangleSelector();
    }
    for(u32 i=0;i<m_aiNode.size();i++)
    {
        std::list<Enemy*>::iterator it = std::next(m_aiNode.begin(), i);
        Enemy* monster = *it;
        for(u32 j=0;j<i;j++)
        {
            std::list<Enemy*>::iterator it2 = std::next(m_aiNode.begin(), j);
            monster->addTriangleSelector(*it2);
        }
        monster->addTriangleSelector( m_whiteBoxSelector );
//        for(int i=0;i<m_forestSize;i++)
//            monster->addTriangleSelector( m_forestSelector[i] );
        monster->updateCollisionAnimator();
    }
}


void GamePark::setSpeedOfTime(f32 speed)
{
    m_speedOfTime = speed;
    speedOfTimeChanged.Emit(m_speedOfTime);
    // Monster loop
//    auto i = std::begin(m_aiNode);
//    while(i != std::end(m_aiNode))
//    {
//        MonsterNode* monster = *i;
//        monster->setSpeedOfTime(m_speedOfTime);
//        i++;
//    }
}

f32 GamePark::speedOfTime() const
{
    return m_speedOfTime;
}

int GamePark::initWhiteBox()
{
    scene::IMesh* mesh = smgr()->getMesh(Common::modelsPath()+"whitebox.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    scene::IMeshSceneNode* node = smgr()->addMeshSceneNode( mesh );
    if (node)
    {
        node->setRotation(core::vector3df(0,180,0));
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setPosition(core::vector3df(133,13.5,974));
        node->setMaterialTexture( 0, texture("plitka.tga") );
        node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
        node->getMaterial(0).getTextureMatrix(0).setTextureScale(400,400);
        node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);

    }
    m_whiteBoxSelector = smgr()->createOctreeTriangleSelector(mesh, node);
    setCollision(node,m_player);
    m_whiteBoxNode = node;
    mesh->drop();
    return 0;
}

int GamePark::initLadder()
{
    m_ladder = new LadderSceneNode(this);
    m_movableNode = (scene::IAnimatedMeshSceneNode*)m_ladder->node();
    m_ladder->setScale(core::vector3df(2,13,0.5));
    m_ladder->setPosition(core::vector3df(861, 23.65, 216));
    m_ladder->setRotation(core::vector3df(0,41,0));
    return 0;
}

int GamePark::initMenu()
{
    scene::IMesh* mesh;
    scene::IMeshSceneNode* node;
    mesh = smgr()->getMesh(Common::modelsPath()+"main_menu.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    node = smgr()->addMeshSceneNode( mesh );
    if (node)
    {
        node->setRotation(core::vector3df(0,180,0));
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setPosition(core::vector3df(287.5, 11, 998));
        node->setMaterialTexture( 0, texture("main_menu.tga") );
        node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
        node->getMaterial(0).MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
        node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);

    }
    setCollision(node,m_player);
    m_mainMenuNode = node;
    mesh->drop();

    mesh = smgr()->getMesh(Common::modelsPath()+"menu_new_game.b3d");
    if (!mesh)
    {
        m_device->drop();
        return 1;
    }
    node = smgr()->addMeshSceneNode( mesh );
    if (node)
    {
        node->setRotation(core::vector3df(0,180,0));
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setPosition(core::vector3df(222, 11, 971));
        node->setMaterialTexture( 0, texture("main_menu.tga") );
        node->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
        node->getMaterial(0).MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
        node->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);

    }
    setCollision(node,m_player);
    m_menuNewGameNode = node;
    mesh->drop();

    return 0;
}

int GamePark::initFader()
{
    m_fader = addInOutFader();
    return 0;
}

void GamePark::setCollision(scene::IAnimatedMeshSceneNode *node, Player *player)
{
    Collision::setCollision(node, player, smgr());
}

void GamePark::setCollision(scene::IMeshSceneNode *node, Player *player)
{
    Collision::setCollision(node, player, smgr());
}

video::ITexture *GamePark::texture(const io::path &filename)
{
    return driver()->getTexture(core::stringc("../../media/textures/")+filename);
}

void GamePark::switchTerrainMaterial(video::E_MATERIAL_FLAG material)
{
    switch (material) {
        case video::EMF_WIREFRAME:
            m_terrain->setMaterialFlag(video::EMF_WIREFRAME, !m_terrain->getMaterial(0).Wireframe);
            m_terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
        break;
        case video::EMF_POINTCLOUD:
            m_terrain->setMaterialFlag(video::EMF_POINTCLOUD, !m_terrain->getMaterial(0).PointCloud);
            m_terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
        break;
        default:
        break;
    }
}

void GamePark::switchTerrainDetailMap()
{
    video::E_MATERIAL_TYPE materialType = video::EMT_SOLID;
    if(m_terrain->getMaterial(0).MaterialType == video::EMT_SOLID)
    {
        materialType = video::EMT_DETAIL_MAP;
    }
    m_terrain->getMaterial(0).MaterialType = materialType;
}

void GamePark::switchSkybox(bool blue)
{
    skyboxBlue->setVisible(blue);
    skyboxRed->setVisible(!blue);
}

void GamePark::updateEnvironment(const core::stringw& str)
{
    m_fpsText->setText(str.c_str());
}

Player *GamePark::player() const
{
    return m_player;
}

scene::IAnimatedMeshSceneNode *GamePark::movableNode() const
{
    return m_movableNode;
}

InOutFader *GamePark::addInOutFader(const core::rect<s32> *rectangle, gui::IGUIElement *parent, s32 id)
{
    core::rect<s32> rect;

    if (rectangle)
        rect = *rectangle;
    else if (m_device->getVideoDriver())
        rect = core::rect<s32>(core::position2d<s32>(0,0), core::dimension2di(m_device->getVideoDriver()->getScreenSize()));

    if (!parent)
        parent = m_device->getGUIEnvironment()->getRootGUIElement();

    InOutFader* fader = new InOutFader(m_device, m_device->getGUIEnvironment(), parent, id, rect);
    fader->drop();
    return fader;
}

void GamePark::menuFlyCamera(const SceneMode& mode)
{
    core::array<core::vector3df> points;

    if(mode == SceneMode::MainMenu)
    {
        points.push_back(core::vector3df(174,25,994));
        points.push_back(core::vector3df(282, 13, 1000));
    }
    else if(mode == SceneMode::NewGame)
    {
        points.push_back(core::vector3df(282, 13, 1000));
        points.push_back(core::vector3df(216,14,972));
    }

    m_mainMenuCamera->setTarget(core::vector3df(541, 18, 840));

    scene::ISceneNodeAnimator* sa = 0;
    sa = smgr()->createFollowSplineAnimator(m_device->getTimer()->getTime(),
        points, 1, 0.5, false);
    m_mainMenuCamera->addAnimator(sa);
    sa->drop();
}


void GamePark::forestLOD(core::vector3df pos)
{
    float dist;
    for(int i=0;i<m_forestSize;i++)
    {
        dist = m_forest[i]->getPosition().getDistanceFrom(pos);
        if(dist < 25000/60.0 && !m_forest[i]->isVisible()){
            m_forest[i]->setVisible(true);
        }
        else if(dist > 25000/60.0 && m_forest[i]->isVisible()){
            m_forest[i]->setVisible(false);
        }
    }
}


void GamePark::finishGame()
{
    if(m_finish == true)
    {
        return;
    }
    m_finish = true;
    m_fader->setColor(  video::SColor ( 255,0,0,0 ), video::SColor ( 0, 0, 0, 0 ));
    m_fader->fadeOut(5000);
    m_fader->setVisible(true);
    m_fader->finished.connect([this](){
        m_fader->setVisible(false);
        setSceneMode(SceneMode::EndGame);
    });
}

void GamePark::gameOver()
{
    if(m_finish == true)
    {
        return;
    }
    m_finish = true;
    m_fader->setColor(  video::SColor ( 255,0,0,0 ), video::SColor ( 0, 0, 0, 0 ));
    m_fader->fadeOut(5000);
    m_fader->setVisible(true);
    m_fader->finished.connect([this](){
        m_fader->setVisible(false);
        setSceneMode(SceneMode::GameOver);
    });
}

void GamePark::setSceneMode(const SceneMode &mode)
{
    m_sceneMode = mode;
    if(m_sceneMode == SceneMode::MainMenu)
    {
        smgr()->setActiveCamera( m_mainMenuCamera );
        m_device->getCursorControl()->setVisible(true);
        m_mainMenuNode->setVisible(true);
        m_menuNewGameNode->setVisible(false);
        menuFlyCamera(m_sceneMode);
        switchSkybox(true);

        if(m_credits)
        {
            delete m_credits;
            m_credits = nullptr;
        }
        if(m_gameOverCredits)
        {
            delete m_gameOverCredits;
            m_gameOverCredits = nullptr;
        }

        auto i = std::begin(m_aiNode);
        while(i != std::end(m_aiNode))
        {
            Enemy* monster = *i;
            monster->node()->setVisible(false);
            i++;
        }
        m_screen = m_screenMainMenu;

        return;
    }
    if(m_sceneMode == SceneMode::Game)
    {
        auto i = std::begin(m_aiNode);
        while(i != std::end(m_aiNode))
        {
            Enemy* monster = *i;
            monster->node()->setVisible(true);
            i++;
        }
        m_player->setHealth(1.0);
        m_player->setPosition(174,15,994);

        m_finish = false;

        smgr()->setActiveCamera(m_player->camera());
        m_device->getCursorControl()->setVisible(false);
        m_mainMenuNode->setVisible(false);
        m_menuNewGameNode->setVisible(false);
        m_nuclearBoom->finished.disconnect_all();
        switchSkybox(false);

        return;
    }
    if(m_sceneMode == SceneMode::NewGame)
    {
        smgr()->setActiveCamera( m_mainMenuCamera );
        m_device->getCursorControl()->setVisible(true);
        m_mainMenuNode->setVisible(false);
        m_menuNewGameNode->setVisible(true);
        menuFlyCamera(m_sceneMode);
        return;
    }
    if(m_sceneMode == SceneMode::History)
    {

        player()->camera()->setUpVector(core::vector3df(0,1,0));
        player()->camera()->setInputReceiverEnabled(true);
        player()->node()->setVisible(true);
        player()->camera()->removeAnimator(player()->m_dieAnimator);
        player()->camera()->setTarget(core::vector3df(200,10,920));


        m_device->getCursorControl()->setVisible(false);
        m_mainMenuNode->setVisible(false);
        m_menuNewGameNode->setVisible(false);
        m_nuclearBoom->start();
        m_nuclearBoom->finished.connect([this](){setSceneMode(SceneMode::Game);});

        m_fader->setColor(video::SColor ( 255,255,255,255 ), video::SColor ( 0, 230, 230, 230 ));
        m_fader->fadeIn(5000);
        m_fader->setVisible(true);
        m_fader->finished.connect([this](){
            m_fader->setVisible(false);
            MyMessageBox::showStartMessage(device());
        });
        m_screen = m_screenSurvival;
        return;
    }
    if(m_sceneMode == SceneMode::EndGame)
    {
        m_credits = new MotionPictureCredits(this);
        m_screen = m_screenEndGame;
        return;
    }
    if(m_sceneMode == SceneMode::GameOver)
    {
        m_gameOverCredits = new GameOverCredits(this);
        m_screen = m_screenGameOver;
        return;
    }
    if(m_sceneMode == SceneMode::Loading)
    {

    }
}

int GamePark::run()
{
    scene::IBillboardSceneNode * bill;
    // Добавляем билборд, который будет подсвечивать место
    // пересечения луча и объекта в который он уперся
    bill = smgr()->addBillboardSceneNode();
    bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
    bill->setMaterialTexture(0, texture("particle.bmp"));
    bill->setMaterialFlag(video::EMF_LIGHTING, false);
    bill->setMaterialFlag(video::EMF_ZBUFFER, false);
    bill->setSize(core::dimension2d< f32 >(20.0f/60.0, 20.0f/60.0));
    // дадим билборду ID для объектов которые не реагируют на столкновения.
    bill->setID(ID_IsNotPickable);
//    scene::ISceneCollisionManager*  collMan;


    //---
    m_nuclearBoom = new NuclearBoomSceneNode(smgr()->getRootSceneNode(),smgr(),this,-1,
                                             core::vector3df(480.5, 51, 948),
                                             core::dimension2d<f32>(150, 150));
    //---
    scene::IParticleSystemSceneNode* ps =
        smgr()->addParticleSystemSceneNode(false);

    scene::IParticleEmitter* em = ps->createBoxEmitter(
        core::aabbox3d<f32>(-70/60.0,0,-70/60.0,70/60.0,10/60.0,70/60.0), // emitter size
        core::vector3df(0.0f,0.0005f,0.0f),   // initial direction
        8,10,                             // emit rate
        video::SColor(0,255,255,255),       // darkest color
        video::SColor(0,255,255,255),       // brightest color
        800,2000,0,                         // min and max age, angle
        core::dimension2df(100.f/60.0,100.f/60.0),         // min size
        core::dimension2df(200.f/60.0,200.f/60.0));        // max size

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(video::SColor(0,180,40,0), 300);

//    ps->addAffector(paf); // same goes for the affector
    paf->drop();


    ps->setPosition(core::vector3df(200,10,900));
//    ps->setScale(core::vector3df(2,2,2));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
//    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, texture("dust2.png"));
    ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

//    scene::ISceneNodeAnimator* sna = smgr()->createDeleteAnimator(1800);
//    ps->addAnimator(sna);
//    sna->drop();
    //---

    initScreens();
    m_screenLoading->draw();
    updateScreensValues();

    u32 timeBefore = 0;
    u32 timeAfter = 0;
    f32 middle = 0.0;
    u32 sum = 0;
    u32 count = 0;
    u32 frame = 0;

    while(m_device->run())
    {
        if(usl_exit)break;

        frame++;
        timeBefore = m_device->getTimer()->getRealTime();
        m_screen->draw();
        timeAfter = m_device->getTimer()->getRealTime();
        if(frame>300)
        {
            count++;
            sum += timeAfter - timeBefore;
        }
    }
    middle = (sum*1.0)/(count*1.0);
    TDEBUG() << "MIDDLE = " << middle << sum << count;


    bill->drop();

    m_device->closeDevice();
    //    m_device->drop();
    return 0;
}

int GamePark::initScreens()
{
    m_screenEndGame = new ScreenEndGame(this);
    m_screenGameOver = new ScreenGameOver(this);
    m_screenLoading = new ScreenLoading(this);
    m_screenMainMenu = new ScreenMainMenu(this);
    m_screenSurvival = new ScreenSurvival(this);
    return 0;
}

void GamePark::updateScreensValues()
{
    m_screenEndGame->updateValues();
    m_screenGameOver->updateValues();
    m_screenMainMenu->updateValues();
    m_screenSurvival->updateValues();
}

