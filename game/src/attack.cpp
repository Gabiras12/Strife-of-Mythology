#include "attack.h"
#include "movable_unit.h"

SoMTD::Attack::Attack(double dmg, double as) :
        m_damage(dmg),
        m_attack_speed(as),
        m_cooldown(0)
{
}

double
SoMTD::Attack::damage() const
{
        return m_damage;
}

double
SoMTD::Attack::attack_speed() const
{
        return m_attack_speed;
}

void
SoMTD::Attack::attack(SoMTD::MovableUnit*&, unsigned int& now, unsigned int& last)
{
        if (m_cooldown < now) {
                m_cooldown = now+attack_speed()*1000;
        }
}

SoMTD::Attack::~Attack()
{
}

unsigned
SoMTD::Attack::cooldown() const
{
        return m_cooldown;
}
