#include "ScreenLoading.h"
#include "GamePark.h"
#include "Common/Collision.h"

ScreenLoading::ScreenLoading(GamePark* gamePark) :
    Screen(gamePark)
{

}

void ScreenLoading::draw()
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

    m_gamePark->loadProgressbarChanged.connect([this](int value){ wchar_t tmp[255]; swprintf(tmp,255, L"Loading... %3d", value); statusText->setText(tmp); m_driver->beginScene(true, true, 0 );
        m_env->drawAll();
        m_driver->endScene();
    });

    m_driver->beginScene(true, true, 0 );
    m_env->drawAll();
    m_driver->endScene();

    load();
}

void ScreenLoading::load()
{
    /*
     * Для начала добавим базовые элементы на сцену: окружение пользователя с текстом подсказок,
     * мир, игрока (камеру от первого лица)
    */
    m_gamePark->loadProgressbarChanged.Emit(0);
    m_gamePark->initCamera();
    m_gamePark->initEnvironment();
    m_gamePark->loadProgressbarChanged.Emit(20);
    m_gamePark->initPlayer();
    m_gamePark->initWorld();
    m_gamePark->loadProgressbarChanged.Emit(90);
    m_gamePark->initReceiver();
    m_gamePark->initSounds();
    m_gamePark->loadProgressbarChanged.Emit(100);

    m_player = m_gamePark->player();
    m_player->shipPointPassed.connect_member(m_gamePark, &GamePark::finishGame);

    Collision::addAnimator(m_player, m_gamePark->m_metaTriangleSelector, m_smgr);

    m_env->getRootGUIElement()->removeChild(image);
    m_gamePark->m_fpsText->setVisible(true);
    statusText->setVisible(false);

    m_gamePark->setSceneMode(SceneMode::History);
}
