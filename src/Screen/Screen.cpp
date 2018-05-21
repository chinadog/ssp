#include "Screen/Screen.h"
#include "GamePark.h"

Screen::Screen(GamePark *gamePark) :
    m_gamePark(gamePark)
{
    m_driver = m_gamePark->driver();
    m_player = m_gamePark->player();
    m_smgr = m_gamePark->smgr();
    m_env = m_gamePark->env();
    m_device = m_gamePark->device();
}

Screen::~Screen()
{

}

