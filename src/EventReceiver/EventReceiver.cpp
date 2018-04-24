#include "EventReceiver/EventReceiver.h"
#include "GamePark.h"
#include "Common/Common.h"

MyEventReceiver::MyEventReceiver(GamePark *gamePark) :
    m_gamePark(gamePark)
{
}

bool MyEventReceiver::OnEvent(const SEvent &event)
{
    scene::IAnimatedMeshSceneNode* animnode = m_gamePark->player()->node();
    io::IReadFile *heightmap = m_gamePark->heightmap;
    IrrlichtDevice* device = m_gamePark->device();
    scene::ITerrainSceneNode* terrain = m_gamePark->m_terrain;
    video::IVideoDriver* driver = m_gamePark->driver();
    scene::ISceneNode* moveNode = m_gamePark->movableNode();

    f32 moveDist = 0.05; // 0.5
    f32 scaleCoef = 0.2f;

    if(event.KeyInput.Shift)
    {
        moveDist *= 4;
        scaleCoef *= 4;
    }

    if(m_gamePark->player()->slowmo()->isActive())
    {
        return true;
    }

    if (event.EventType == irr::EET_KEY_INPUT_EVENT )
    {
        if(event.KeyInput.Key==KEY_KEY_W || event.KeyInput.Key==KEY_KEY_A ||
           event.KeyInput.Key==KEY_KEY_S || event.KeyInput.Key==KEY_KEY_D )
        {
            m_gamePark->player()->setKeyPressed(event.KeyInput.Key, event.KeyInput.PressedDown);

        }
        if(event.KeyInput.Key==160) // Shift
        {
//            m_gamePark->player()->setRun( !event.KeyInput.Shift );
            m_gamePark->player()->setKeyPressed(event.KeyInput.Key, event.KeyInput.PressedDown);
        }

        if(event.KeyInput.Key==KEY_KEY_1 || event.KeyInput.Key==KEY_KEY_2 || event.KeyInput.Key==KEY_KEY_3)
        {
            m_gamePark->player()->setWeapon(event.KeyInput.Key - 49);
        }
//        if((event.KeyInput.Key==KEY_KEY_1 || event.KeyInput.Key==KEY_KEY_2) &&
//            m_gamePark->player()->weaponAction() != WeaponAction::Draw)
//        {
//            m_gamePark->player()->setWeapon(event.KeyInput.Key - 49);
//        }
        if( event.KeyInput.Key==KEY_KEY_R )
        {
            m_gamePark->player()->setKeyPressed(event.KeyInput.Key, event.KeyInput.PressedDown);
        }


core::stringc str;

        switch (event.KeyInput.Key)
        {
        case irr::KEY_KEY_Q: // switch wire frame mode
            if(event.KeyInput.PressedDown == false)
            {
                m_gamePark->switchTerrainMaterial(video::EMF_WIREFRAME);
            }
            return true;
        case irr::KEY_KEY_P: // switch wire frame mode
            if(event.KeyInput.PressedDown == false)
            {
                m_gamePark->switchTerrainMaterial(video::EMF_POINTCLOUD);
            }
            return true;
        case irr::KEY_KEY_E: // toggle detail map
            if(event.KeyInput.PressedDown == false)
            {
                m_gamePark->switchTerrainDetailMap();
            }
            return true;
        case irr::KEY_KEY_Z: // toggle skies
            if(event.KeyInput.PressedDown == false)
            {
//                m_gamePark->switchSkybox();
            }
            return true;
        case irr::KEY_ESCAPE:
            m_gamePark->exit();
            return true;
        case irr::KEY_KEY_K:
            //exit_usl = true;
//            animnode->setFrameLoop(0,60);
//            animnode->setLoopMode(false);
            if(event.KeyInput.PressedDown == false)
            {
                if(m_gamePark->speedOfTime() < 1.0)
                {
                    m_gamePark->setSpeedOfTime(1.0);
                }
                else
                {
                    m_gamePark->setSpeedOfTime(0.3);
                }
            }
            return true;
        case irr::KEY_KEY_O: // switch wire frame mode
            //usl_exit = true;
            heightmap = device->getFileSystem()->createAndOpenFile("../../media/park-ter-5.png");
            terrain->loadHeightMap(heightmap,video::SColor( 255, 255, 255, 255 ),3);
            //if(!shadows)
            terrain->setMaterialFlag(video::EMF_LIGHTING, false);

            terrain->setMaterialTexture(0,
                    driver->getTexture("../../media/park-textura-5.png"));
            terrain->setMaterialTexture(1,
                    driver->getTexture("../../media/grass_dirty.jpg"));

            terrain->setMaterialType(video::EMT_DETAIL_MAP);

            terrain->scaleTexture(1.0f, 390.0f);
            return true;
        case irr::KEY_KEY_Y:
            moveNode->setPosition(core::vector3df(moveNode->getPosition().X+moveDist,
                                                  moveNode->getPosition().Y,
                                                  moveNode->getPosition().Z));
            std::cout << moveNode->getPosition().X << " " <<
                         moveNode->getPosition().Y << " " <<
                         moveNode->getPosition().Z << std::endl << std::flush;
            return true;
        case irr::KEY_KEY_H:
            moveNode->setPosition(core::vector3df(moveNode->getPosition().X-moveDist,
                                                  moveNode->getPosition().Y,
                                                  moveNode->getPosition().Z));
            std::cout << moveNode->getPosition().X << " " <<
                         moveNode->getPosition().Y << " " <<
                         moveNode->getPosition().Z << std::endl << std::flush;
            return true;
        case irr::KEY_KEY_G:
            moveNode->setPosition(core::vector3df(moveNode->getPosition().X,
                                                  moveNode->getPosition().Y,
                                                  moveNode->getPosition().Z+moveDist));
            std::cout << moveNode->getPosition().X << " " <<
                         moveNode->getPosition().Y << " " <<
                         moveNode->getPosition().Z << std::endl << std::flush;
            return true;
        case irr::KEY_KEY_J:
            moveNode->setPosition(core::vector3df(moveNode->getPosition().X,
                                                  moveNode->getPosition().Y,
                                                  moveNode->getPosition().Z-moveDist));
            std::cout << moveNode->getPosition().X << " " <<
                         moveNode->getPosition().Y << " " <<
                         moveNode->getPosition().Z << std::endl << std::flush;
            return true;
        case irr::KEY_KEY_T:
            moveNode->setPosition(core::vector3df(moveNode->getPosition().X,
                                                  moveNode->getPosition().Y+moveDist/3,
                                                  moveNode->getPosition().Z));
            std::cout << moveNode->getPosition().X << " " <<
                         moveNode->getPosition().Y << " " <<
                         moveNode->getPosition().Z << std::endl << std::flush;
            return true;
        case irr::KEY_KEY_U:
            moveNode->setPosition(core::vector3df(moveNode->getPosition().X,
                                                  moveNode->getPosition().Y-moveDist/3,
                                                  moveNode->getPosition().Z));
            std::cout << moveNode->getPosition().X << " " <<
                         moveNode->getPosition().Y << " " <<
                         moveNode->getPosition().Z << std::endl << std::flush;
            return true;
        case irr::KEY_PLUS:
            moveNode->setScale(core::vector3df(moveNode->getScale().X + scaleCoef,
                                               moveNode->getScale().Y + scaleCoef,
                                               moveNode->getScale().Z + scaleCoef));
            std::cout << moveNode->getScale().X << " " <<
                         moveNode->getScale().Y << " " <<
                         moveNode->getScale().Z << std::endl << std::flush;
            return true;
        case irr::KEY_MINUS:
            moveNode->setScale(core::vector3df(moveNode->getScale().X - scaleCoef,
                                               moveNode->getScale().Y - scaleCoef,
                                               moveNode->getScale().Z - scaleCoef));
            std::cout << moveNode->getScale().X << " " <<
                         moveNode->getScale().Y << " " <<
                         moveNode->getScale().Z << std::endl << std::flush;
            return true;
        case irr::KEY_KEY_N:
            moveNode->setRotation(core::vector3df(moveNode->getRotation().X ,
                                                  moveNode->getRotation().Y + scaleCoef,
                                                  moveNode->getRotation().Z ));
            std::cout << moveNode->getRotation().X << " " <<
                         moveNode->getRotation().Y << " " <<
                         moveNode->getRotation().Z << std::endl << std::flush;
            return true;
        case irr::KEY_KEY_M:
            moveNode->setRotation(core::vector3df(moveNode->getRotation().X ,
                                                  moveNode->getRotation().Y - scaleCoef,
                                                  moveNode->getRotation().Z ));
            std::cout << moveNode->getRotation().X << " " <<
                         moveNode->getRotation().Y << " " <<
                         moveNode->getRotation().Z << std::endl << std::flush;
            return true;

        default:
            break;
        }
    }

    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
        {
            scene::ISceneCollisionManager*  collMan = m_gamePark->smgr()->getSceneCollisionManager();
            core::vector2d<s32> pos = m_gamePark->device()->getCursorControl()->getPosition();
            scene::ISceneNode* node = collMan->getSceneNodeFromScreenCoordinatesBB(pos);
            if(m_gamePark->sceneMode() == SceneMode::MainMenu)
            {
                if(node == m_gamePark->m_mainMenuNode && pos.Y < 321)
                {
                    m_gamePark->setSceneMode(SceneMode::NewGame);
                }
            }
            else if(m_gamePark->sceneMode() == SceneMode::NewGame)
            {
                if(node == m_gamePark->m_menuNewGameNode /*&& pos.Y < 392*/)
                {
                    m_gamePark->setSceneMode(SceneMode::History);
                }
                else if(node == m_gamePark->m_menuNewGameNode /*&& pos.Y < 562*/)
                {
                    m_gamePark->setSceneMode(SceneMode::Survive);
                }
            }

        }

        m_gamePark->player()->setMouseButtonPressed(event.MouseInput.Event);
    }

    return false;
}
