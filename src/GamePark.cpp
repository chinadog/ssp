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
}

void GamePark::exit()
{
    usl_exit = true;
}

int GamePark::initWorld()
{
    loadProgressbarChanged.Emit(50);
    initTerrain();
    loadProgressbarChanged.Emit(60);
    initWhiteBox();
    initLadder();
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
    loadProgressbarChanged.Emit(40);
    m_player->setWeapon(0);
    m_player->weapon(1)->addBullets(50);
    m_player->weapon(0)->addBullets(100);
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

    skybox=smgr()->addSkyBoxSceneNode(texture("skybox/irrlicht2_up.jpg"),
                                      texture("skybox/irrlicht2_dn.jpg"),
                                      texture("skybox/irrlicht2_lf.jpg"),
                                      texture("skybox/irrlicht2_rt.jpg"),
                                      texture("skybox/irrlicht2_ft.jpg"),
                                      texture("skybox/irrlicht2_bk.jpg"));
    skydome=smgr()->addSkyDomeSceneNode(texture("skybox/skydome.jpg"),16,8,0.95f,2.0f);
    skybox->setVisible(false);
    skydome->setVisible(true);

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

int GamePark::initForest()
{
    core::vector3df pos[m_forestSize];
    pos[0] = core::vector3df(20000/60.0,260*2/60.0,59360/60.0);
    pos[1] = core::vector3df(43630/60.0,260*2/60.0,49760/60.0);
    pos[2] = core::vector3df(15060/60.0,260*2/60.0,33010/60.0);
    pos[3] = core::vector3df(46400/60.0,260*2/60.0,26800/60.0);
    pos[4] = core::vector3df(46400/60.0,260*2/60.0,26800/60.0);
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

        for(uint i=0;i<node->getMaterialCount();i++)
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

void GamePark::switchSkybox()
{
    skybox->setVisible(!skybox->isVisible());
    skydome->setVisible(!skydome->isVisible());
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
    InOutFader* fader = addInOutFader();
    fader->setColor( video::SColor ( 0, 0, 0, 0 ), video::SColor ( 255,0,0,0 ));
    fader->fadeIn(5000);
    fader->finished.connect([this](){setSceneMode(SceneMode::EndGame);});
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
        return;
    }
    if(m_sceneMode == SceneMode::Game)
    {
        smgr()->setActiveCamera(m_player->camera());
        m_device->getCursorControl()->setVisible(false);
        m_mainMenuNode->setVisible(false);
        m_menuNewGameNode->setVisible(false);

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
        m_device->getCursorControl()->setVisible(false);
        m_mainMenuNode->setVisible(false);
        m_menuNewGameNode->setVisible(false);
//        setSceneMode(SceneMode::Game);
//        m_atomicBoom.start();


        InOutFader* fader = addInOutFader();
        fader->setColor(video::SColor ( 255,255,255,255 ), video::SColor ( 0, 230, 230, 230 ));
        fader->fadeIn(5000);
        fader->finished.connect([this](){MessageBox::showStartMessage(device());});


        return;
    }
    if(m_sceneMode == SceneMode::EndGame)
    {
        std::cout << "END GAME" << std::endl;
//        MotionPictureCredits* credits = addMotionPictureCredits();
//        credits.setText();
        return;
    }
    if(m_sceneMode == SceneMode::Loading)
    {
        core::dimension2d<u32> size = m_device->getVideoDriver()->getScreenSize();

        m_device->getCursorControl()->setVisible(false);

        // irrlicht logo
        int imageWidth  = 800;
        int imageHeight = 600;
        image = m_device->getGUIEnvironment()->addImage(m_device->getVideoDriver()->getTexture("../../media/textures/load/1.tga"),
                                core::position2d<s32>((size.Width - imageWidth)/2,(size.Height - imageHeight)/2));


        const int lwidth = size.Width - 20;
        const int lheight = 16;

        core::rect<int> pos(10, size.Height-lheight-10, 10+lwidth, size.Height-10);

//        m_device->getGUIEnvironment()->addImage(pos);
        statusText = m_device->getGUIEnvironment()->addStaticText(L"Loading...",	pos,false);
        statusText->setOverrideColor(video::SColor(255,205,200,200));
        statusText->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);



        loadProgressbarChanged.connect([this](int value){ wchar_t tmp[255]; swprintf(tmp,255, L"Loading... %3d", value); statusText->setText(tmp); driver()->beginScene(true, true, 0 );
            env()->drawAll();
            driver()->endScene();});

        driver()->beginScene(true, true, 0 );
        env()->drawAll();
        driver()->endScene();

        load();
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



    m_atomicBoom.setParent( this );
    setSceneMode(SceneMode::Undefined);

    while(m_device->run())
    {
        if(sceneMode() == SceneMode::Undefined)
        {
            setSceneMode(SceneMode::Loading);
        }

        if( m_atomicBoom.isBoom() )
        {
            m_atomicBoom.updateFrame();
        }




        // Work out a frame delta time.
//        const u32 now = m_device->getTimer()->getTime();
//        then = now;

        // RENDER -------------------------------------------------------------------------
//        const u32 timeBeforeRender = m_device->getTimer()->getRealTime();
        driver()->beginScene(true, true, 0 );
        m_player->draw();
        smgr()->drawAll();
        // MY BLUR DRAW
//        blur->draw(driver(), rtt0,m_config.params().WindowSize.Width,m_config.params().WindowSize.Height);
        // END MY BLUR DRAW

        env()->drawAll();
m_ladder->draw();

        driver()->endScene();









//        std::cout << "Time to render: " << m_device->getTimer()->getRealTime() - timeBeforeRender << " ms" << std::endl;
        // LOGIC -------------------------------------------------------------------------

//        const u32 timeBeforeLogic = m_device->getTimer()->getRealTime();

//        m_soundEngine->setListenerPosition(m_player->camera()->getPosition(),
//                                           m_player->camera()->getTarget());




        ShootIntersection shootIntersection = m_player->shootIntersection();

        if(shootIntersection.isValid())
        {
            bill->setPosition(shootIntersection.m_intersection);
        }

        if(usl_exit)break;


        // display frames per second in window title
        int fps = driver()->getFPS();
        int count = driver()->getPrimitiveCountDrawn();
//        if (lastFPS != fps)
        if(m_checkFpsCounter > 12)
        {
            m_checkFpsCounter = 0;
            core::stringw str = L"Driver [";
            str += driver()->getName();
            str += "] FPS:";
            str += fps;
            // Also print terrain height of current camera position
            // We can use camera position because terrain is located at coordinate origin
            str += " Triangle: ";
            str += count;
            str += " | Calls:";
            str += smgr()->getParameters()->getAttributeAsInt("calls");

            updateEnvironment(str);
//            forestLOD(m_player->camera()->getPosition());
        }
        m_checkFpsCounter++;
//        std::cout << "Time to Logic: " << m_device->getTimer()->getRealTime() - timeBeforeLogic << " ms" << std::endl;

    }

    bill->drop();

m_device->closeDevice();
//    m_device->drop();

return 0;
}

int GamePark::load()
{
    /*
     * Для начала добавим базовые элементы на сцену: окружение пользователя с текстом подсказок,
     * мир, игрока (камеру от первого лица)
    */
    loadProgressbarChanged.Emit(0);
    initCamera();
    initEnvironment();
    loadProgressbarChanged.Emit(20);
    initPlayer();
    initWorld();
    loadProgressbarChanged.Emit(90);
    initReceiver();
    initSounds();
    loadProgressbarChanged.Emit(100);

    m_player->shipPointPassed.connect_member(this, &GamePark::finishGame);

    Collision::addAnimator(m_player, m_metaTriangleSelector, smgr());

    env()->getRootGUIElement()->removeChild(image);
    m_fpsText->setVisible(true);
    statusText->setVisible(false);

    setSceneMode(SceneMode::History);
}


