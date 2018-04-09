#ifndef AK_H
#define AK_H

#include "Weapon/Weapon.h"

class AK : public Weapon
{
public:
    AK(GamePark* gamePark = 0);
    ~AK();

    AnimationFrameLoop animationFrameLoop(const WeaponAction &action);
    AnimationFrameLoop animationFrameLoop(const PlayerState &action);
    int shootBreakDelta() const;
    int shootDelta() const;
    void drawFinished();
    void reloadFinished();
    void initNode();
    void initSounds();
    void addBullets(int value);
    void removeBullets(int value);
    int bulletMaxCountInClip() const;






};

#endif // AK_H
