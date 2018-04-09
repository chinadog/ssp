#ifndef CZ805_H
#define CZ805_H

#include "Weapon/Weapon.h"

using namespace irr;

class CZ805 : public Weapon
{
public:
    CZ805(GamePark* gamePark = 0);
    ~CZ805();

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

#endif // CZ805_H
