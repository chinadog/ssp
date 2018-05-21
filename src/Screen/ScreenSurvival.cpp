#include "ScreenSurvival.h"
#include "GamePark.h"
#include <irrString.h>
#include "Common/Logger.h"

ScreenSurvival::ScreenSurvival(GamePark* gamePark) :
    Screen(gamePark)
{
    m_ladder = m_gamePark->m_ladder;
}

ScreenSurvival::~ScreenSurvival()
{
TERROR() << "DESTR SURV";
}

void ScreenSurvival::draw()
{
    int ii=0;
    TWARNING() << "i=" << ii++;
    m_driver->beginScene(true, true, 0 );TWARNING() << "i=" << ii++;
    m_player->draw();TWARNING() << "i=" << ii++;
    m_smgr->drawAll();TWARNING() << "i=" << ii++;
    m_env->drawAll();TWARNING() << "i=" << ii++;
    m_ladder->draw();TWARNING() << "i=" << ii++;
    m_driver->endScene();TWARNING() << "i=" << ii++;
    ShootIntersection shootIntersection = m_player->shootIntersection();
TWARNING() << "i=" << ii++;
    if(shootIntersection.isValid())
    {
//                bill->setPosition(shootIntersection.m_intersection);
    }TWARNING() << "i=" << ii++;
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
    }TWARNING() << "i=" << ii++;
//            driver()->endScene();

    if(m_gamePark->m_aiNode.size() < m_gamePark->m_config.count())
    {
        int rand = m_device->getRandomizer()->rand() % m_gamePark->m_respPoints.size();
        auto it = m_gamePark->m_respPoints.begin();
        // Advance the iterator by 2 positions,
        std::advance(it, rand);
        RespawnPoint* p = *it;
        p->createMonster();
    }TWARNING() << "i=" << ii++;

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
    }TWARNING() << "i=" << ii++;
    m_gamePark->m_checkFpsCounter++;
}

void ScreenSurvival::type()
{
    TWARNING() << "type = survival";
}
