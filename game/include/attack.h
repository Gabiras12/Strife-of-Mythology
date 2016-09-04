#ifndef SOMTD_ATTACK_H
#define SOMTD_ATTACK_H

#include "movable_unit.h"

namespace SoMTD {
        class Attack {
        public:
                Attack(double dmg, double as);
                ~Attack();
                double damage() const;
                double attack_speed() const;
                virtual void attack(SoMTD::MovableUnit*&, unsigned int&, unsigned int&);
                unsigned cooldown() const;

        private:
                double m_damage;
                double m_attack_speed;
                unsigned m_cooldown;
        };
}
#endif
