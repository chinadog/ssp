#include "ScreenEndGame.h"
#include "GamePark.h"
#include "Common/Logger.h"

ScreenEndGame::ScreenEndGame(GamePark* gamePark) :
    Screen(gamePark)
{

}

void ScreenEndGame::draw()
{
    m_gamePark->m_credits->draw();
}

void ScreenEndGame::type()
{
    TDEBUG() << "type = end game";
}
