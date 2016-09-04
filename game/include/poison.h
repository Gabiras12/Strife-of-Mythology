#ifndef SOMTD_POISON_H
#define SOMTD_POISON_H

#include "attack.h"

namespace SoMTD {
        class Poison : public SoMTD::Attack {
        public:
                Poison(double dmg, double as);
                void attack(SoMTD::MovableUnit*&, unsigned int&, unsigned int&);

        private:
        };
}

#endif
