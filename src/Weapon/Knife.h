#ifndef KNIFE_H
#define KNIFE_H

#include "Weapon/Weapon.h"

class Knife : public Weapon
{
public:
    Knife(GamePark* gamePark = 0);
    ~Knife();

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

#endif // KNIFE_H
