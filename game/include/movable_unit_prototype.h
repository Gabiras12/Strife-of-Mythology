#ifndef SOMTD_MOVABLE_UNIT_PROTOTYPE_H
#define SOMTD_MOVABLE_UNIT_PROTOTYPE_H

#include <ijengine/game_object.h>
#include <ijengine/game_events_listener.h>

namespace SoMTD {
    class MovableUnitPrototype {
    public:
        virtual ~MovableUnitPrototype() {}
        virtual MovableUnitPrototype* clone() = 0;

    protected:

    private:
        int m_grid_x;
        int m_grid_y;
        int m_x;
        int m_y;
    };
}

#endif

