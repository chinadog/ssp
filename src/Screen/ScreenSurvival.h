#ifndef SCREENSURVIVAL_H
#define SCREENSURVIVAL_H

#include "Screen/Screen.h"
#include <irrString.h>

class LadderSceneNode;

class ScreenSurvival : public Screen
{
public:
    ScreenSurvival(GamePark* gamePark = 0);
    ~ScreenSurvival();

    void draw();
    void updateValues();
private:
    LadderSceneNode* m_ladder = nullptr;
    irr::core::stringw str;
};

#endif // SCREENSURVIVAL_H
