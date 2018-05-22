#include "ScreenMainMenu.h"
#include "GamePark.h"

ScreenMainMenu::ScreenMainMenu(GamePark* gamePark) :
    Screen(gamePark)
{

}

void ScreenMainMenu::draw()
{
    m_driver->beginScene(true, true, 0 );
    m_player->draw();
    m_smgr->drawAll();
    m_env->drawAll();
    m_driver->endScene();

    if(m_gamePark->m_checkFpsCounter > 25)
    {
        m_gamePark->m_checkFpsCounter = 0;
        str = L"Driver [";
        str += m_driver->getName();
        str += "] FPS:";
        str += m_driver->getFPS();
        str += " \nTriangle: ";
        str += m_driver->getPrimitiveCountDrawn();
        str += " | Calls:";
        str += m_smgr->getParameters()->getAttributeAsInt("calls");

        m_gamePark->updateEnvironment(str);
    }
    m_gamePark->m_checkFpsCounter++;
}
