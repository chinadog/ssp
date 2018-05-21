#include "ScreenGameOver.h"
#include "GamePark.h"

ScreenGameOver::ScreenGameOver(GamePark* gamePark) :
    Screen(gamePark)
{

}

void ScreenGameOver::draw()
{
    m_gamePark->m_gameOverCredits->draw();
}
