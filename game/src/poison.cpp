#include "poison.h"
#include "attack.h"
#include <iostream>

SoMTD::Poison::Poison(double dmg, double as) : Attack(dmg, as)
{
}

void
SoMTD::Poison::attack(SoMTD::MovableUnit*& tgt, unsigned int& now, unsigned int& last)
{
        Attack::attack(tgt, now, last);
        tgt->suffer_poison(damage(), 8000, now, last);
}
