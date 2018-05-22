#include "ScreenSurvival.h"
#include "GamePark.h"
#include <irrString.h>
#include "Common/Logger.h"

ScreenSurvival::ScreenSurvival(GamePark* gamePark) :
    Screen(gamePark)
{
    updateValues();
}

ScreenSurvival::~ScreenSurvival()
{

}

void ScreenSurvival::draw()
{
    m_driver->beginScene(true, true, 0 );
    m_player->draw();
    m_smgr->drawAll();
    m_env->drawAll();
    m_ladder->draw();
    m_driver->endScene();
    ShootIntersection shootIntersection = m_player->shootIntersection();

    if(shootIntersection.isValid())
    {
//                bill->setPosition(shootIntersection.m_intersection);
    }
    // Monster loop
    auto i = std::begin(m_gamePark->m_aiNode);
    while(i != std::end(m_gamePark->m_aiNode))
    {
        MonsterNode* monster = *i;
        monster->draw();
        if(monster->node() == shootIntersection.m_node)
        {
            if(monster->isHeadshot(shootIntersection.m_intersection) &&
               shootIntersection.m_intersection.getDistanceFrom(m_player->camera()->getAbsolutePosition()) > 15.0)
            {
//                        monster->damage(1.0, shootIntersection.m_intersection);
                m_player->showSlowMoShoot(monster);
            }
            else
            {
                monster->damage(m_player->currentWeapon()->damage(),
                                shootIntersection.m_intersection);
            }
        }
        if(monster->life() == false)
        {
            i = m_gamePark->m_aiNode.erase(i);
            for(auto&& tmpMonster : m_gamePark->m_aiNode)
            {
                tmpMonster->removeTriangleSelector(monster);
                tmpMonster->updateCollisionAnimator();
            }
            delete monster;
            i--;
        }
        i++;
    }
//            driver()->endScene();

//    if(m_gamePark->m_aiNode.size() < m_gamePark->m_config.count())
//    {
//        int rand = m_device->getRandomizer()->rand() % m_gamePark->m_respPoints.size();
//        auto it = m_gamePark->m_respPoints.begin();
//        std::advance(it, rand);
//        RespawnPoint* p = *it;
//        p->createMonster();
//    }

    if(m_gamePark->m_aiNodeNew.size() < m_gamePark->m_config.count())
    {
        int rand = m_device->getRandomizer()->rand() % m_gamePark->m_respPoints.size();
        auto it = m_gamePark->m_respPoints.begin();
        std::advance(it, rand);
        RespawnPoint* p = *it;
        p->createMonsterNew();
    }

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

void ScreenSurvival::updateValues()
{
    Screen::updateValues();
    m_ladder = m_gamePark->m_ladder;
}

